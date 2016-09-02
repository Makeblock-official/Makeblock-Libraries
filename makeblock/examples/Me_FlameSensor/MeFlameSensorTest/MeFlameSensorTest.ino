/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    MeFlameSensorTest.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/09
 * @brief   Description: this file is sample code for Me flame snesor device.
 *
 * Function List:
 *    1. uint8_t MeFlameSensor::readDigital(void)
 *    2. MeFlameSensor::readAnalog(void)
 *
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Mark Yan     2015/09/09    1.0.0          rebuild the old lib
 * </pre>
 */
#include "MeOrion.h"

MeFlameSensor FlameSensor1(PORT_8);

void setup() 
{
  Serial.begin(115200);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  Serial.print("Analog Value is: ");
  Serial.print(FlameSensor1.readAnalog());
  Serial.print("----Status: ");
  if(FlameSensor1.readDigital() == Fire)
  {
    Serial.println("Fire");
  }
  else if(FlameSensor1.readDigital() == NoFire)
  {
    Serial.println("NoFire");
  }
  delay(200);
}

