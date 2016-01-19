/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MeEncoderOnBoard
 * \brief   Driver for Encoder module on mZero.
 * @file    MeEncoderOnBoard.cpp
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/11/10
 * @brief   Driver for Encoder module on mZero.
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
 * This file is a drive for Encoder On mZero.
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

Encoder_port_type encoder_Port[2] =
{
  { 19, 42, 44, 49, 48},
  { 18, 43, 45, 47, 46},
};

MeEncoderOnBoard::MeEncoderOnBoard(uint8_t slot)
{
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
  
  if(_Port_A == 19)
  {
    _IntNum = 4;
  }
  else if(_Port_A == 18)
  {
    _IntNum = 5;
  }
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
  int pwm_temp = constrain(pwm,-255,255);
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