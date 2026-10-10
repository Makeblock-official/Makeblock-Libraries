#include "MeAuriga.h"

struct RGBSensorValues {
  uint8_t r[4];
  uint8_t g[4];
  uint8_t b[4];
} rgbValues;

enum class ColorPhase { PHASE_RED, PHASE_GREEN, PHASE_BLUE };

unsigned long currentTime;

MeRGBLineFollower tracker;

char version[8];

void setup() {
  Serial.begin(115200);
  
  tracker.begin();
  tracker.getFirmwareVersion(version);
  
  Serial.print("RGB Line follower Version :");
  Serial.println(version);
  
  tracker.setRGBColour(RGB_COLOUR_GREEN);
  
  delay(3000);

}

void loop() {
  currentTime = millis();
  
  tracker.loop();
  
  updateRGBValuesTask(currentTime);
  serialPrintTask(currentTime);
}

void serialPrintTask(unsigned long currentTime) {
  static unsigned long lastTime = 0;
  const unsigned long rate = 100; // ms
  
  if (currentTime - lastTime < rate) return;
  lastTime = currentTime;

  // Print values for each sensor channel
  for (int i = 0; i < 4; i++) {
    Serial.print("RGB"); Serial.print(i + 1); Serial.print(": (");
    Serial.print(rgbValues.r[i]); Serial.print(",");
    Serial.print(rgbValues.g[i]); Serial.print(",");
    Serial.print(rgbValues.b[i]); Serial.print(")  ");
  }
  Serial.println(); // newline after all 4 sensors
}


void updateRGBValuesTask(unsigned long currentTime) {
  static unsigned long lastTime = 0;
  // Don't update too fast to avoid I2C overloading
  const unsigned long rate = 8; // ms
  
  const unsigned int stabilizeDelay = 500;
  
  static ColorPhase currentPhase = ColorPhase::PHASE_RED;
  
  if (currentTime - lastTime < rate) return;
  lastTime = currentTime;
  
  switch (currentPhase) {
    case ColorPhase::PHASE_RED:
      tracker.setRGBColour(RGB_COLOUR_RED);
      delayMicroseconds(stabilizeDelay);  // wait for light to stabilize
      tracker.updataAllSensorValue();
      rgbValues.r[0] = tracker.getADCValueRGB1();
      rgbValues.r[1] = tracker.getADCValueRGB2();
      rgbValues.r[2] = tracker.getADCValueRGB3();
      rgbValues.r[3] = tracker.getADCValueRGB4();
      currentPhase = ColorPhase::PHASE_GREEN;
      break;

    case ColorPhase::PHASE_GREEN:
      tracker.setRGBColour(RGB_COLOUR_GREEN);
      delayMicroseconds(stabilizeDelay);
      tracker.updataAllSensorValue();
      rgbValues.g[0] = tracker.getADCValueRGB1();
      rgbValues.g[1] = tracker.getADCValueRGB2();
      rgbValues.g[2] = tracker.getADCValueRGB3();
      rgbValues.g[3] = tracker.getADCValueRGB4();
      currentPhase = ColorPhase::PHASE_BLUE;
      break;

    case ColorPhase::PHASE_BLUE:
      tracker.setRGBColour(RGB_COLOUR_BLUE);
      delayMicroseconds(stabilizeDelay);
      tracker.updataAllSensorValue();
      rgbValues.b[0] = tracker.getADCValueRGB1();
      rgbValues.b[1] = tracker.getADCValueRGB2();
      rgbValues.b[2] = tracker.getADCValueRGB3();
      rgbValues.b[3] = tracker.getADCValueRGB4();
      currentPhase = ColorPhase::PHASE_RED;
      break;
  }
}
