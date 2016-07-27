/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class MeMegaPiDCMotor
 * \brief   Driver for Me Megapi DC motor device.
 * @file    MeMegaPiDCMotor.h
 * @author  MakeBlock
 * @version V1.0.1
 * @date    2016/04/07
 * @brief   Header for MeMegaPiDCMotor.cpp module
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
 * This file is a drive for Me Megapi DC motor device.
 *
 * \par Method List:
 *
 *    1. void MeMegaPiDCMotor::setpin(uint8_t dc_dir_h1,uint8_t dc_dir_h2,uint8_t pwm_pin)
 *    2. void MeMegaPiDCMotor::run(int16_t speed)
 *    3. void MeMegaPiDCMotor::stop(void)
 *    4. void MeMegaPiDCMotor::reset(uint8_t port)
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2016/02/20     1.0.0            Rebuild the new.
 * Mark Yan         2016/04/07     1.0.1            fix motor reset issue.
 * </pre>
 *
 */
#ifndef MeMegaPiDCMotor_H
#define MeMegaPiDCMotor_H

#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>
#include "MeConfig.h"

typedef struct
{
  uint8_t dc_dir_h1;
  uint8_t dc_dir_h2;
  uint8_t pwm_pin; //PWM
} megapi_dc_type;

extern megapi_dc_type megapi_dc_Port[14];  // megapi_dc_Port[0] is nonsense

/**
 * Class: MeMegaPiDCMotor
 * \par Description
 * Declaration of Class MeMegaPiDCMotor
 */
class MeMegaPiDCMotor
{
public:
/**
 * Alternate Constructor which can call your own function to map the DC motor to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
  MeMegaPiDCMotor(void);

/**
 * Alternate Constructor which can call your own function to map the DC motor to arduino port
 * \param[in]
 *   port - megapi dc port from PORT_1 to PORT_12
 */
  MeMegaPiDCMotor(uint8_t port);

/**
 * Alternate Constructor which can call your own function to map the DC motor to arduino port,
 * it will assigned the output pin.
 * \param[in]
 *   dc_dir_h1 - arduino port for direction pin1
 * \param[in]
 *   dc_dir_h2 - arduino port for direction pin2
 * \param[in]
 *   pwm_pin - arduino port for pwm input(should analog pin)
 */
  MeMegaPiDCMotor(uint8_t dc_dir_h1,uint8_t dc_dir_h2,uint8_t pwm_pin);

/**
 * \par Function
 *   setpin
 * \par Description
 *   Reset the DC motor available PIN by megapi port.
 * \param[in]
 *   port - megapi dc port from PORT_1 to PORT_12
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
 *   Reset the DC motor available PIN by its arduino port.
 * \param[in]
 *   dc_dir_h1 - arduino port for direction pin1
 * \param[in]
 *   dc_dir_h2 - arduino port for direction pin2
 * \param[in]
 *   pwm_pin - arduino port for pwm input(should analog pin)
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void setpin(uint8_t dc_dir_h1,uint8_t dc_dir_h2,uint8_t pwm_pin);

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
  volatile uint8_t _dc_dir_h1;
  volatile uint8_t _dc_dir_h2;
  volatile uint8_t _dc_pwm_pin;
  volatile int16_t last_speed;
};
#endif //MeMegaPiDCMotor_H

