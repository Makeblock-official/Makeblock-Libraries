/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MeEncoderOnBoard
 * \brief   Driver for Encoder module on mZero.
 * @file    MeEncoderOnBoard.h
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/12/15
 * @brief   Header for MeEncoderOnBoard.cpp module
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
 * Mark Yan         2015/12/15     1.0.0            Build New
 * </pre>
 */

#ifndef ME_Encoder_OnBoard_H
#define ME_Encoder_OnBoard_H

#include <Arduino.h>
#include <stdbool.h>
#include <avr/interrupt.h>

enum
{
  MOTOR_1 = 0x00,
  MOTOR_2 = 0x01,
};

typedef struct
{
  int pulse;
  
  // current output pwm
  int pwm;

  // target speed
  double targetSpeed;
  // current speed
  double currentSpeed;

  // target position
  long targetPos;
  // current position
  long currentPos;
  // position counter the pulse
  long pulse_pos;
 
  // pid
  float p;
  float i;
  float d;
  float s;

  //ratio
  float ratio;
}Me_Encoder_type;

typedef struct
{
  uint8_t start;
  float p;
  float i;
  float d;
  float s;
  float ratio;
  uint16_t pulse;
  uint8_t end;
}eeprom_store_type;

typedef struct
{
  uint8_t port_A;   //INT
  uint8_t port_B;   //DIR
  uint8_t port_PWM; //PWM
  uint8_t port_H1;
  uint8_t port_H2;
} Encoder_port_type;

/**
 * Class: MeEncoderOnBoard
 * \par Description
 * Declaration of Class MeEncoderOnBoard
 */
class MeEncoderOnBoard 
{
public:
  MeEncoderOnBoard(uint8_t slot);
  uint8_t GetIntNum(void);
  uint8_t GetPortA(void);
  uint8_t GetPortB(void);
  long GetPulsePos(void);
  void SetPulsePos(long pulse_pos);
  void setMotorPwm(int pwm);
  int GetPwm(void);
  void PulsePosPlus(void);
  void PulsePosMinus(void);
  void SetCurrentSpeed(double speed);
  double GetCurrentSpeed(void);

private:
   volatile Me_Encoder_type encode_structure;
   uint8_t  _Port_A;
   uint8_t  _Port_B;
   uint8_t  _Port_PWM;
   uint8_t  _Port_H1;
   uint8_t  _Port_H2;
   uint8_t  _IntNum;
};
#endif



