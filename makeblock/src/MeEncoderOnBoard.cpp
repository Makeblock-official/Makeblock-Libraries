/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MeEncoderOnBoard
 * \brief   Driver for Encoder module on MeAuriga and MeMegaPi.
 * @file    MeEncoderOnBoard.cpp
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/11/10
 * @brief   Driver for Encoder module on MeAuriga and MeMegaPi.
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
 * This file is a drive for Encoder On MeAuriga and MeMegaPi.
 *
 * \par Method List:
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * forfish         2015/11/10     1.0.0            Add description
 * </pre>
 *
 * @example MeEncoderOnBoardMoveTo.ino
 */

#include "MeEncoderOnBoard.h"
MeEncoderOnBoard::MeEncoderOnBoard()
{
	
}
MeEncoderOnBoard::MeEncoderOnBoard(uint8_t slot)
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
  _Slot = slot;
  _Port_A = encoder_Port[slot].port_A;
  _Port_B = encoder_Port[slot].port_B;
  _Port_PWM = encoder_Port[slot].port_PWM;
  _Port_H1 = encoder_Port[slot].port_H1;
  _Port_H2 = encoder_Port[slot].port_H2;

  pinMode(_Port_A, INPUT_PULLUP);
  pinMode(_Port_B, INPUT_PULLUP);
  pinMode(_Port_H1, INPUT_PULLUP);
  pinMode(_Port_H2, INPUT_PULLUP);
  
  encode_structure.pulse_pos = 0;
  
  if(_Port_A == 18)
  {
    _IntNum = 5;
  }
  else if(_Port_A == 19)
  {
    _IntNum = 4;
  }
  else if(_Port_A == 20)
  {
    _IntNum = 3;
  }
  else if(_Port_A == 21)
  {
    _IntNum = 2;
  }
  else if(_Port_A == 3)
  {
    _IntNum = 1;
  }
  else if(_Port_A == 2)
  {
    _IntNum = 0;
  }
  MeEncoderOnBoard::setMotorPwm(0);
  MeEncoderOnBoard::SetPulsePos(0);
  _Measurement_speed_time = millis();
}
void MeEncoderOnBoard::reset(uint8_t slot)
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
  _Slot = slot;
  _Port_A = encoder_Port[slot].port_A;
  _Port_B = encoder_Port[slot].port_B;
  _Port_PWM = encoder_Port[slot].port_PWM;
  _Port_H1 = encoder_Port[slot].port_H1;
  _Port_H2 = encoder_Port[slot].port_H2;

  pinMode(_Port_A, INPUT_PULLUP);
  pinMode(_Port_B, INPUT_PULLUP);
  pinMode(_Port_H1, INPUT_PULLUP);
  pinMode(_Port_H2, INPUT_PULLUP);
  
  encode_structure.pulse_pos = 0;
  
  if(_Port_A == 18)
  {
    _IntNum = 5;
  }
  else if(_Port_A == 19)
  {
    _IntNum = 4;
  }
  else if(_Port_A == 20)
  {
    _IntNum = 3;
  }
  else if(_Port_A == 21)
  {
    _IntNum = 2;
  }
  else if(_Port_A == 3)
  {
    _IntNum = 1;
  }
  else if(_Port_A == 2)
  {
    _IntNum = 0;
  }
  MeEncoderOnBoard::setMotorPwm(0);
  MeEncoderOnBoard::SetPulsePos(0);
  _Measurement_speed_time = millis();
}
uint8_t MeEncoderOnBoard::GetSlotNum(void)
{
  return _Slot;
}

uint8_t MeEncoderOnBoard::GetIntNum(void)
{
  return _IntNum;
}

uint8_t MeEncoderOnBoard::GetPortA(void)
{
  return _Port_A;
}

uint8_t MeEncoderOnBoard::GetPortB(void)
{
  return _Port_B;
}

long MeEncoderOnBoard::GetPulsePos(void)
{
  return encode_structure.pulse_pos;
}

void MeEncoderOnBoard::SetPulsePos(long pulse_pos)
{
  encode_structure.pulse_pos = pulse_pos;
}

void MeEncoderOnBoard::PulsePosPlus(void)
{
  encode_structure.pulse_pos++;
}

void MeEncoderOnBoard::PulsePosMinus(void)
{
  encode_structure.pulse_pos--;
}

void MeEncoderOnBoard::SetCurrentSpeed(double speed)
{
  encode_structure.currentSpeed = speed;
}

double MeEncoderOnBoard::GetCurrentSpeed(void)
{
  return encode_structure.currentSpeed;
}

int MeEncoderOnBoard::GetPwm(void)
{
  return encode_structure.pwm;
}

void MeEncoderOnBoard::setMotorPwm(int pwm)
{
  pwm = constrain(pwm,-255,255);
  encode_structure.pwm = pwm;
  if(pwm < 0)
  {
    digitalWrite(MeEncoderOnBoard::_Port_H1, LOW);
    digitalWrite(MeEncoderOnBoard::_Port_H2, HIGH);
    analogWrite(MeEncoderOnBoard::_Port_PWM, abs(pwm));
  }
  else
  {
    digitalWrite(MeEncoderOnBoard::_Port_H1, HIGH);
    digitalWrite(MeEncoderOnBoard::_Port_H2, LOW);
    analogWrite(MeEncoderOnBoard::_Port_PWM, abs(pwm));
  }
}

void MeEncoderOnBoard::Update_speed(void)
{
  if((millis() - _Measurement_speed_time) > 20)
  {
    uint16_t dt = millis() - _Measurement_speed_time;
    long cur_pos = MeEncoderOnBoard::GetPulsePos();
    MeEncoderOnBoard::SetCurrentSpeed(((cur_pos - _Last_pulse_pos)/357.3)*(1000/dt)*60);
    _Last_pulse_pos = cur_pos;
    _Measurement_speed_time = millis();
  }
}
void MeEncoderOnBoard::update()
{
	if(!_moving){
		return;
	}
  if((millis() - _Measurement_speed_time) > 20)
  {
    uint16_t dt = millis() - _Measurement_speed_time;
    long cur_pos = MeEncoderOnBoard::GetPulsePos();
    MeEncoderOnBoard::SetCurrentSpeed(((cur_pos - _Last_pulse_pos)/357.3)*(1000/dt)*60);
    _Last_pulse_pos = cur_pos;
    _Measurement_speed_time = millis();
	if(_mode==0){
		setMotorPwm(GetPwm()+(_targetSpeed-GetCurrentSpeed())/8.0);
	}else if(_mode==1){
		long dist = distanceToGo();
		int dir = dist>0?1:-1;
		if(abs(dist)>100)
		{
			setMotorPwm(GetPwm()+dir*(abs(_targetSpeed)-abs(GetCurrentSpeed()))/8.0);
		}else{
			setMotorPwm(dist);	
			if(abs(dist)<10&&_moving){
				_moving = false;
				_callback(_Slot,_extId);
			}
		}
	}
  }
}
long MeEncoderOnBoard::distanceToGo()
{
	return _targetPosition - _Last_pulse_pos;
}
void MeEncoderOnBoard::runSpeed(double speed)
{
  _mode = 0;
  _moving = true;
  _targetSpeed = speed;
}
void MeEncoderOnBoard::setSpeed(double speed)
{
  _targetSpeed = speed;
}
void MeEncoderOnBoard::move(long distance,cb callback,int extId)
{
  if(_targetSpeed==0){
	_targetSpeed = 100;
  }
  _extId = extId;
  _moving = true;
  _mode = 1;
  _targetPosition += distance;
  _callback = callback;
}
void MeEncoderOnBoard::moveTo(long position,cb callback,int extId)
{
  if(_targetSpeed==0){
	_targetSpeed = 100;
  }
  _extId = extId;
  _moving = true;
  _mode = 1;
  _targetPosition = position;
  _callback = callback;
}