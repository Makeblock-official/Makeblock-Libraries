/**
 * \par Copyright (C), 2012-2017, MakeBlock
 * @file    MeMegaPiProDCMotorTest.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2017/05/10
 * @brief   Description: this file is sample code for MegaPi Pro DC motor device.
 *
 * Function List:
 *    1. void MeMegaPiProDCMotorTest::run(int16_t speed)
 *    2. void MeMegaPiProDCMotorTest::stop(void)
 *
 * \par History:
 * <pre>
 * <Author>        <Time>        <Version>      <Descr>
 * Lan weiting     2017/05/10     1.0.0          build the new
 * Zzipeng         2017/05/12     1.0.1          build the new
 * </pre>
 */
#include "MeMegaPiPro.h"

MeDCMotor motor1(M9);

MeDCMotor motor2(M10);

MeDCMotor motor3(M11);

MeDCMotor motor4(M12);

uint8_t motorSpeed = 250;

void setup()
{
}

void loop()
{
  motor1.run(motorSpeed); /* value: between -255 and 255. */
  motor2.run(motorSpeed); /* value: between -255 and 255. */
  motor3.run(motorSpeed);
  motor4.run(motorSpeed);
  delay(2000);
  motor1.stop();
  motor2.stop();
  motor3.stop();
  motor4.stop();
  delay(1000);
  motor1.run(-motorSpeed);
  motor2.run(-motorSpeed);
  motor3.run(-motorSpeed);
  motor4.run(-motorSpeed);
  delay(2000);
  motor1.stop();
  motor2.stop();
  motor3.stop();
  motor4.stop();
  delay(1000);
}
