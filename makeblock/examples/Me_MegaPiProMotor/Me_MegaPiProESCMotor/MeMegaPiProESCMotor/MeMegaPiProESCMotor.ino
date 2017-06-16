/**
 * \par Copyright (C), 2012-2017, MakeBlock
 * @file    MeMegaPiProESCMotor.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2017/05/016
 * @brief   Description: this file is sample code for MegaPiPro ESC motor device.
 *
 * Function List:
 *    1. void MeMegaPiProESCMotorTest::init(void);
 *    2. void MeMegaPiProESCMotorTest::run(int16_t speed);
 *    3. void MeMegaPiProESCMotorTest::stop(void);
 * 
 * \par History:
 * <pre>
 * <Author>        <Time>        <Version>      <Descr>
 * lan Weiting     2017/05/09     1.0.0          build the new
 * Zzipeng         2017/05/09     1.0.1          build the new
 * </pre>
 */
#include "MeMegaPiPro.h"

MeMegaPiProESCMotor bldcmotor1(PORT1);
MeMegaPiProESCMotor bldcmotor2(PORT2);
MeMegaPiProESCMotor bldcmotor3(PORT3);
MeMegaPiProESCMotor bldcmotor4(PORT4);
uint8_t motorSpeed = 40;//0~100
void setup() 
{
  // put your setup code here, to run once:
  bldcmotor1.init();
  bldcmotor2.init();
  bldcmotor3.init();
  bldcmotor4.init();
}

void loop() 
{
  // put your main code here, to run repeatedly:
   bldcmotor1.run(motorSpeed);
   bldcmotor2.run(motorSpeed);
   bldcmotor3.run(motorSpeed);
   bldcmotor4.run(motorSpeed);
}
