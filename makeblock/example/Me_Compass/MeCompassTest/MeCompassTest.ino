/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    MeCompass.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/09
 * @brief   Description: this file is sample code for Me MeCompass device.
 *
 * Function List:
 * 1. void MeCompass::init(void)
 * 2. double MeCompass::getAngle(void)
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

MeCompass Compass(PORT_4);

void setup()
{
  Serial.begin(9600);
  Serial.println("Initializing I2C devices...");
  Compass.begin();
  Serial.println("Testing device connections...");
  Serial.println(Compass.testConnection() ? "HMC5883L connection successful" : "HMC5883L connection failed");
}

void loop()
{
  int16_t head_X, head_Y, head_Z;
  double angle_number = 0;
  
  head_X = Compass.getHeadingX();
  head_Y = Compass.getHeadingY();
  head_Z = Compass.getHeadingZ();

  angle_number = Compass.getAngle();
  Serial.println(angle_number, 1);
  delay(500);
}
