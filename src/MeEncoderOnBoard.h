/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MeEncoderOnBoard
 * \brief   Driver for Encoder module on MeAuriga and MeMegaPi.
 * @file    MeEncoderOnBoard.h
 * @author  MakeBlock
 * @version V1.0.4
 * @date    2018/01/03
 * @brief   Header for MeEncoderOnBoard.cpp module
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
 * Mark Yan         2015/12/15     1.0.0            Build New
 * Mark Yan         2016/04/07     1.0.1            fix motor reset issue.
 * Mark Yan         2016/05/17     1.0.2            add some comments.
 * Mark Yan         2016/06/25     1.0.3            add PID calibration for encoder driver.
 * Mark Yan         2018/01/03     1.0.4            add callback flag.
 * </pre>
 */

#ifndef ME_Encoder_OnBoard_H
#define ME_Encoder_OnBoard_H

#include <Arduino.h>
#include <stdbool.h>
#include <avr/interrupt.h>

#define DIRECT_MODE       0x00
#define PID_MODE          0x01
#define PWM_MODE          0x02

#define MOTION_WITH_POS        0x00
#define MOTION_WITHOUT_POS     0x01

#define PWM_MIN_OFFSET                 25
#define ENCODER_POS_DEADBAND           10
#define DECELERATION_DISTANCE_PITCH    6 

typedef struct
{
  float P, I, D;
  float Setpoint, Output, Integral, differential, last_error;
} PID_internal;

typedef struct
{
  uint8_t mode;
  uint8_t motionState;

  int16_t pulseEncoder;
  int16_t currentPwm;
  int16_t targetPwm;
  int16_t previousPwm;

  float currentSpeed;
  float targetSpeed;
  float previousSpeed;
  float ratio;

  long currentPos;
  long targetPos;
  long previousPos;
  long pulsePos;
 
  PID_internal  PID_speed;
  PID_internal  PID_pos;
}Me_Encoder_type;

typedef struct
{
  uint8_t port_A;   //INT
  uint8_t port_B;   //DIR
  uint8_t port_PWM; //PWM
  uint8_t port_H1;
  uint8_t port_H2;
} Encoder_port_type;

extern Encoder_port_type encoder_Port[6];  // encoder_Port[0] is nonsense
typedef void (*cb)(int16_t,int16_t); 

/**
 * Class: MeEncoderOnBoard
 * \par Description
 * Declaration of Class MeEncoderOnBoard
 */
class MeEncoderOnBoard 
{
public:
/**
 * Alternate Constructor which can call your own function to map the Encoder motor to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
  MeEncoderOnBoard();

/**
 * Alternate Constructor which can call your own function to map the Encoder motor to arduino port
 * \param[in]
 *   slot - megapi slot from SLOT1 to SLOT4(Auriga SLOT1 and SLOT2).
 */
  MeEncoderOnBoard(int slot);

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
  void reset(uint8_t slot);

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
  uint8_t getSlotNum(void) const;

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
  uint8_t getIntNum(void) const;
  
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
  uint8_t getPortA(void) const;

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
  uint8_t getPortB(void) const;

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
  long getPulsePos(void) const;

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
  void setPulsePos(long pulse_pos);

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
  void pulsePosPlus(void);
  
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
  void pulsePosMinus(void);

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
  void setCurrentSpeed(float speed);

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
  float getCurrentSpeed(void) const;

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
 *   None
 * \par Others
 *   None
 */
  int16_t getCurPwm(void) const;

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
  void setTarPWM(int16_t pwm_value);

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
  void setMotorPwm(int16_t pwm);
  
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
  void updateSpeed(void);

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
  void updateCurPos(void);

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
  long getCurPos(void) const;

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
  void runSpeed(float speed);

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
  void setSpeed(float speed);

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
  void move(long position,float speed = 100,int16_t extId=0,cb callback=NULL);

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
  void moveTo(long position,float speed = 100,int16_t extId=0,cb callback=NULL);
  
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
  long distanceToGo() const;

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
  void setSpeedPid(float p,float i,float d);

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
  void setPosPid(float p,float i,float d);

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
  void setPulse(int16_t pulseValue);

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
  void setRatio(int16_t RatioValue);

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
  void setMotionMode(int16_t motionMode);

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
  int16_t pidPositionToPwm(void);

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
  int16_t speedWithoutPos(void);

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
  void encoderMove(void);

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
  void pwmMove(void);

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
  boolean isTarPosReached(void) const;

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
  void loop(void);

private:
   volatile Me_Encoder_type encode_structure;
   boolean _Lock_flag;
   boolean _Dir_lock_flag;
   boolean _Callback_flag;
   uint8_t _extId;
   uint8_t _Port_A;
   uint8_t _Port_B;
   uint8_t _Port_PWM;
   uint8_t _Port_H1;
   uint8_t _Port_H2;
   uint8_t _IntNum;
   uint8_t _Slot;
   int16_t _Encoder_output;
   long _Measurement_speed_time;
   long _Encoder_move_time;
   bool _enabled;
   cb _callback;
};
#endif



