/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MeEncoderNew
 * \brief   Driver for Me Encoder New module.
 * @file    MeEncoderNew.h
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2016/03/18
 * @brief   Header for MeEncoderNew.cpp module
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
 * This file is a drive for Me EncoderNew device, The Me EncoderNew inherited the 
 * MeSerial class from SoftwareSerial.
 *
 * \par Method List:
 *
 *    1. void MeEncoderNew::begin(void);
 *    2. void MeEncoderNew::move(long angle, float speed, float lock_state);
 *    3. void MeEncoderNew::moveTo(long angle, float speed,float lock_state);
 *    4. void MeEncoderNew::runSpeed(int speed);
 *    5. void MeEncoderNew::runTurns(long turns, float speed,float lock_state);
 *    6. void MeEncoderNew::reset(void);
 *    7. void MeEncoderNew::setSpeedPID(float p,float i,float d);
 *    8. void MeEncoderNew::setPosPID(float p,float i,float d);
 *    9. void MeEncoderNew::setMode(uint8_t mode);
 *    10. void MeEncoderNew::setPWM(int pwm);
 *    11. void MeEncoderNew::setCurrentPosition(long pulse_counter)
 *    12. long MeEncoderNew::getCurrentPosition();
 *    13. void MeEncoderNew::getSpeedPID(float * p,float * i,float * d);
 *    14. void MeEncoderNew::getPosPID(float * p,float * i,float * d);
 *    15. float MeEncoderNew::getCurrentSpeed(void);
 *    16. void MeEncoderNew::sendCmd(void);
 *    17. float MeEncoderNew::getRatio(void);
 *    18. void MeEncoderNew::setRatio(float r);
 *    19. int MeEncoderNew::getPulse(void);
 *    20. void MeEncoderNew::setPulse(int p);
 *    21. void MeEncoderNew::setDevid(int devid);
 *    22. void MeEncoderNew::runSpeedAndTime(float speed, float time, float lock_state);
 *    23. boolean MeEncoderNew::isTarPosReached(void);
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan        2016/03/18     1.0.0            build the new
 * </pre>
 *
 */

#ifndef ME_LEGOENCODER_H
#define ME_LEGOENCODER_H
#include <Arduino.h>
#include <stdbool.h>
#include "MeConfig.h"
#include "MePort.h"

#define LEONARDO_PORT1

#if defined(UNO_PORT1) //10:PB3 11:PB2
#define SDA_PORT PORTB
#define SDA_PIN 3
#define SCL_PORT PORTB
#define SCL_PIN 2
#define I2C_SLOWMODE 1
#elif defined(UNO_PORT2) //3:PD3 9:PB1
#define SDA_PORT PORTD
#define SDA_PIN 3
#define SCL_PORT PORTB
#define SCL_PIN 1
#define I2C_SLOWMODE 1
#elif defined(LEONARDO_PORT1) //11:PB7 8:PB4
#define SDA_PORT PORTB
#define SDA_PIN 7
#define SCL_PORT PORTB
#define SCL_PIN 4
#define I2C_SLOWMODE 1
#elif defined(LEONARDO_PORT2) //13:PC7 12:PD6
#define SDA_PORT PORTC
#define SDA_PIN 7
#define SCL_PORT PORTD
#define SCL_PIN 6
#define I2C_SLOWMODE 1
#else
// change sda scl if you use a different pin mapping
#endif

#define PULSE_PER_C 8 

/**
 * Class: MeEncoderNew
 * \par Description
 * Declaration of Class MeEncoderNew
 */
class MeEncoderNew{
public:
/**
 * Alternate Constructor which can call your own function to map the Encoder Motor New to arduino port,
 * you can set any slot for the Encoder Motor New device. 
 * \param[in]
 *   port - RJ25 port from PORT_1 to PORT_10
 * \param[in]
 *   slot - SLOT1 or SLOT2
 */
  MeEncoderNew(uint8_t addr,uint8_t slot);

/**
 * Alternate Constructor which can call your own function to map the Encoder Motor New to arduino port,
 * you can set any slot for the Encoder Motor New device.
 * \param[in]
 *   slot - SLOT1 or SLOT2
 */
  MeEncoderNew(uint8_t slot);

/**
 * Alternate Constructor which can call your own function to map the Encoder Motor New to arduino port,
 * you should initialized slot and slaveAddress here for the Encoder Motor New device.
 * \param[in]
 *   None
 */
  MeEncoderNew(void);

/**
 * \par Function
 *    begin
 * \par Description
 *    Initialize Encoder Motor New.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void begin(void);

/**
 * \par Function
 *    reset
 * \par Description
 *    Reset the parameter of Motor.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void reset(void);

/**
 * \par Function
 *    move
 * \par Description
 *    Motor move.
 * \param[in]
 *    angle - The angle move of Motor New.
 * \param[in]
 *    speed - The speed move of Motor New.
 * \param[in]
 *    lock_state - The lock state of Motor.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void move(long angle, float speed = 220, float lock_state = 1);

/**
 * \par Function
 *    moveTo
 * \par Description
 *    Motor New move to the aim.
 * \param[in]
 *    angle - The angle move of Motor New.
 * \param[in]
 *    speed - The speed move of Motor New.
 * \param[in]
 *    lock_state - The lock state of Motor.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void moveTo(long angle, float speed = 220, float lock_state = 1);

/**
 * \par Function
 *    runTurns
 * \par Description
 *    Motor move turns.
 * \param[in]
 *    turns - The turns move of Motor.
 * \param[in]
 *    speed - The speed move of Motor.
 * \param[in]
 *    lock_state - The lock state of Motor.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void runTurns(long turns, float speed = 220,float lock_state = 1);

/**
 * \par Function
 *    runSpeed
 * \par Description
 *    The speed of Motor's movement.
 * \param[in]
 *    speed - The speed move of Motor.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void runSpeed(float speed,float lock_state = 1);

/**
 * \par Function
 *    getCurrentSpeed
 * \par Description
 *    The current speed of Motor's movement.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    The speed of encoder motor(The unit is rpm)
 * \par Others
 *    None
 */
  float getCurrentSpeed(void);

/**
 * \par Function
 *    setCurrentPosition
 * \par Description
 *    Set current position of Motor.
 * \param[in]
 *    pulse_counter - The count value of current encoder
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void setCurrentPosition(long pulse_counter);

/**
 * \par Function
 *    getCurrentPosition
 * \par Description
 *    The current position of Motor.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    Motor encoder pulse count value.
 * \par Others
 *    None
 */
  long getCurrentPosition(void);

/**
 * \par Function
 *    setSpeedPID
 * \par Description
 *    Set speed PID for Motor.
 * \param[in]
 *    p - P means Proportion.
 * \param[in]
 *    i - I means Integration.
 * \param[in]
 *    d - D means Differentiation.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void setSpeedPID(float p,float i,float d);

  /**
 * \par Function
 *    setPosPID
 * \par Description
 *    Set pos PID for Motor.
 * \param[in]
 *    p - P means Proportion.
 * \param[in]
 *    i - I means Integration.
 * \param[in]
 *    d - D means Differentiation.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void setPosPID(float p,float i,float d);

/**
 * \par Function
 *    getSpeedPID
 * \par Description
 *    Get Speed PID from Motor.
 * \param[in]
 *    p - P means Proportion.
 * \param[in]
 *    i - I means Integration.
 * \param[in]
 *    d - D means Differentiation.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void getSpeedPID(float * p,float * i,float * d);

/**
 * \par Function
 *    getPosPID
 * \par Description
 *    Get PID from Motor.
 * \param[in]
 *    p - P means Proportion.
 * \param[in]
 *    i - I means Integration.
 * \param[in]
 *    d - D means Differentiation.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void getPosPID(float * p,float * i,float * d);

/**
 * \par Function
 *    getRatio
 * \par Description
 *    Get the ratio of Motor.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    Return the ratio of Motor.
 * \par Others
 *    None
 */
  float getRatio(void);

/**
 * \par Function
 *    setRatio
 * \par Description
 *    Set the ratio to Motor.
 * \param[in]
 *    ratio - the ratio of Motor
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void setRatio(float r);

/**
 * \par Function
 *    getPulse
 * \par Description
 *    Get the pulse of Motor.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    Return the pulse of Motor.
 * \par Others
 *    None
 */
  int getPulse(void);

/**
 * \par Function
 *    setPulse
 * \par Description
 *    Set the pulse to Motor.
 * \param[in]
 *    pulse - the line number of Motor
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void setPulse(int p);

/**
 * \par Function
 *    setDevid
 * \par Description
 *    Set the devid to Motor.
 * \param[in]
 *    devid - the I2C adress
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void setDevid(uint8_t devid);

/**
 * \par Function
 *    setMode
 * \par Description
 *    Set the work mode to Motor.
 * \param[in]
 *    mode - The work mode of Motor.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void setMode(uint8_t mode);

/**
 * \par Function
 *    setPWM
 * \par Description
 *    Set the PWM to Motor.
 * \param[in]
 *    pwm - Pulse-Width Modulation.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void setPWM(int pwm);

/**
 * \par Function
 *    runSpeedAndTime
 * \par Description
 *    The speed and time of Motor's movement.
 * \param[in]
 *    speed - The speed move of Motor.
 * \param[in]
 *    time - The time move of Motor.
 * \param[in]
 *    lock_state - The lock state of Motor.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void runSpeedAndTime(float speed, float time, float lock_state = 1);
  
/**
 * \par Function
 *    isTarPosReached
 * \par Description
 *    Check whether the target position has been reached
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    true - The target position reaches
 *    false - Does not reach the target position
 * \par Others
 *    None
 */
  boolean isTarPosReached(void);

private:
/**
 * \par Function
 *    sendCmd
 * \par Description
 *    Send command to Motor.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void sendCmd(void);

  uint8_t _slot; 
  uint8_t address;
  unsigned long _lastTime;
  char cmdBuf[18];
};

#endif



