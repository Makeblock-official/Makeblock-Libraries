/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    EncoderMotorTestRunSpeed.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/11/19
 * @brief   Description: this file is sample code for Encoder Motor  device.
 *
 * Function List:
 *
 *    1. void MeEncoderMotor::begin();
 *    2. boolean MeEncoderMotor::runSpeed(float speed);
 *
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * forfish      2015/11/19    1.0.0          add some descriptions
 * </pre>
 */

#include "MeOrion.h"
#include <Wire.h>
#include <SoftwareSerial.h>

MeEncoderMotor motor2(0x09, SLOT2);   //  motor at slot2

void setup()
{
  motor2.begin();
  Serial.begin(9600);
}

void loop()
{
  motor2.runSpeed(-150);
  delay(3000);
  motor2.runSpeed(0);
  delay(2000);
}

