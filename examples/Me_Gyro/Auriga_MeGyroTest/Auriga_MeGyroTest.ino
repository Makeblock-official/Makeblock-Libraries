/**
 * \par
 * @file    Auriga_MeGyroTest.ino
 * @author  Nick B
 * @version V1.1.0
 * @date    2025/09/24
 * @brief   Description: This file test MakeBlock Auriga Gyroscope and Accelerometer
 *
 * Function List:
 * 1. void MeGyro::begin(void)
 * 2. void MeGyro::update(void) 
 * 3. double MeGyro::getAngleX(void)
 * 4. double MeGyro::getAngleY(void)
 * 5. double MeGyro::getAngleZ(void)
 * 6. double MeGyro::getAccX(void)
 * 7. double MeGyro::getAccY(void)
 * 8. double MeGyro::getAccZ(void)
 *
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Nick B     2023/11/28      1.0.0          Test Auriga gyro
 * Nick B     2025/09/24      1.1.0          Added acceleration display 
 * </pre>
 *
 */
#include "MeAuriga.h"

MeGyro gyro (0, 0x69);

void setup()
{
  Serial.begin(115200);
  gyro.begin();
  
  // Setup message - will appear in Serial Monitor
  Serial.println("=== Auriga Gyro & Accelerometer Test ===");
  Serial.println("IMPORTANT: Open Tools -> Serial Plotter for data visualization!");
  Serial.println("Data format: AngleX AngleY AngleZ AccelX AccelY AccelZ Temp");
  Serial.println("Initializing sensor...");
  
  for (int i = 0; i < 20; i++) {
    Serial.print(".");
    delay(100);
  }
  Serial.println(" Setup complete!");
  Serial.println("Switch to Serial Plotter now for real-time graphs.");
  delay(2000); // Give time to switch to plotter
}

void loop()
{
  gyro.update();

  // Serial Plotter compatible format with labels
  Serial.print("AngleX:");
  Serial.print(gyro.getAngleX(), 2);
  Serial.print("\tAngleY:");
  Serial.print(gyro.getAngleY(), 2);
  Serial.print("\tAngleZ:");
  Serial.print(gyro.getAngleZ(), 2);
  Serial.print("\tAccelX:");
  Serial.print(gyro.getAccX(), 3);
  Serial.print("\tAccelY:");
  Serial.print(gyro.getAccY(), 3);
  Serial.print("\tAccelZ:");
  Serial.println(gyro.getAccZ(), 3);
  
  delay(50); // 20 Hz update rate for smooth plotting
}

