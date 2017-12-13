/**
 * \par Copyright (C), 2012-2017, MakeBlock
 * \class MeMegaPiProESCMotor
 * \brief   Driver for Me Megapi Pro ESC motor device.
 * @file    MeMegaPiProESCMotor.h
 * @author  MakeBlock
 * @version V1.0.1
 * @date    2017/05/12
 * @brief   Header for MeMegaPiProESCMotor.cpp module
 *
 * \par Copyright
 * This software is Copyright (C), 2012-2017, MakeBlock. Use is subject to license \n
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
 * This file is a drive for Me Megapi Pro ESC motor device.
 *
 * \par Method List:
 *
 *    1. void MeMegaPiProESCMotor::setpin(uint8_t pwm_pin)
 *    2. void MeMegaPiProESCMotor::run(int16_t speed)
 *    3. void MeMegaPiProESCMotor::stop(void)
 *    4. void MeMegaPiProESCMotor::reset(uint8_t port)
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Lan weiting         2017/05/09       1.0.0            Rebuild the new.
 * Zzipeng             2017/05/12       1.0.1            add some details.
 * </pre>
 *
 */
#ifndef MeMegaPiProESCMotor_H
#define MeMegaPiProESCMotor_H

#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>
#include "MeConfig.h"
#ifdef ME_PORT_DEFINED
#include "MePort.h"
#endif // ME_PORT_DEFINED

typedef struct
{
  uint8_t pwm_pin; //PWM
} megapipro_esc_type;

extern megapipro_esc_type megapi_esc_Port[5];  // megapipro_esc_Port[0] is nonsense

#define THROTTLE_OFFSET 137

#define THROTTLE_MIN 137//0.55ms~0.95ms throttle scale,970hz,1.024ms.
                        //255 * (0.55/1.024) = 137
#define THROTTLE_MAX 237//255 * (0.95/1.024) = 237
/**
 * Class: MeMegaPiProESCMotor
 * \par Description
 * Declaration of Class MeMegaPiProESCMotor
 */
class MeMegaPiProESCMotor
{
public:
#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the ESC motor to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
  MeMegaPiProESCMotor(void);

/**
 * Alternate Constructor which can call your own function to map the ESC motor to arduino port
 * \param[in]
 *   port - megapi dc port from PORT_1 to PORT_12
 */
  MeMegaPiProESCMotor(uint8_t port);
#else
/**
 * Alternate Constructor which can call your own function to map the ESC motor to arduino port,
 * it will assigned the output pin.
 * \param[in]
 *   pwm_pin - arduino port for pwm input(should analog pin)
 */
  MeMegaPiProESCMotorPWMpin(uint8_t pwm_pin);
#endif
/**
 * \par Function
 *   setpin
 * \par Description
 *   Reset the ESC motor available PIN by megapipro port.
 * \param[in]
 *   port - megapipro esc port from PORT_1 to PORT_12
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
 *   setpin
 * \par Description
 *   Reset the ESC motor available PIN by its arduino port.
 * \param[in]
 *   pwm_pin - arduino port for pwm input(should analog pin)
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void setpin(uint8_t pwm_pin);
/**
 * \par Function
 *   init
 * \par Description
 *   init ESC motor
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void init(void);
/**
 * \par Function
 *   run
 * \par Description
 *   Control the motor forward or reverse
 * \param[in]
 *   speed - Speed value from 0 to 100
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
  volatile uint8_t _esc_pwm_pin;
  volatile int16_t last_speed;
};

#endif //MeMegaPiProESCMotor_H

