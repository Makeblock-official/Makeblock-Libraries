/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class MeDCMotor
 * \brief   Driver for Me DC motor device.
 * @file    MeDCMotor.cpp
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/09
 * @brief   Driver for Me DC motor device.
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
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/09/09     1.0.0            Rebuild the old lib.
 * </pre>
 *
 * @example DCMotorDriverTest.ino
 */
#include "MeDCMotor.h"

#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the DC motor to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
MeDCMotor::MeDCMotor(void) : MePort(0)
{

}

/**
 * Alternate Constructor which can call your own function to map the DC motor to arduino port
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
MeDCMotor::MeDCMotor(uint8_t port) : MePort(port)
{

}
#else // ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the DC motor to arduino port,
 * it will assigned the output pin.
 * \param[in]
 *   dir_pin - arduino port for direction pin(should analog pin)
 * \param[in]
 *   pwm_pin - arduino port for pwm input(should analog pin)
 */
MeDCMotor::MeDCMotor(uint8_t dir_pin,uint8_t pwm_pin)
{
  dc_dir_pin = dir_pin;
  dc_pwm_pin = pwm_pin;
  
  pinMode(dc_dir_pin, OUTPUT);
}
#endif /* ME_PORT_DEFINED */

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
void MeDCMotor::setpin(uint8_t dir_pin,uint8_t pwm_pin)
{
  dc_dir_pin = dir_pin;
  dc_pwm_pin = pwm_pin;
  pinMode(dc_dir_pin, OUTPUT);
#ifdef ME_PORT_DEFINED
  s1 = pwm_pin;
  s2 = dir_pin;
#endif // ME_PORT_DEFINED
}

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
void MeDCMotor::run(int16_t speed)
{
  speed	= speed > 255 ? 255 : speed;
  speed	= speed < -255 ? -255 : speed;

  if(speed >= 0)
  {
#ifdef ME_PORT_DEFINED
    MePort::dWrite2(HIGH);
    MePort::aWrite1(speed);
#else /* ME_PORT_DEFINED */
    digitalWrite(dc_dir_pin,HIGH);
    analogWrite(dc_pwm_pin,speed);
#endif/* ME_PORT_DEFINED */
  }
  else
  {
#ifdef ME_PORT_DEFINED
    MePort::dWrite2(LOW);
    MePort::aWrite1(-speed);
#else /* ME_PORT_DEFINED */
    digitalWrite(dc_dir_pin,LOW);
    analogWrite(dc_pwm_pin,-speed);
#endif/* ME_PORT_DEFINED */
  }
}

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
void MeDCMotor::stop(void)
{
  MeDCMotor::run(0);
}

