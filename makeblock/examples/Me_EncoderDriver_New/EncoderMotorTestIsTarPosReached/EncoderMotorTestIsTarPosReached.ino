/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    EncoderMotorTestIsTarPosReaches.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2016/03/23
 * @brief   Description: this file is sample code for Encoder Motor  device.
 *
 * Function List:
 *
 *    1. void MeEncoderMotor::begin();
 *    2. boolean MeEncoderMotor::moveTo(float angle, float speed);
 *    3. void MeEncoderNew::setCurrentPosition(long pulse_counter);
 *    4. long MeEncoderNew::getCurrentPosition();
 *    5. boolean MeEncoderNew::isTarPosReached(void);
 *
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * forfish      2016/03/19    1.0.0          add some descriptions
 * </pre>
 */

#include "MeOrion.h"
#include <Wire.h>
#include <SoftwareSerial.h>

MeEncoderNew motor2(0x09, SLOT2);   //  motor at slot2

void setup()
{
  motor2.begin();
  Serial.begin(9600);
  motor2.setCurrentPosition(0);
}

void loop()
{
  motor2.moveTo(2000, 200);
  while(motor2.isTarPosReached() == false)
  {
    Serial.print("pos: ");
    Serial.println(motor2.getCurrentPosition());
  }
  motor2.moveTo(0, 200);
  while(motor2.isTarPosReached() == false)
  {
    Serial.print("pos: ");
    Serial.println(motor2.getCurrentPosition());
  }
}
