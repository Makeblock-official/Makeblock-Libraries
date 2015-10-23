#include "MeOrion.h"
#include <SoftwareSerial.h>
#include <Wire.h>

MeEncoderMotor encoder;

void setup()
{
  encoder.begin();
  encoder.ResetMotor(MOTOR_1);
  encoder.SetMode(MOTOR_1, 0);
  encoder.ResetMotor(MOTOR_2);
  encoder.SetMode(MOTOR_2, 0);
}

void loop()
{
  //encoder.MoveSpeed(MOTOR_2, 60);
  encoder.MoveTo(MOTOR_1,3600);
  encoder.MoveTo(MOTOR_2,7200);
  //encoder.MoveTo(MOTOR_1, 720);
//   for (int i = 0; i < 255; i++)
//   {
//     encoder.SetPWM(MOTOR_1, i);
//     delay(10);
//   }
//   for (int i = 255; i > 0; i--)
//   {
//     encoder.SetPWM(MOTOR_1, i);
//     delay(10);
//   }
}

