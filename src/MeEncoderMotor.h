/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MeEncoderMotor
 * \brief   Driver for Me Encoder Motor module.
 * @file    MeEncoderMotor.h
 * @author  MakeBlock
 * @version V1.0.1
 * @date    2015/11/09
 * @brief   Header for MeEncoderMotor.cpp module
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
 * This file is a drive for Me EncoderMotor device, The Me EncoderMotor inherited the 
 * MeSerial class from SoftwareSerial.
 *
 * \par Method List:
 *
 *    1. void MeEncoderMotor::begin();
 *    2. boolean MeEncoderMotor::reset();
 *    3. boolean MeEncoderMotor::move(float angle, float speed);
 *    4. boolean MeEncoderMotor::moveTo(float angle, float speed);
 *    5. boolean MeEncoderMotor::runTurns(float turns, float speed);
 *    6. boolean MeEncoderMotor::runSpeed(float speed);
 *    7. boolean MeEncoderMotor::runSpeedAndTime(float speed, float time);
 *    8. float MeEncoderMotor::getCurrentSpeed();
 *    9. float MeEncoderMotor::getCurrentPosition();
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan        2015/11/03     1.0.0            Fix minor errors on format
 * forfish         2015/11/09     1.0.1            Add description
 * </pre>
 *
 * @example EncoderMotorTestRunSpeedAndTime.ino
 */

#ifndef MeEncoderMotor_h
#define MeEncoderMotor_h

#include <stdbool.h>
#include "MeConfig.h"
#include "MePort.h"

///@brief Class for Encoder Motor Driver
class MeEncoderMotor: public MePort{
public:
/**
 * Alternate Constructor which can call your own function to map the Encoder Motor to arduino port,
 * you can set any slot for the Encoder Motor device. 
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 * \param[in]
 *   slot - SLOT1 or SLOT2
 */
  MeEncoderMotor(uint8_t addr,uint8_t slot);

/**
 * Alternate Constructor which can call your own function to map the Encoder Motor to arduino port,
 * you can set any slot for the Encoder Motor device.
 * \param[in]
 *   slot - SLOT1 or SLOT2
 */
  MeEncoderMotor(uint8_t slot);

/**
 * Alternate Constructor which can call your own function to map the Encoder Motor to arduino port,
 * you should initialized slot and slaveAddress here for the Encoder Motor device.
 * \param[in]
 *   None
 */
  MeEncoderMotor();

/**
 * \par Function
 *    begin
 * \par Description
 *    Initialize Encoder Motor.
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
 *   reset
 * \par Description
 *   Reset the available data for Encoder Motor.
 * \param[in]
 *   None
 * \par Output
 *   None 
 * \return
 *   None
 * \par Others
 *   None
 */
  boolean reset();

/**
 * \par Function
 *    move
 * \par Description
 *    Motor move.
 * \param[in]
 *    angle - The angle move of Motor.
 * \param[in]
 *    speed - The speed move of Motor.
 * \par Output
 *    None
 * \par Return
 *    Return the result of Motor's movement.
 * \par Others
 *    None
 */
  boolean move(float angle, float speed);

/**
 * \par Function
 *    moveTo
 * \par Description
 *    Motor move to the aim.
 * \param[in]
 *    angle - The angle move of Motor.
 * \param[in]
 *    speed - The speed move of Motor.
 * \par Output
 *    None
 * \par Return
 *    Return the result of Motor's movement to the aim.
 * \par Others
 *    None
 */
  boolean moveTo(float angle, float speed);

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
 *    Return the result of Motor's movement.
 * \par Others
 *    None
 */
  boolean runTurns(float turns, float speed);

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
 *    Return 0.
 * \par Others
 *    None
 */
  boolean runSpeed(float speed);

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
 *    Return the result of Motor's movement.
 * \par Others
 *    None
 */
  boolean runSpeedAndTime(float speed, float time);

/**
 * \par Function
 *    getCurrentSpeed
 * \par Description
 *    The current speed of Motor's movement.
 * \param[in]
      None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  float getCurrentSpeed();

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
  float getCurrentPosition();

private:
/**
 * \par Function
 *    request
 * \par Description
 *    The request of Motor.
 * \param[in]
 *    writeData - Write data to Encoder Motor.
  * \param[in]
 *    readData - Read data from Encoder Motor.
  * \param[in]
 *    wlen - The data's length that write to Encoder Motor.
  * \param[in]
 *    rlen - The data's length that read from Encoder Motor.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void request(byte *writeData, byte *readData, int wlen, int rlen);

  uint8_t _slot;    
  uint8_t _slaveAddress;
  unsigned long _lastTime;
};
#endif
