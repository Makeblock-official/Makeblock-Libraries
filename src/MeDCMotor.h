/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class MeDCMotor
 * \brief   Driver for Me DC motor device.
 * @file    MeDCMotor.h
 * @author  MakeBlock
 * @version V1.0.1
 * @date    2016/04/07
 * @brief   Header for MeDCMotor.cpp module
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
 * This file is a drive for Me DC motor device.
 *
 * \par Method List:
 *
 *    1. void MeDCMotor::setpin(uint8_t dir_pin,uint8_t pwm_pin)
 *    2. void MeDCMotor::run(int16_t speed)
 *    3. void MeDCMotor::stop(void)
 *    4. void MeDCMotor::reset(uint8_t port)
 *    5. void MeDCMotor::reset(uint8_t port, uint8_t slot)
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/09/09     1.0.0            Rebuild the old lib.
 * Mark Yan         2016/04/07     1.0.1            fix motor reset issue.
 * </pre>
 *
 */
#ifndef MeDCMotor_H
#define MeDCMotor_H

#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>
#include "MeConfig.h"

#ifdef ME_PORT_DEFINED
#include "MePort.h"
#endif

/**
 * Class: MeDCMotor
 * \par Description
 * Declaration of Class MeDCMotor
 */
#ifndef ME_PORT_DEFINED
class MeDCMotor
#else // !ME_PORT_DEFINED
class MeDCMotor : public MePort
#endif // !ME_PORT_DEFINED
{
public:
#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the DC motor to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
  MeDCMotor(void);

/**
 * Alternate Constructor which can call your own function to map the DC motor to arduino port
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
  MeDCMotor(uint8_t port);
#else // ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the DC motor to arduino port,
 * it will assigned the output pin.
 * \param[in]
 *   dir_pin - arduino port for direction pin(should analog pin)
 * \param[in]
 *   pwm_pin - arduino port for pwm input(should analog pin)
 */
  MeDCMotor(uint8_t dir_pin,uint8_t pwm_pin);
#endif // ME_PORT_DEFINED
/**
 * \par Function
 *   setpin
 * \par Description
 *   Reset the DC motor available PIN by its arduino port.
 * \param[in]
 *   dir_pin - arduino port for direction pin(should analog pin)
 * \param[in]
 *   pwm_pin - arduino port for pwm input(should analog pin)
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void setpin(uint8_t dir_pin,uint8_t pwm_pin);

/**
 * \par Function
 *   reset
 * \par Description
 *   Reset the DC motor available PIN by its RJ25 port.
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void reset(uint8_t port);

/**
 * \par Function
 *   reset
 * \par Description
 *   Reset the DC motor available PIN by its RJ25 port and slot.
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 * \param[in]
 *   slot - SLOT1 or SLOT2
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void reset(uint8_t port, uint8_t slot);

/**
 * \par Function
 *   run
 * \par Description
 *   Control the motor forward or reverse
 * \param[in]
 *   speed - Speed value from -255 to 255
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void run(int16_t speed);

/**
 * \par Function
 *   stop
 * \par Description
 *   Stop the rotation of the motor
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void stop(void);
private:
  volatile uint8_t dc_dir_pin;
  volatile uint8_t dc_pwm_pin;
  int16_t  last_speed;
};
#endif

