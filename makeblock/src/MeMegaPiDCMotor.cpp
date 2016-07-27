/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class MeMegaPiDCMotor
 * \brief   Driver for Me Megapi DC motor device.
 * @file    MeMegaPiDCMotor.cpp
 * @author  MakeBlock
 * @version V1.0.1
 * @date    2016/04/07
 * @brief   Driver for Me Megapi DC motor device.
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
 * This file is a drive for Me Megapi DC mtor device.
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
 * </pre>o
 *
 * @example MeMegaPiDCMotorTest.ino
 */
#include "MeMegaPiDCMotor.h"

/**
 * Alternate Constructor which can call your own function to map the DC motor to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
MeMegaPiDCMotor::MeMegaPiDCMotor(void)
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
 *   port - megapi dc port from PORT_1 to PORT_12
 */
MeMegaPiDCMotor::MeMegaPiDCMotor(uint8_t port)
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
  _dc_dir_h1 = megapi_dc_Port[port].dc_dir_h1;
  _dc_dir_h2 = megapi_dc_Port[port].dc_dir_h2;
  _dc_pwm_pin = megapi_dc_Port[port].pwm_pin;
  
  pinMode(_dc_dir_h1, OUTPUT);
  pinMode(_dc_dir_h2, OUTPUT);
}

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
MeMegaPiDCMotor::MeMegaPiDCMotor(uint8_t dc_dir_h1,uint8_t dc_dir_h2,uint8_t pwm_pin)
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
  _dc_dir_h1 = dc_dir_h1;
  _dc_dir_h2 = dc_dir_h2;
  _dc_pwm_pin = pwm_pin;
  
  pinMode(_dc_dir_h1, OUTPUT);
  pinMode(_dc_dir_h2, OUTPUT);
}

/**
 * \par Function
 *   reset
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
void MeMegaPiDCMotor::reset(uint8_t port)
{
  _dc_dir_h1 = megapi_dc_Port[port].dc_dir_h1;
  _dc_dir_h2 = megapi_dc_Port[port].dc_dir_h2;
  _dc_pwm_pin = megapi_dc_Port[port].pwm_pin;
  
  pinMode(_dc_dir_h1, OUTPUT);
  pinMode(_dc_dir_h2, OUTPUT);
  last_speed = 500;
}

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
void MeMegaPiDCMotor::setpin(uint8_t dc_dir_h1,uint8_t dc_dir_h2,uint8_t pwm_pin)
{
  _dc_dir_h1 = dc_dir_h1;
  _dc_dir_h2 = dc_dir_h2;
  _dc_pwm_pin = pwm_pin;
  
  pinMode(_dc_dir_h1, OUTPUT);
  pinMode(_dc_dir_h2, OUTPUT);
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
void MeMegaPiDCMotor::run(int16_t speed)
{
  speed = speed > 255 ? 255 : speed;
  speed = speed < -255 ? -255 : speed;

  if(last_speed != speed)
  {
    last_speed = speed;
  }
  else
  {
    return;
  }
 
  if(speed > 0)
  {
    digitalWrite(_dc_dir_h2, LOW);
    digitalWrite(_dc_dir_h1, HIGH);
    analogWrite(_dc_pwm_pin,speed);
  }
  else if(speed < 0)
  {
    digitalWrite(_dc_dir_h1, LOW);
    digitalWrite(_dc_dir_h2, HIGH);
    analogWrite(_dc_pwm_pin,-speed);
  }
  else
  {
    digitalWrite(_dc_dir_h2, LOW);
    digitalWrite(_dc_dir_h1, LOW);
    analogWrite(_dc_pwm_pin,0);
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
void MeMegaPiDCMotor::stop(void)
{
  run(0);
}

