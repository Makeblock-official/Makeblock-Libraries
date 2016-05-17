/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MeEncoderOnBoard
 * \brief   Driver for Encoder module on MeAuriga and MeMegaPi.
 * @file    MeEncoderOnBoard.cpp
 * @author  MakeBlock
 * @version V1.0.2
 * @date    2016/05/17
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
 *    2. uint8_t MeEncoderOnBoard::GetSlotNum(void);
 *    3. uint8_t MeEncoderOnBoard::GetIntNum(void);
 *    4. uint8_t MeEncoderOnBoard::GetPortA(void);
 *    5. uint8_t MeEncoderOnBoard::GetPortB(void);
 *    6. long MeEncoderOnBoard::GetPulsePos(void);
 *    7. void MeEncoderOnBoard::SetPulsePos(long pulse_pos);
 *    8. void MeEncoderOnBoard::PulsePosPlus(void);
 *    9. void MeEncoderOnBoard::PulsePosMinus(void);
 *    10. void MeEncoderOnBoard::SetCurrentSpeed(double speed);
 *    11. double MeEncoderOnBoard::GetCurrentSpeed(void);
 *    12. int MeEncoderOnBoard::GetPwm(void);
 *    13. void MeEncoderOnBoard::setMotorPwm(int pwm);
 *    14. void MeEncoderOnBoard::Update_speed(void);
 *    15. void MeEncoderOnBoard::update(void);
 *    16. long MeEncoderOnBoard::distanceToGo(void);
 *    17. void MeEncoderOnBoard::runSpeed(double speed);
 *    18. void MeEncoderOnBoard::setSpeed(double speed);
 *    19. void MeEncoderOnBoard::move(long distance,cb callback,int extId);
 *    20. void MeEncoderOnBoard::moveTo(long position,cb callback,int extId);
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * forfish         2015/11/10     1.0.0            Add description
 * Mark Yan        2016/04/07     1.0.1            fix motor reset issue.
 * Mark Yan        2016/05/17     1.0.2            add some comments.
 * </pre>
 *
 * @example MeEncoderOnBoardMoveTo.ino
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
 *   port - megapi dc port from PORT_1 to PORT_12
 */
MeEncoderOnBoard::MeEncoderOnBoard(uint8_t slot)
{
  _Slot = slot;
  _Port_A = encoder_Port[slot].port_A;
  _Port_B = encoder_Port[slot].port_B;
  _Port_PWM = encoder_Port[slot].port_PWM;
  _Port_H1 = encoder_Port[slot].port_H1;
  _Port_H2 = encoder_Port[slot].port_H2;

  pinMode(_Port_A, INPUT_PULLUP);
  pinMode(_Port_B, INPUT_PULLUP);
  pinMode(_Port_H1, INPUT_PULLUP);
  pinMode(_Port_H2, INPUT_PULLUP);
  
  encode_structure.pulse_pos = 0;
  
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
  MeEncoderOnBoard::setMotorPwm(0);
  MeEncoderOnBoard::SetPulsePos(0);
  _Measurement_speed_time = millis();
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
  _Slot = slot;
  _last_speed = 500;
  _Port_A = encoder_Port[slot].port_A;
  _Port_B = encoder_Port[slot].port_B;
  _Port_PWM = encoder_Port[slot].port_PWM;
  _Port_H1 = encoder_Port[slot].port_H1;
  _Port_H2 = encoder_Port[slot].port_H2;

  pinMode(_Port_A, INPUT_PULLUP);
  pinMode(_Port_B, INPUT_PULLUP);
  pinMode(_Port_H1, INPUT_PULLUP);
  pinMode(_Port_H2, INPUT_PULLUP);
  
  encode_structure.pulse_pos = 0;
  
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
  MeEncoderOnBoard::setMotorPwm(0);
  MeEncoderOnBoard::SetPulsePos(0);
  _Measurement_speed_time = millis();
}

/**
 * \par Function
 *   GetSlotNum
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
uint8_t MeEncoderOnBoard::GetSlotNum(void)
{
  return _Slot;
}

/**
 * \par Function
 *   GetIntNum
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
uint8_t MeEncoderOnBoard::GetIntNum(void)
{
  return _IntNum;
}

/**
 * \par Function
 *   GetPortA
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
uint8_t MeEncoderOnBoard::GetPortA(void)
{
  return _Port_A;
}

/**
 * \par Function
 *   GetPortB
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
uint8_t MeEncoderOnBoard::GetPortB(void)
{
  return _Port_B;
}

/**
 * \par Function
 *   GetPulsePos
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
long MeEncoderOnBoard::GetPulsePos(void)
{
  return encode_structure.pulse_pos;
}

/**
 * \par Function
 *   SetPulsePos
 * \par Description
 *   This function used to Set the current pos value(pulse counter). Generally used for\n
 *   reset the distance calculation.
 * \param[in]
 *   pulse_pos - the value of pos value(pulse counter)
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
void MeEncoderOnBoard::SetPulsePos(long pulse_pos)
{
  encode_structure.pulse_pos = pulse_pos;
}

/**
 * \par Function
 *   PulsePosPlus
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
void MeEncoderOnBoard::PulsePosPlus(void)
{
  encode_structure.pulse_pos++;
}

/**
 * \par Function
 *   PulsePosPlus
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
void MeEncoderOnBoard::PulsePosMinus(void)
{
  encode_structure.pulse_pos--;
}

/**
 * \par Function
 *   SetCurrentSpeed
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
void MeEncoderOnBoard::SetCurrentSpeed(double speed)
{
  encode_structure.currentSpeed = speed;
}

/**
 * \par Function
 *   GetCurrentSpeed
 * \par Description
 *   This function used to get the current speed(The unit is rpm).
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   double - the speed value(The unit is rpm).
 * \par Others
 *   None
 */
double MeEncoderOnBoard::GetCurrentSpeed(void)
{
  return encode_structure.currentSpeed;
}

/**
 * \par Function
 *   GetPwm
 * \par Description
 *   This function used to get the current pwm setting.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   int - the pwm setting
 * \par Others
 *   None
 */
int MeEncoderOnBoard::GetPwm(void)
{
  return encode_structure.pwm;
}

/**
 * \par Function
 *   setMotorPwm
 * \par Description
 *   This function used to set current pwm setting.
 * \param[in]
 *   pwm - the pwm setting
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
void MeEncoderOnBoard::setMotorPwm(int pwm)
{
  pwm = constrain(pwm,-255,255);
  if(_last_speed != pwm)
  {
    _last_speed = pwm;
  }
  else
  {
    return;
  }
  encode_structure.pwm = pwm;

  if(pwm < 0)
  {
    digitalWrite(MeEncoderOnBoard::_Port_H1, LOW);
    digitalWrite(MeEncoderOnBoard::_Port_H2, HIGH);
    analogWrite(MeEncoderOnBoard::_Port_PWM, abs(pwm));
  }
  else
  {
    digitalWrite(MeEncoderOnBoard::_Port_H1, HIGH);
    digitalWrite(MeEncoderOnBoard::_Port_H2, LOW);
    analogWrite(MeEncoderOnBoard::_Port_PWM, abs(pwm));
  }
}

/**
 * \par Function
 *   Update_speed
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
void MeEncoderOnBoard::Update_speed(void)
{
  if((millis() - _Measurement_speed_time) > 20)
  {
    uint16_t dt = millis() - _Measurement_speed_time;
    long cur_pos = MeEncoderOnBoard::GetPulsePos();
    MeEncoderOnBoard::SetCurrentSpeed(((cur_pos - _Last_pulse_pos)/357.3)*(1000/dt)*60);
    _Last_pulse_pos = cur_pos;
    _Measurement_speed_time = millis();
  }
}

/**
 * \par Function
 *   update
 * \par Description
 *   This function called in main loop if you need run to the set position.\n
 *   It should be used with move and moveTo function;
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
void MeEncoderOnBoard::update(void)
{
  if(!_moving)
  {
    return;
  }
  if((millis() - _Measurement_speed_time) > 20)
  {
    uint16_t dt = millis() - _Measurement_speed_time;
    long cur_pos = MeEncoderOnBoard::GetPulsePos();
    MeEncoderOnBoard::SetCurrentSpeed(((cur_pos - _Last_pulse_pos)/357.3)*(1000/dt)*60);
    _Last_pulse_pos = cur_pos;
    _Measurement_speed_time = millis();
    if(_mode == 0)
    {
      setMotorPwm(GetPwm()+(_targetSpeed-GetCurrentSpeed())/8.0);
    }
    else if(_mode == 1)
    {
      long dist = distanceToGo();
      int dir = dist>0?1:-1;
      if(abs(dist)>100)
      {
        setMotorPwm(GetPwm()+dir*(abs(_targetSpeed)-abs(GetCurrentSpeed()))/8.0);
      }
      else
      {
        setMotorPwm(dist);	
        if((abs(dist) < 10) && _moving)
        {
          _moving = false;
          _callback(_Slot,_extId);
        }
      }
    }
  }
}

/**
 * \par Function
 *    distanceToGo
 * \par Description
 *    The distance that encoder should go.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    long - Return the length that encoder need run.
 * \par Others
 *    None
 */
long MeEncoderOnBoard::distanceToGo(void)
{
  return _targetPosition - _Last_pulse_pos;
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
void MeEncoderOnBoard::runSpeed(double speed)
{
  _mode = 0;
  _moving = true;
  _targetSpeed = speed;
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
void MeEncoderOnBoard::setSpeed(double speed)
{
  _targetSpeed = speed;
}

/**
 * \par Function
 *    move
 * \par Description
 *    encoder motor moves to the relative positions.
 * \param[in]
 *    relative - The relative length to Stepper's movement.
 * \param[in]
 *    absolute - callback function when the target position has been reached.
 * \param[in]
 *    extId - It is used to indicate the ID of motor.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderOnBoard::move(long distance,cb callback,int extId)
{
  if(_targetSpeed == 0)
  {
    _targetSpeed = 100;
  }
  _extId = extId;
  _moving = true;
  _mode = 1;
  _targetPosition += distance;
  _callback = callback;
}

/**
 * \par Function
 *    moveTo
 * \par Description
 *    encoder motor moves to the absolute position.
 * \param[in]
 *    absolute - The absolute length to Stepper's movement.
 * \param[in]
 *    absolute - callback function when the target position has been reached.
 * \param[in]
 *    extId - It is used to indicate the ID of motor.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderOnBoard::moveTo(long position,cb callback,int extId)
{
  if(_targetSpeed == 0)
  {
    _targetSpeed = 100;
  }
  _extId = extId;
  _moving = true;
  _mode = 1;
  _targetPosition = position;
  _callback = callback;
}