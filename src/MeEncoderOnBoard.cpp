/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MeEncoderOnBoard
 * \brief   Driver for Encoder module on MeAuriga and MeMegaPi.
 * @file    MeEncoderOnBoard.cpp
 * @author  MakeBlock
 * @version V1.0.5
 * @date    2018/01/03
 * @brief   Driver for Encoder module on MeAuriga and MeMegaPi.
 *
 * \par Copyright
 * This software is Copyright (C), 2012-2016, MakeBlock. Use is subject to license \n
 * conditions. The main licensing options available are GPL V2 or Commercial: \n
 *
 * \par Open Source Licensing GPL V2
 * This is the appropriate option if you want to share the source code of your \n
 * application with everyone you distribute it to, and you also want to give them \n
 * the right to share who uses it. If you wish to use this software under Open \n
 * Source Licensing, you must contribute all your source code to the open source \n
 * community in accordance with the GPL Version 2 when your application is \n
 * distributed. See http://www.gnu.org/copyleft/gpl.html
 *
 * \par Description
 * This file is a drive for Encoder On MeAuriga and MeMegaPi.
 *
 * \par Method List:
 *
 *    1. void MeEncoderOnBoard::reset(uint8_t slot);
 *    2. uint8_t MeEncoderOnBoard::getSlotNum(void);
 *    3. uint8_t MeEncoderOnBoard::getIntNum(void);
 *    4. uint8_t MeEncoderOnBoard::getPortA(void);
 *    5. uint8_t MeEncoderOnBoard::getPortB(void);
 *    6. long MeEncoderOnBoard::getPulsePos(void);
 *    7. void MeEncoderOnBoard::setPulsePos(long pulse_pos);
 *    8. void MeEncoderOnBoard::pulsePosPlus(void);
 *    9. void MeEncoderOnBoard::pulsePosMinus(void);
 *    10. void MeEncoderOnBoard::setCurrentSpeed(float speed);
 *    11. float MeEncoderOnBoard::getCurrentSpeed(void);
 *    12. int16_t MeEncoderOnBoard::getCurPwm(void);
 *    13. void MeEncoderOnBoard::setTarPWM(int16_t pwm_value);
 *    14. void MeEncoderOnBoard::setMotorPwm(int16_t pwm);
 *    15. void MeEncoderOnBoard::updateSpeed(void);
 *    16. void MeEncoderOnBoard::updateCurPos(void);
 *    17. long MeEncoderOnBoard::getCurPos(void)
 *    18. void MeEncoderOnBoard::runSpeed(float speed);
 *    19. void MeEncoderOnBoard::setSpeed(float speed);
 *    20. void MeEncoderOnBoard::move(long position,float speed,int16_t extId,cb callback);
 *    21. void MeEncoderOnBoard::moveTo(long position,float speed,int16_t extId,cb callback);
 *    22. long MeEncoderOnBoard::distanceToGo(void);
 *    23. void MeEncoderOnBoard::setSpeedPid(float p,float i,float d);
 *    24. void MeEncoderOnBoard::setPosPid(float p,float i,float d);
 *    25. void MeEncoderOnBoard::setPulse(int16_t pulseValue);
 *    26. void MeEncoderOnBoard::setRatio(int16_t RatioValue);
 *    27. void MeEncoderOnBoard::setMotionMode(int16_t motionMode);
 *    28. int16_t MeEncoderOnBoard::pidPositionToPwm(void);
 *    29. int16_t MeEncoderOnBoard::speedWithoutPos(void);
 *    30. void MeEncoderOnBoard::encoderMove(void);
 *    31. void MeEncoderOnBoard::pwmMove(void);
 *    32. boolean MeEncoderOnBoard::isTarPosReached(void);
 *    33. void MeEncoderOnBoard::loop(void);
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * forfish         2015/11/10     1.0.0            Add description
 * Mark Yan        2016/04/07     1.0.1            fix motor reset issue.
 * Mark Yan        2016/05/17     1.0.2            add some comments.
 * Mark Yan        2016/06/25     1.0.3            add PID calibration for encoder driver.
 * Zzipeng         2017/05/22     1.0.4            when motor turn its direction.
 * Mark Yan        2018/01/03     1.0.4            add callback flag.
 * </pre>
 *
 * @example Me_Auriga_encoder_direct.ino
 * @example Me_Auriga_encoder_pid_pos.ino
 * @example Me_Auriga_encoder_pid_speed.ino
 * @example Me_Auriga_encoder_pwm.ino
 * @example Me_Auriga_encoder_callback.ino
 * @example Me_Megapi_encoder_direct.ino
 * @example Me_Megapi_encoder_pid_pos.ino
 * @example Me_Megapi_encoder_pid_speed.ino
 * @example Me_Megapi_encoder_pwm.ino
 */

#include "MeEncoderOnBoard.h"

/**
 * Alternate Constructor which can call your own function to map the Encoder motor to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
MeEncoderOnBoard::MeEncoderOnBoard()
{

}

/**
 * Alternate Constructor which can call your own function to map the Encoder motor to arduino port
 * \param[in]
 *   slot - megapi slot from SLOT1 to SLOT4(Auriga SLOT1 and SLOT2).
 */
MeEncoderOnBoard::MeEncoderOnBoard(int slot)
{
  _enabled = false;
  _Slot = slot;
  _Port_A = encoder_Port[slot].port_A;
  _Port_B = encoder_Port[slot].port_B;
  _Port_PWM = encoder_Port[slot].port_PWM;
  _Port_H1 = encoder_Port[slot].port_H1;
  _Port_H2 = encoder_Port[slot].port_H2;
  _Callback_flag = false;

  pinMode(_Port_A, INPUT_PULLUP);
  pinMode(_Port_B, INPUT_PULLUP);
  pinMode(_Port_H1, OUTPUT);
  pinMode(_Port_H2, OUTPUT);
  
  encode_structure.pulsePos = 0;
  encode_structure.previousPwm = 500;
  encode_structure.mode = DIRECT_MODE;
  encode_structure.pulseEncoder = 9;
  encode_structure.ratio = 39.267;
  
  if(_Port_A == 18)
  {
    _IntNum = 5;
  }
  else if(_Port_A == 19)
  {
    _IntNum = 4;
  }
  else if(_Port_A == 20)
  {
    _IntNum = 3;
  }
  else if(_Port_A == 21)
  {
    _IntNum = 2;
  }
  else if(_Port_A == 3)
  {
    _IntNum = 1;
  }
  else if(_Port_A == 2)
  {
    _IntNum = 0;
  }
  setMotorPwm(0);
  setPulsePos(0);
  _Measurement_speed_time = millis();
  reset(slot);
}

/**
 * \par Function
 *   reset
 * \par Description
 *   Reset the encoder motor available PIN by Auriga/MegaPi slot.
 * \param[in]
 *   slot - MegaPi encoder port from SLOT1 to SLOT4(Auriga SLOT1 and SLOT2).
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
void MeEncoderOnBoard::reset(uint8_t slot)
{
  _enabled = true;
  _Slot = slot;
  _Port_A = encoder_Port[slot].port_A;
  _Port_B = encoder_Port[slot].port_B;
  _Port_PWM = encoder_Port[slot].port_PWM;
  _Port_H1 = encoder_Port[slot].port_H1;
  _Port_H2 = encoder_Port[slot].port_H2;
  _Callback_flag = false;

  pinMode(_Port_A, INPUT_PULLUP);
  pinMode(_Port_B, INPUT_PULLUP);
  pinMode(_Port_H1, OUTPUT);
  pinMode(_Port_H2, OUTPUT);
  
  // encode_structure.pulsePos = 0;
  // encode_structure.previousPwm = 500;
  // encode_structure.mode = DIRECT_MODE;
  // encode_structure.pulseEncoder = 9;
  // encode_structure.ratio = 39.267;
  
  if(_Port_A == 18)
  {
    _IntNum = 5;
  }
  else if(_Port_A == 19)
  {
    _IntNum = 4;
  }
  else if(_Port_A == 20)
  {
    _IntNum = 3;
  }
  else if(_Port_A == 21)
  {
    _IntNum = 2;
  }
  else if(_Port_A == 3)
  {
    _IntNum = 1;
  }
  else if(_Port_A == 2)
  {
    _IntNum = 0;
  }
  // setMotorPwm(0);
  // setPulsePos(0);
  _Measurement_speed_time = millis();
}

/**
 * \par Function
 *   getSlotNum
 * \par Description
 *   This function used to get the Auriga/MegaPi slot number of current objects.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   uint8_t - The slot number of current objects \n
 * \par Others
 *   None
 */
uint8_t MeEncoderOnBoard::getSlotNum(void) const
{
  return _Slot;
}

/**
 * \par Function
 *   getIntNum
 * \par Description
 *   This function used to get the Auriga/MegaPi Interrupt number of current objects.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   uint8_t - The Interrupt number of current objects \n
 * \par Others
 *   None
 */
uint8_t MeEncoderOnBoard::getIntNum(void) const
{
  return _IntNum;
}

/**
 * \par Function
 *   getPortA
 * \par Description
 *   This function used to get the GPIO number of current objects's dir port A.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   uint8_t - The GPIO number of current objects's dir port A \n
 * \par Others
 *   None
 */
uint8_t MeEncoderOnBoard::getPortA(void) const
{
  return _Port_A;
}

/**
 * \par Function
 *   getPortB
 * \par Description
 *   This function used to get the GPIO number of current objects's dir port B.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   uint8_t - The GPIO number of current objects's dir port B \n
 * \par Others
 *   None
 */
uint8_t MeEncoderOnBoard::getPortB(void) const
{
  return _Port_B;
}

/**
 * \par Function
 *   getPulsePos
 * \par Description
 *   This function used to get the current pos value(pulse counter).
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   long - current pos value \n
 * \par Others
 *   None
 */
long MeEncoderOnBoard::getPulsePos(void) const
{
  return encode_structure.pulsePos;
}

/**
 * \par Function
 *   setPulsePos
 * \par Description
 *   This function used to Set the current pos value(pulse counter). Generally used for\n
 *   reset the distance calculation.
 * \param[in]
 *   pulsePos - the value of pos value(pulse counter)
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
void MeEncoderOnBoard::setPulsePos(long pulsePos)
{
  encode_structure.pulsePos = pulsePos;
}

/**
 * \par Function
 *   pulsePosPlus
 * \par Description
 *   This function used to increase the current pos value(pulse counter).
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
void MeEncoderOnBoard::pulsePosPlus(void)
{
  encode_structure.pulsePos++;
}

/**
 * \par Function
 *   pulsePosMinus
 * \par Description
 *   This function used to reduction the current pos value(pulse counter).
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
void MeEncoderOnBoard::pulsePosMinus(void)
{
  encode_structure.pulsePos--;
}

/**
 * \par Function
 *   setCurrentSpeed
 * \par Description
 *   This function used to set the current speed(The unit is rpm).
 * \param[in]
 *   speed - the speed value(The unit is rpm).
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
void MeEncoderOnBoard::setCurrentSpeed(float speed)
{
  encode_structure.currentSpeed = speed;
}

/**
 * \par Function
 *   getCurrentSpeed
 * \par Description
 *   This function used to get the current speed(The unit is rpm).
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   float - the speed value(The unit is rpm).
 * \par Others
 *   None
 */
float MeEncoderOnBoard::getCurrentSpeed(void) const
{
  return encode_structure.currentSpeed;
}

/**
 * \par Function
 *   getCurPwm
 * \par Description
 *   This function used to get the current pwm setting.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   int16_t - the pwm setting
 * \par Others
 *   None
 */
int16_t MeEncoderOnBoard::getCurPwm(void) const
{
  return encode_structure.currentPwm;
}

/**
 * \par Function
 *    setTarPWM
 * \par Description
 *    This function used for set the target pwm value.
 * \param[in]
 *    pwm_value - the target pwm value.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderOnBoard::setTarPWM(int16_t pwm_value)
{
  encode_structure.mode = PWM_MODE;
  encode_structure.targetPwm = constrain(pwm_value,-255,255);;
}

/**
 * \par Function
 *   setMotorPwm
 * \par Description
 *   This function used to execute current pwm setting.
 * \param[in]
 *   pwm - the pwm setting
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
void MeEncoderOnBoard::setMotorPwm(int16_t pwm)
{
  pwm = constrain(pwm,-255,255);
  if(encode_structure.previousPwm != pwm)
  {
    encode_structure.previousPwm = pwm;
  }
  else
  {
    return;
  }
  encode_structure.currentPwm = pwm;

  if(pwm < 0)
  {
    digitalWrite(MeEncoderOnBoard::_Port_H1, LOW);
    delayMicroseconds(5);
    digitalWrite(MeEncoderOnBoard::_Port_H2, HIGH);
    analogWrite(MeEncoderOnBoard::_Port_PWM, abs(pwm));
  }
  else
  {
    digitalWrite(MeEncoderOnBoard::_Port_H1, HIGH);
    delayMicroseconds(5);
    digitalWrite(MeEncoderOnBoard::_Port_H2, LOW);
    analogWrite(MeEncoderOnBoard::_Port_PWM, abs(pwm));
  }
}

/**
 * \par Function
 *   updateSpeed
 * \par Description
 *   This function used to update current speed.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
void MeEncoderOnBoard::updateSpeed(void)
{
  if((millis() - _Measurement_speed_time) > 20)
  {
    uint16_t dt = millis() - _Measurement_speed_time;
    long cur_pos = getPulsePos();
    setCurrentSpeed(((cur_pos - encode_structure.previousPos) *(1000.0/dt)*60.0) /(encode_structure.pulseEncoder * encode_structure.ratio));
    encode_structure.previousPos = cur_pos;
    _Measurement_speed_time = millis();
  }
}

/**
 * \par Function
 *    updateCurPos
 * \par Description
 *    This function used to calculate the current position(The units are degrees).
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderOnBoard::updateCurPos(void)
{
  encode_structure.currentPos = (long)((encode_structure.pulsePos/(encode_structure.pulseEncoder * encode_structure.ratio)) * 360);
}

/**
 * \par Function
 *    getCurPos
 * \par Description
 *    This function used to get the current position(The units are degrees).
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
long MeEncoderOnBoard::getCurPos(void) const
{
  return encode_structure.currentPos;
}

/**
 * \par Function
 *    runSpeed
 * \par Description
 *    The speed of encode, and encoder motor will running with the setting speed.
 * \param[in]
 *    speed - the speed value(The unit is rpm).
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderOnBoard::runSpeed(float speed)
{
  encode_structure.mode = PID_MODE;
  encode_structure.motionState = MOTION_WITHOUT_POS;
  encode_structure.targetSpeed = speed;
  _Lock_flag = false;
}

/**
 * \par Function
 *   setSpeed
 * \par Description
 *   This function used to set the current speed(The unit is rpm).\n
 *   Note: used with update function.
 * \param[in]
 *   speed - the speed value(The unit is rpm).
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
void MeEncoderOnBoard::setSpeed(float speed)
{
  encode_structure.motionState = MOTION_WITHOUT_POS;
  encode_structure.targetSpeed = speed;
  _Lock_flag = false;
}

/**
 * \par Function
 *    move
 * \par Description
 *    encoder motor moves to the relative positions.
 * \param[in]
 *    position - The relative angle encoder motor moves.
 * \param[in]
 *    speed - the speed value(The unit is rpm).
 * \param[in]
 *    extId - It is used to indicate the ID of motor(Optional parameters).
 * \param[in]
 *    callback - callback function when the target position has been reached(Optional parameters).
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderOnBoard::move(long position,float speed,int16_t extId,cb callback)
{
  encode_structure.targetPos = encode_structure.currentPos + position;
  moveTo(encode_structure.targetPos,speed,extId,callback);
}

/**
 * \par Function
 *    moveTo
 * \par Description
 *    encoder motor moves to the absolute position.
 * \param[in]
 *    position - The absolute angle encoder motor moves.
 * \param[in]
 *    speed - the speed value(The unit is rpm).
 * \param[in]
 *    extId - It is used to indicate the ID of motor(Optional parameters).
 * \param[in]
 *    callback - callback function when the target position has been reached(Optional parameters).
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderOnBoard::moveTo(long position,float speed,int16_t extId,cb callback)
{
  encode_structure.targetSpeed = speed;
  _extId = extId;
  _Lock_flag = false;
  _Callback_flag = false;
  encode_structure.mode = PID_MODE;
  encode_structure.motionState = MOTION_WITH_POS;
  encode_structure.targetPos = position;
  _callback = callback;
  if(distanceToGo() > 0)
  {
    _Dir_lock_flag = true;
  }
  else
  {
    _Dir_lock_flag = false;
  }
}

/**
 * \par Function
 *    distanceToGo
 * \par Description
 *    The distance that encoder should go, units are in degrees.\r\n
 *    So 360-degree means a circle.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    long - Return the length that encoder need run.
 * \par Others
 *    None
 */
long MeEncoderOnBoard::distanceToGo(void) const
{
  return encode_structure.targetPos - encode_structure.currentPos;
}

/**
 * \par Function
 *    setSpeedPid
 * \par Description
 *    This function used to set the PID parameters for encoder motor's speed.
 * \param[in]
 *    p - Proportion parameter
 * \param[in]
 *    i - Integration Parameters
 * \param[in]
 *    d - Differential parameter
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderOnBoard::setSpeedPid(float p,float i,float d)
{
  encode_structure.PID_speed.P = p;
  encode_structure.PID_speed.I = i;
  encode_structure.PID_speed.D = d;
}

/**
 * \par Function
 *    setPosPid
 * \par Description
 *    This function used to set the PID parameters for encoder motor's pos.
 * \param[in]
 *    p - Proportion parameter
 * \param[in]
 *    i - Integration Parameters
 * \param[in]
 *    d - Differential parameter
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderOnBoard::setPosPid(float p,float i,float d)
{
  encode_structure.PID_pos.P = p;
  encode_structure.PID_pos.I = i;
  encode_structure.PID_pos.D = d;
}

/**
 * \par Function
 *    setPulse
 * \par Description
 *    This function used to set the pulse number of encoder code disc.
 * \param[in]
 *    pulseValue - pulse number
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderOnBoard::setPulse(int16_t pulseValue)
{
  encode_structure.pulseEncoder = pulseValue;
}

/**
 * \par Function
 *    setRatio
 * \par Description
 *    This function used to set the ratio of encoder motor.
 * \param[in]
 *    RatioValue - ratio number
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderOnBoard::setRatio(int16_t RatioValue)
{
  encode_structure.ratio = RatioValue;
}

/**
 * \par Function
 *    setMotionMode
 * \par Description
 *    This function used to set the motion mode of encoder motor.
 * \param[in]
 *    motionMode - motion mode
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderOnBoard::setMotionMode(int16_t motionMode)
{
  encode_structure.mode = motionMode;
}

/**
 * \par Function
 *    pidPositionToPwm
 * \par Description
 *    This function used to calculate the PWM motor value of encoder motor(position & speed).
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
int16_t MeEncoderOnBoard::pidPositionToPwm(void)
{
  float seek_speed = 0;
  float pos_error = 0;
  float speed_error = 0;
  float d_component = 0;
  float out_put_offset = 0;

  pos_error = distanceToGo();

  if((_Lock_flag == false) && (_Dir_lock_flag == true) && (pos_error < 0))
  {
    d_component = encode_structure.currentSpeed;
    out_put_offset = encode_structure.PID_pos.D * d_component;
    encode_structure.PID_pos.Output = -out_put_offset;
    _Encoder_output = encode_structure.PID_pos.Output;
    _Lock_flag = true;
    encode_structure.currentPwm = _Encoder_output;
    return _Encoder_output;
  }
  else if((_Lock_flag == false) && (_Dir_lock_flag == false) && (pos_error > 0))
  {
    d_component = encode_structure.currentSpeed;
    out_put_offset = encode_structure.PID_pos.D * d_component;
    encode_structure.PID_pos.Output = -out_put_offset;
    _Encoder_output = encode_structure.PID_pos.Output;
    _Lock_flag = true;
    encode_structure.currentPwm = _Encoder_output;
    return _Encoder_output;
  }
      
  //speed pid;
  if((_Lock_flag == false) && (abs(pos_error) >= encode_structure.targetSpeed * DECELERATION_DISTANCE_PITCH))
  {
    speed_error = encode_structure.currentSpeed - encode_structure.targetSpeed * (pos_error/abs(pos_error));
    out_put_offset = encode_structure.PID_speed.P * speed_error;
    out_put_offset = constrain(out_put_offset,-25,25);
    encode_structure.PID_speed.Output = _Encoder_output;
    encode_structure.PID_speed.Output -= out_put_offset;  
    encode_structure.PID_speed.Output = constrain(encode_structure.PID_speed.Output,-255,255);
    _Encoder_output = encode_structure.PID_speed.Output;
  }
  //position pid;
  else
  {
    if((_Lock_flag == false) && (abs(pos_error) > ENCODER_POS_DEADBAND))
    {
      seek_speed = sqrt(abs(encode_structure.targetSpeed * DECELERATION_DISTANCE_PITCH * (pos_error-ENCODER_POS_DEADBAND)))/DECELERATION_DISTANCE_PITCH;
      d_component = encode_structure.currentSpeed - seek_speed * (pos_error/abs(pos_error));
      out_put_offset = encode_structure.PID_pos.D * d_component;
      out_put_offset = constrain(out_put_offset,-20,20);

      encode_structure.PID_pos.Output = _Encoder_output;
      encode_structure.PID_pos.Output -= out_put_offset;
      if(pos_error >= 0)
      {
        encode_structure.PID_pos.Output = constrain(encode_structure.PID_pos.Output,PWM_MIN_OFFSET,255);
      }
      else
      {
        encode_structure.PID_pos.Output = constrain(encode_structure.PID_pos.Output,-255,-PWM_MIN_OFFSET);
      }
      _Encoder_output = encode_structure.PID_pos.Output;
    }
    else
    {
      _Lock_flag = true;
      if((_callback != NULL) && (_Callback_flag == false))
      {
        _Callback_flag = true;
        _callback(_Slot,_extId);
      }
      d_component = encode_structure.currentSpeed;
      out_put_offset = encode_structure.PID_pos.D * d_component;
      out_put_offset = constrain(out_put_offset,-20,20);
      encode_structure.PID_pos.Output = pos_error * encode_structure.PID_pos.P;
      encode_structure.PID_pos.Output -= out_put_offset;
      encode_structure.PID_pos.Output = constrain(encode_structure.PID_pos.Output,-255,255);
      _Encoder_output = encode_structure.PID_pos.Output;
    }
  }
#ifdef DEBUG_INFO
  Serial.print("targetPos:");
  Serial.print(encode_structure.targetPos);
  Serial.print(" ,currentPos:");
  Serial.print(encode_structure.currentPos);
  Serial.print(" ,targetSpeed:");
  Serial.print(encode_structure.targetSpeed);
  Serial.print(" ,currentSpeed:");
  Serial.print(encode_structure.currentSpeed);
  Serial.print(" ,pos_error:");
  Serial.print(pos_error);
  Serial.print(" ,d_component:");
  Serial.print(d_component);
  Serial.print(" ,motion_state:");
  Serial.print(encode_structure.motionState);
  Serial.print(" ,out1:");
  Serial.println(_Encoder_output);
#endif
  return _Encoder_output;
}

/**
 * \par Function
 *    speedWithoutPos
 * \par Description
 *    This function used to calculate the PWM motor value of encoder motor(single speed).
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
int16_t MeEncoderOnBoard::speedWithoutPos(void)
{
  float speed_error;
  float out_put_offset;
  speed_error = encode_structure.currentSpeed - encode_structure.targetSpeed;
  out_put_offset = encode_structure.PID_speed.P * speed_error;

  out_put_offset = constrain(out_put_offset,-25,25);
  encode_structure.PID_speed.Output = _Encoder_output;
  encode_structure.PID_speed.Output -= out_put_offset;

  if(_Lock_flag == true)
  {
    encode_structure.PID_speed.Output = 0;
  }

  if((_Lock_flag == false) && (encode_structure.targetSpeed == 0) && (abs(out_put_offset) < 15))
  {
    encode_structure.PID_speed.Output = 0;
    _Lock_flag = true;
  }

  encode_structure.PID_speed.Output = constrain(encode_structure.PID_speed.Output,-255,255);
 _Encoder_output = encode_structure.PID_speed.Output;
#ifdef DEBUG_INFO
  Serial.print("Mode:");
  Serial.print(encode_structure.mode);
  Serial.print(" ,tar:");
  Serial.print(encode_structure.targetSpeed);
  Serial.print(" ,cur:");
  Serial.print(encode_structure.currentSpeed);
  Serial.print(" ,speed_error:");
  Serial.print(speed_error);
  Serial.print(" ,out:");
  Serial.println(_Encoder_output);
#endif /* DEBUG_INFO */
  return _Encoder_output;
}

/**
 * \par Function
 *    encoderMove
 * \par Description
 *    This function used for move with PID mode.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderOnBoard::encoderMove(void)
{
  if(millis() - _Encoder_move_time > 40)
  {
    int16_t pwm_encoder = 0;
    _Encoder_move_time = millis();
    if(encode_structure.motionState == MOTION_WITH_POS)
    {
      pwm_encoder = pidPositionToPwm();
    }
    else if(encode_structure.motionState == MOTION_WITHOUT_POS)
    {
      pwm_encoder = speedWithoutPos();
    }
    encode_structure.currentPwm = pwm_encoder;
  }
}

/**
 * \par Function
 *    pwmMove
 * \par Description
 *    This function used for move with PWM mode.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderOnBoard::pwmMove(void)
{
  if(millis() - _Encoder_move_time > 40)
  {
    _Encoder_move_time = millis();
    encode_structure.currentPwm = round(0.8 * encode_structure.currentPwm + 0.2 * encode_structure.targetPwm);
    if((abs(encode_structure.currentPwm) <= 10) && (encode_structure.targetPwm == 0))
    {
      encode_structure.currentPwm = 0;
    }
  }
}

/**
 * \par Function
 *    isTarPosReached
 * \par Description
 *    This function used to determine whether the motor come to the target position.\n
 *    It is mainly applied to function moveTo or function move
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    true - target position is reached, otherwise
 * \par Others
 *    None
 */
boolean MeEncoderOnBoard::isTarPosReached(void) const
{
  return _Lock_flag;
}

/**
 * \par Function
 *    loop
 * \par Description
 *    This function should be called without Blocked.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderOnBoard::loop(void)
{
  if(!_enabled)return;
  updateCurPos();
  updateSpeed();

  if(encode_structure.mode == PID_MODE)
  {
    encoderMove();
    setMotorPwm(encode_structure.currentPwm);
  }
  else if(encode_structure.mode == PWM_MODE)
  {
    pwmMove();
    setMotorPwm(encode_structure.currentPwm);
  }
}
