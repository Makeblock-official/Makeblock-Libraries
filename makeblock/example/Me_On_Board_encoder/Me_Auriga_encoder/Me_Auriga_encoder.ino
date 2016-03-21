#include <MeAuriga.h>

MeEncoderOnBoard Encoder_1(SLOT1);
MeEncoderOnBoard Encoder_2(SLOT2);

void isr_process_encoder1(void)
{
  if(digitalRead(Encoder_1.GetPortB()) == 0)
  {
    Encoder_1.PulsePosMinus();
  }
  else
  {
    Encoder_1.PulsePosPlus();;
  }
}

void isr_process_encoder2(void)
{
  if(digitalRead(Encoder_2.GetPortB()) == 0)
  {
    Encoder_2.PulsePosMinus();
  }
  else
  {
    Encoder_2.PulsePosPlus();
  }
}

void setup()
{
  attachInterrupt(Encoder_1.GetIntNum(), isr_process_encoder1, RISING);
  attachInterrupt(Encoder_2.GetIntNum(), isr_process_encoder2, RISING);
  Serial.begin(115200);
  
  //Set PWM 8KHz
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);

  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);
}

void loop()
{
  Encoder_1.setMotorPwm(100);
  Encoder_2.setMotorPwm(100);
  Encoder_1.Update_speed();
  Encoder_2.Update_speed();
  Serial.print("Spped 1:");
  Serial.print(Encoder_1.GetCurrentSpeed());
  Serial.print(" ,Spped 2:");
  Serial.println(Encoder_2.GetCurrentSpeed());
}
