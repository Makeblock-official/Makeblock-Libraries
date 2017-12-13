/**
 * \par Copyright (C), 2012-2017, MakeBlock
 * \class MeMegaPiProESCMotor
 * \brief   Driver for Me Megapipro ESC motor device.
 * @file    MeMegaPiProESCMotor.cpp
 * @author  MakeBlock
 * @version V1.0.2
 * @date    2017/05/22
 * @brief   Driver for Me Megapipro ESC motor device.
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
 * This file is a drive for Me Megapipro ESC mtor device.
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
 * Zzipeng             2017/05/22       1.0.2            limit bldc speed at 50% throttle.
 * </pre>
 *
 *
 * @example MeMegaPiProESCMotor.ino
 */
#include "MeMegaPiProESCMotor.h"

#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the ESC motor to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
MeMegaPiProESCMotor::MeMegaPiProESCMotor(void)
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
  // TCCR2B = _BV(CS22);
  TCCR2B = _BV(CS22)  | _BV(CS20);  //***********************490Hz  ZFY20170509-1501
  

#elif defined(__AVR_ATmega2560__) //else ATmega2560
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(CS10) | _BV(WGM12);

  TCCR2A = _BV(WGM21) | _BV(WGM20);
  // TCCR2B = _BV(CS22);
  TCCR2B = _BV(CS22)  | _BV(CS20);  //***********************490Hz  ZFY20170509-1501
  
#endif
}

/**
 * Alternate Constructor which can call your own function to map the ESC motor to arduino port
 * \param[in]
 *   port - megapipro esc port from PORT_1 to PORT_12
 */
MeMegaPiProESCMotor::MeMegaPiProESCMotor(uint8_t port)
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
  if((megapi_esc_Port[port].pwm_pin == 12))//970HZ
  {
    TCCR1A = _BV(WGM10);
    TCCR1B = _BV(CS11) | _BV(CS10) | _BV(WGM12);
  }
  else if((megapi_esc_Port[port].pwm_pin == 9))//970HZ
  {
    TCCR2A = _BV(WGM21) | _BV(WGM20);
    TCCR2B = _BV(CS22);
  }
  else if((megapi_esc_Port[port].pwm_pin == 5))//970HZ
  {
    TCCR3A = _BV(WGM30);
    TCCR3B = _BV(CS31) | _BV(CS30) | _BV(WGM32);
  }
  else if((megapi_esc_Port[port].pwm_pin == 8))//970HZ
  {
    TCCR4A = _BV(WGM40);
    TCCR4B = _BV(CS41) | _BV(CS40) | _BV(WGM42);
  }
 #endif
  _esc_pwm_pin = megapi_esc_Port[port].pwm_pin;
}
#else
/**
 * Alternate Constructor which can call your own function to map the ESC motor to arduino port,
 * it will assigned the output pin.
 * \param[in]
 *   pwm_pin - arduino port for pwm input(should analog pin)
 */
MeMegaPiProESCMotorPWMpin::MeMegaPiProESCMotorPWMpin(uint8_t pwm_pin)
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
  // TCCR2B = _BV(CS22);
  TCCR2B = _BV(CS22)  | _BV(CS20);  //***********************490Hz  ZFY20170509-1501
  

#elif defined(__AVR_ATmega2560__) //else ATmega2560
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(CS10) | _BV(WGM12);

  TCCR2A = _BV(WGM21) | _BV(WGM20);
  // TCCR2B = _BV(CS22);
  TCCR2B = _BV(CS22)  | _BV(CS20);  //***********************490Hz  ZFY20170509-1501
  
#endif
  _esc_pwm_pin = pwm_pin;
}
#endif
/**
 * \par Function
 *   reset
 * \par Description
 *   Reset the ESC motor available PIN by megapi port.
 * \param[in]
 *   port - megapipro esc port from PORT_1 to PORT_12
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
void MeMegaPiProESCMotor::reset(uint8_t port)
{
  _esc_pwm_pin = megapi_esc_Port[port].pwm_pin;
  pinMode(_esc_pwm_pin,OUTPUT);
  analogWrite(_esc_pwm_pin,120);
  last_speed = 255;
}

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
void MeMegaPiProESCMotor::setpin(uint8_t pwm_pin)
{
  _esc_pwm_pin = pwm_pin;
  last_speed = 255;
}
/**
 * \par Function
 *   init
 * \par Description
 *   init esc Motor
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
void MeMegaPiProESCMotor::init(void)
{
  pinMode(_esc_pwm_pin,OUTPUT);
  if (_esc_pwm_pin == 12)//timer1 
  {
    TCCR1A = _BV(WGM10);
    TCCR1B = _BV(CS11) | _BV(CS10) | _BV(WGM12);
  }
  analogWrite(_esc_pwm_pin,THROTTLE_MIN);
  delay(50);
  last_speed = THROTTLE_MIN - THROTTLE_OFFSET;
}
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
void MeMegaPiProESCMotor::run(int16_t speed)
{
  speed*=0.5;
  speed = speed > 50 ? 50 : speed;
  speed = speed < 0 ? 0 : speed;

  if(last_speed != speed)
  {
    last_speed = speed;
  }
  else
  {
    return;
  }
  analogWrite(_esc_pwm_pin,THROTTLE_MIN+speed);
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
void MeMegaPiProESCMotor::stop(void)
{
  run(0);
}

