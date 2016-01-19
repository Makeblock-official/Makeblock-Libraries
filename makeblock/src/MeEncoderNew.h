/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MeEncoderNew
 * \brief   Driver for Me Encoder New module.
 * @file    MeEncoderNew.h
 * @author  MakeBlock
 * @version V1.0.1
 * @date    2015/11/10
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
 *    1. void MeEncoderNew::begin();
 *    2. void MeEncoderNew::reset();
 *    3. void MeEncoderNew::move(long angle, int speed);
 *    4. void MeEncoderNew::moveTo(long angle, int speed);
 *    5. void MeEncoderNew::runTurns(int turns, int speed);
 *    6. void MeEncoderNew::runSpeed(int speed);
 *    7. void MeEncoderNew::runSpeedAndTime(int speed, float time);
 *    8. int MeEncoderNew::getCurrentSpeed();
 *    9. long MeEncoderNew::getCurrentPosition();
 *    10. void MeEncoderNew::moveTo(long angle);
 *    11. void MeEncoderNew::movetoSpeed(long angle,int speed);
 *    12. void MeEncoderNew::setHold(uint8_t hold);
 *    13. void MeEncoderNew::setPID(float p,float i,float d,float s);
 *    14. void MeEncoderNew::setMaxPower(int8_t maxPower);
 *    15. int8_t MeEncoderNew::getPower();
 *    16. void MeEncoderNew::getPID(float * p,float * i,float * d,float * s);
 *    17. float MeEncoderNew::getRatio();
 *    18. void MeEncoderNew::setRatio(float r);
 *    19. int MeEncoderNew::getPulse();
 *    20. void MeEncoderNew::setPulse(int p);
 *    21. void MeEncoderNew::setDevid(int devid);
 *    22. void MeEncoderNew::setMode(uint8_t mode);
 *    23. void MeEncoderNew::setPWM(int pwm);
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan        2015/11/04     1.0.0            build the new
 * forfish         2015/11/10     1.0.1            Add description
 * </pre>
 *
 * @example EncoderMotorTestRunSpeedAndTime.ino
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
 *   port - RJ25 port from PORT_1 to M2
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
  MeEncoderNew();

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
  void begin();

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
  void reset();

/**
 * \par Function
 *    move
 * \par Description
 *    Motor move.
 * \param[in]
 *    angle - The angle move of Motor New.
 * \param[in]
 *    speed - The speed move of Motor New.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void move(long angle, int speed);

/**
 * \par Function
 *    moveTo
 * \par Description
 *    Motor New move to the aim.
 * \param[in]
 *    angle - The angle move of Motor New.
 * \param[in]
 *    speed - The speed move of Motor New.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void moveTo(long angle, int speed);

/**
 * \par Function
 *    runTurns
 * \par Description
 *    Motor move turns.
 * \param[in]
 *    turns - The turns move of Motor.
 * \param[in]
 *    speed - The speed move of Motor.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void runTurns(int turns, int speed);

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
  void runSpeed(int speed);

/**
 * \par Function
 *    runSpeedAndTime
 * \par Description
 *    The speed and time of Motor's movement.
 * \param[in]
 *    speed - The speed move of Motor.
 * \param[in]
 *    time - The time move of Motor.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void runSpeedAndTime(int speed, float time);

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
 *    None
 * \par Others
 *    None
 */
  int getCurrentSpeed();

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
 *    None
 * \par Others
 *    None
 */
  long getCurrentPosition();

/**
 * \par Function
 *    moveTo
 * \par Description
 *    Motor New move to the aim.
 * \param[in]
 *    angle - The angle move of Motor New.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void moveTo(long angle);

/**
 * \par Function
 *    movetoSpeed
 * \par Description
 *    Motor move to the set speed.
 * \param[in]
 *    angle - The angle move of Motor.
 * \param[in]
 *    speed - The speed move of Motor.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void movetoSpeed(long angle,int speed);

/**
 * \par Function
 *    setHold
 * \par Description
 *    Hold the state of Motor.
 * \param[in]
 *    hold - The time of Motor hold.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void setHold(uint8_t hold);

/**
 * \par Function
 *    setPID
 * \par Description
 *    Set PID for Motor.
 * \param[in]
 *    p - P means Proportion.
 * \param[in]
 *    i - I means Integration.
 * \param[in]
 *    d - D means Differentiation.
 * \param[in]
 *    s - S means slot of Motor.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void setPID(float p,float i,float d,float s);

/**
 * \par Function
 *    setMaxPower
 * \par Description
 *    Set the max power to Motor.
 * \param[in]
 *    maxPower - The size of power.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void setMaxPower(int8_t maxPower);

/**
 * \par Function
 *    getPower
 * \par Description
 *    The current power of Motor's movement.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    Return the size of power.
 * \par Others
 *    None
 */
  int8_t getPower();

/**
 * \par Function
 *    getPID
 * \par Description
 *    Get PID from Motor.
 * \param[in]
 *    p - P means Proportion.
 * \param[in]
 *    i - I means Integration.
 * \param[in]
 *    d - D means Differentiation.
 * \param[in]
 *    s - S means slot of Motor.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void getPID(float * p,float * i,float * d,float * s);

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
  float getRatio();

/**
 * \par Function
 *    setRatio
 * \par Description
 *    Set the ratio to Motor.
 * \param[in]
 *    None
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
  int getPulse();

/**
 * \par Function
 *    setPulse
 * \par Description
 *    Set the pulse to Motor.
 * \param[in]
 *    None
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
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void setDevid(int devid);

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
  void sendCmd();

  uint8_t _slot; 
  uint8_t address;
  unsigned long _lastTime;
  char cmdBuf[18];
};

#endif



