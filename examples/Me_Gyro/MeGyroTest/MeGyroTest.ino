/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    GyroRotation.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/09
 * @brief   Description: this file is sample code for MeGyro device.
 *
 * Function List:
 * 1. void MeGyro::begin(void)
 * 2. void MeGyro::update(void) 
 * 3. double MeGyro::angleX(void)
 * 4. double MeGyro::angleY(void)
 * 5. double MeGyro::angleZ(void)
 *
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Mark Yan     2015/09/09    1.0.0          rebuild the old lib
 * </pre>
 *
 */
#include "MeOrion.h"
#include <Wire.h>

MeGyro gyro;
void setup()
{
  Serial.begin(115200);
  gyro.begin();
}

void loop()
{
  gyro.update();
  Serial.read();
  Serial.print("X:");
  Serial.print(gyro.getAngleX() );
  Serial.print(" Y:");
  Serial.print(gyro.getAngleY() );
  Serial.print(" Z:");
  Serial.println(gyro.getAngleZ() );
  delay(10);
}

