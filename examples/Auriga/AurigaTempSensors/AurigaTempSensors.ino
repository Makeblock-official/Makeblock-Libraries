/**
 * @file    AurigaTempSensors.ino
 * @author  Nick B
 * @version V1.0.0
 * @date    2025/09/24
 * @brief   Description: This file demonstrates reading temperature from both 
 *          the MPU-6050 gyro sensor and the onboard NTC thermistor (NCP18XH103F03RB)
 *
 */

#include <MeAuriga.h>
#include <Wire.h>

// Auriga onboard gyro is 0x69
MeGyro gyro(0, 0x69);

// Initialize onboard temperature sensor on PORT_13 (A0)
MeOnBoardTemp onBoardTemp(PORT_13);

float gyroTemp = 0.0;
float onboardTemp = 0.0;
uint16_t onboardADC = 0;

int ct = millis(); // currentTime

void setup()
{
  Serial.begin(115200);
  gyro.begin();
  
  Serial.println("=== Auriga Temperature Sensors Test ===");
  Serial.println("Reading from:");
  Serial.println("1. MPU-6050 Gyro internal temperature sensor");
  Serial.println("2. Onboard NTC thermistor (NCP18XH103F03RB) on A0");
  Serial.println();
}

void loop()
{
  // Board timing
  ct = millis();

  gyroTempTask(ct);
  onboardTemp = onboardTempTask(ct);
  serialPrintTask(ct);
}

void gyroTempTask(unsigned long currentTime) {
  static unsigned long lastTime = 0;
  const int rate = 20; // Update gyro every 20ms
  
  if (currentTime - lastTime < rate) return;
  lastTime = currentTime;
 
  gyro.update();
  gyroTemp = gyro.getTemperature();
}

float onboardTempTask(unsigned long currentTime) {
  static unsigned long lastTime = 0;
  const int rate = 200; // Update onboard temp every 200ms
  static float temperature = 0.0;
  
  if (currentTime - lastTime < rate) {
    return temperature;
  }
  
  lastTime = currentTime;
  
  temperature = onBoardTemp.readValue();
  onboardADC = onBoardTemp.readAnalog();

  return temperature;
}

void serialPrintTask(unsigned long currentTime) {
  static unsigned long lastTime = 0;
  static int detailedCounter = 0;
  const int rate = 500; // Print every 500ms
  
  if (currentTime - lastTime < rate) return;
  
  lastTime = currentTime;

  // Print basic readings in CSV format for plotting
  Serial.print("Gyro: ");
  Serial.print(gyroTemp, 2);
  Serial.print("°C, Onboard: ");
  Serial.print(onboardTemp, 2);
  Serial.print("°C, ADC: ");
  Serial.println(onboardADC);
  
  // Print detailed comparison every 10 readings (5 seconds)
  detailedCounter++;
  if (detailedCounter >= 10) {
    Serial.println("--- Temperature Comparison ---");
    Serial.print("MPU-6050 Gyro Temperature: ");
    Serial.print(gyroTemp, 2);
    Serial.println("°C");
    Serial.print("Onboard NTC Temperature:   ");
    Serial.print(onboardTemp, 2);
    Serial.println("°C");
    Serial.print("Temperature Difference:    ");
    Serial.print(abs(gyroTemp - onboardTemp), 2);
    Serial.println("°C");
    // Note: Gyro temperature is typically higher due to the MPU-6050 chip generating heat during operation
    Serial.println();
    detailedCounter = 0;
  }
}