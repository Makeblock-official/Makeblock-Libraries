/**
 * \par
 * @file    Auriga_MeGyroTest.ino
 * @author  Nick B
 * @version V1.0.0
 * @date    2023/11/08
 * @brief   Description: This file test MakeBlock Auriga Gyroscope 
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
 * Nick B     2023/11/28      1.0.0          Test Auriga gyro 
 * </pre>
 *
 */
#include "MeAuriga.h"

MeGyro gyro (0, 0x69);

void setup()
{
  Serial.begin(115200);
  gyro.begin();
  Serial.println("Open the serial plotter tool!");
  for (int i = 0; i < 20; i++) {
    Serial.print(".");
    delay(100);
  }
  Serial.println("Setup done!");
}

void loop()
{
  gyro.update();

  Serial.read();
  Serial.print("GyroX:");
  Serial.print(gyro.getAngleX() );
  Serial.print("GyroY:");
  Serial.print(gyro.getAngleY() );
  Serial.print("GyroZ:");
  Serial.println(gyro.getAngleZ() );
  delay(10);
}

