#include "MeOrion.h"
#include <SoftwareSerial.h>
#include <Wire.h>

MeEncoderMotor encoder;

void setup()
{
  encoder.begin();
  encoder.ResetMotor(MOTOR_1);
  encoder.SetMode(MOTOR_1, 1);
}

void loop()
{
  for (int i = 0; i < 255; i++)
  {
    encoder.SetPWM(MOTOR_1, i);
    delay(10);
  }
  for (int i = 255; i > 0; i--)
  {
    encoder.SetPWM(MOTOR_1, i);
    delay(10);
  }
}

