/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    Me_Megapipro_encoder_pwm.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2017/05/11
 * @brief   Description: this file is sample code for Megapi Pro encoder motor device.
 *
 * Function List:
 *    1. uint8_t MeEncoderOnBoard::getPortB(void);
 *    2. uint8_t MeEncoderOnBoard::getIntNum(void);
 *    3. void MeEncoderOnBoard::pulsePosPlus(void);
 *    4. void MeEncoderOnBoard::pulsePosMinus(void);
 *    5. void MeEncoderOnBoard::setMotorPwm(int pwm);
 *    6. double MeEncoderOnBoard::getCurrentSpeed(void);
 *    7. void MeEncoderOnBoard::loop(void);
 *    8. void MeEncoderOnBoard::setTarPWM(int16_t pwm_value);
 *
 * \par History:
 * <pre>
 * <Author>        <Time>        <Version>      <Descr>
 * Lan weiting     2017/05/11     1.0.0          build the new
 * Zzipeng         2017/05/18     1.0.1          set all timer frequency at 970hz
 * </pre>
 */

#include <MeMegaPiPro.h>

MeEncoderOnBoard Encoder_1(SLOT1);
MeEncoderOnBoard Encoder_2(SLOT2);

void isr_process_encoder1(void)
{
  if(digitalRead(Encoder_1.getPortB()) == 0)
  {
    Encoder_1.pulsePosMinus();
  }
  else
  {
    Encoder_1.pulsePosPlus();;
  }
}

void isr_process_encoder2(void)
{
  if(digitalRead(Encoder_2.getPortB()) == 0)
  {
    Encoder_2.pulsePosMinus();
  }
  else
  {
    Encoder_2.pulsePosPlus();
  }
}

void setup()
{
  attachInterrupt(Encoder_1.getIntNum(), isr_process_encoder1, RISING);
  attachInterrupt(Encoder_2.getIntNum(), isr_process_encoder2, RISING);
  Serial.begin(115200);

//set pwm 1khz
  TCCR1A = _BV(WGM10);//PIN12
  TCCR1B = _BV(CS11) | _BV(CS10) | _BV(WGM12);
 
  TCCR2A = _BV(WGM21) | _BV(WGM20);//PIN8
  TCCR2B = _BV(CS22);

  TCCR3A = _BV(WGM30);//PIN9
  TCCR3B = _BV(CS31) | _BV(CS30) | _BV(WGM32);

  TCCR4A = _BV(WGM40);//PIN5
  TCCR4B = _BV(CS41) | _BV(CS40) | _BV(WGM42);
}

void loop()
{
  if(Serial.available())
  {
    char a = Serial.read();
    switch(a)
    {
      case '0':
      Encoder_1.setTarPWM(0);
      Encoder_2.setTarPWM(0);
      break;
      case '1':
      Encoder_1.setTarPWM(100);
      Encoder_2.setTarPWM(-100);
      break;
      case '2':
      Encoder_1.setTarPWM(200);
      Encoder_2.setTarPWM(-200);
      break;
      case '3':
      Encoder_1.setTarPWM(255);
      Encoder_2.setTarPWM(-255);
      break;
      case '4':
      Encoder_1.setTarPWM(-100);
      Encoder_2.setTarPWM(100);
      break;
      case '5':
      Encoder_1.setTarPWM(-200);
      Encoder_2.setTarPWM(200);
      break;
      case '6':
      Encoder_1.setTarPWM(-255);
      Encoder_2.setTarPWM(255);
      break;
      default:
      break;
    }
  }
  Encoder_1.loop();
  Encoder_2.loop();
  Serial.print("Spped 1:");
  Serial.print(Encoder_1.getCurrentSpeed());
  Serial.print(" ,Spped 2:");
  Serial.println(Encoder_2.getCurrentSpeed());
}

