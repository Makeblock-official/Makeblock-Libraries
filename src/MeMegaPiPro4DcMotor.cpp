/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class MeMegaPiPro4DcMotor
 * \brief   Driver for Me DC motor device.
 * @file    MeMegaPiPro4DcMotor.cpp
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2017/03/14
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
 *    1. void MeMegaPiPro4DcMotor::setpin(uint8_t dir_pin,uint8_t pwm_pin)
 *    2. void MeMegaPiPro4DcMotor::run(int16_t speed)
 *    3. void MeMegaPiPro4DcMotor::stop(void)
 *    4. void MeMegaPiPro4DcMotor::reset(uint8_t port)
 *    5. void MeMegaPiPro4DcMotor::reset(uint8_t port, uint8_t slot)
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Zzipeng          2017/03/14          1.0.0            build the new
 * </pre>
 *
 * @example DCMotorDriverTest.ino
 */
#include "MeMegaPiPro4DcMotor.h"

#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the DC motor to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
MeMegaPiPro4DcMotor::MeMegaPiPro4DcMotor(void) : MePort(0)
{
  //The PWM frequency is 976 Hz
#if defined(__AVR_ATmega32U4__) //MeBaseBoard use ATmega32U4 as MCU
  TCCR1A =  _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(CS10) | _BV(WGM12);

  TCCR3A = _BV(WGM30);
  TCCR3B = _BV(CS31) | _BV(CS30) | _BV(WGM32);

  TCCR4B = _BV(CS42) | _BV(CS41) | _BV(CS40);
  TCCR4D = 0;

#elif defined(__AVR_ATmega328__) // else ATmega328

  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(CS10) | _BV(WGM12);

  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS22);

#elif defined(__AVR_ATmega2560__) //else ATmega2560
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(CS10) | _BV(WGM12);

  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS22);
#endif
}

/**
 * Alternate Constructor which can call your own function to map the DC motor to arduino port
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
MeMegaPiPro4DcMotor::MeMegaPiPro4DcMotor(uint8_t port) : MePort(port)
{
  //The PWM frequency is 976 Hz
#if defined(__AVR_ATmega32U4__) //MeBaseBoard use ATmega32U4 as MCU
  TCCR1A =  _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(CS10) | _BV(WGM12);

  TCCR3A = _BV(WGM30);
  TCCR3B = _BV(CS31) | _BV(CS30) | _BV(WGM32);

  TCCR4B = _BV(CS42) | _BV(CS41) | _BV(CS40);
  TCCR4D = 0;

#elif defined(__AVR_ATmega328__) // else ATmega328

  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(CS10) | _BV(WGM12);

  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS22);

#elif defined(__AVR_ATmega2560__) //else ATmega2560
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(CS10) | _BV(WGM12);

  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS22);
#endif
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
MeMegaPiPro4DcMotor::MeMegaPiPro4DcMotor(uint8_t dir_pin,uint8_t pwm_pin)
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
void MeMegaPiPro4DcMotor::setpin(uint8_t dir_pin,uint8_t pwm_pin)
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
void MeMegaPiPro4DcMotor::reset(uint8_t port)
{
  MePort::reset(port);
  last_speed = 500;
}

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
void MeMegaPiPro4DcMotor::reset(uint8_t port, uint8_t slot)
{
  MePort::reset(port, slot);
  last_speed = 500;
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
void MeMegaPiPro4DcMotor::run(int16_t speed)
{
  speed	= speed > 255 ? 255 : speed;
  speed	= speed < -255 ? -255 : speed;

  if(last_speed != speed)
  {
    last_speed = speed;
  }
  else
  {
    return;
  }

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
void MeMegaPiPro4DcMotor::stop(void)
{
  MeMegaPiPro4DcMotor::run(0);
}

