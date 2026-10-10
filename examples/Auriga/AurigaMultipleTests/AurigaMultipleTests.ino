/**
 * @file    AurigaMultipleTests.ino
 * @author  Nicolas Bourré
 * @version V1.0.0
 * @date    2020/04/01
 * @brief   Description: this file is sample code for RGB LED, gyro, temperature.
 *
 */

#include <MeAuriga.h>
#include <Wire.h>

#define ALL_LEDS 0
#define LEDNUM  12 // Auriga on-board light ring has 12 LEDs
#define LED_PIN 44

// on-board LED ring, at PORT0 (onboard)
MeRGBLed led( 0, LEDNUM );

MeSoundSensor mySound(PORT_6);

// Auriga onboard gyro is 0x69
MeGyro gyro(0, 0x69);

const int TEMP_PIN = A0;

float tempOutput = 0;
float batt;

int ct = millis(); // currentTime

void setup()
{
  // LED Ring controller is on Auriga D44/PWM
  led.setpin( LED_PIN );
  Serial.begin(115200); 
  gyro.begin();
}

void loop()
{
  // Board timing
  ct = millis();

  ledLoopTask();
  measureSound();
  gyroTask(ct);
  tempOutput = tempTask(ct);
  batt = readBatteryTask(ct);

  serialPrintTask(ct);
}

int sound_cnt = 0;   // sampling count
float sound_avg = 0; // sound average
short sound_reset_flag = 1;

void measureSound() {
  if (sound_reset_flag != 0) {
    sound_cnt = 0;
    sound_avg = 0.0;

    sound_reset_flag = 0;
  }
  
  sound_cnt++;
  
  sound_avg += mySound.strength();
}


void gyroTask(unsigned long currentTime) {
  static unsigned long lastTime = 0;
  const int rate = 20;
  
  static unsigned long lastPrint = 0;
  const int printRate = 200;
  
  if (currentTime - lastTime < rate) return;
  lastTime = currentTime;
 
  gyro.update();

  if (currentTime - lastPrint > printRate) {
    
    lastPrint = currentTime;

    Serial.print("X:");
    Serial.print(gyro.getAngleX() );
    Serial.print(" Y:");
    Serial.print(gyro.getAngleY() );
    Serial.print(" Z:");
    Serial.println(gyro.getAngleZ() );
  }
}

// Tâche de lecture de la tension batterie
float readBatteryTask(unsigned long currentTime) {
  static unsigned long lastTime = 0;
  const unsigned long interval = 500; // 500 ms
  static float voltage = 0.0;

  if (currentTime - lastTime >= interval) {
    lastTime = currentTime;

    int raw = analogRead(A4);

    const float VREF = 5.0; // Référence ADC (par défaut VCC)
    const float R1 = 100000.0;
    const float R2 = 51000.0;

    voltage = raw * (VREF / 1023.0) * ((R1 + R2) / R2);

  }

  return voltage;
}

void serialPrintTask(unsigned long currentTime) {
  static unsigned long lastTime = 0;
  const int rate = 200;
  
  if (currentTime - lastTime < rate) return;
  
  lastTime = currentTime;

  // Resetting sound values
  if (sound_cnt > 0) {
    sound_avg /= sound_cnt;
    sound_reset_flag = 1;
    Serial.print("Sound = ");
    Serial.print(sound_avg);    
  }
  

  Serial.print("\tTemperature = ");
  Serial.print(tempOutput);

  Serial.print("\tPower = ");
  Serial.print(batt);
  Serial.print(" V");

  Serial.println();
}

void ledLoopTask()
{
  static float j;
  static float f;
  static float k;
  
  for (uint8_t t = 0; t < LEDNUM; t++ )
  {
    uint8_t red	= 8 * (1 + sin(t / 2.0 + j / 4.0) );
    uint8_t green = 8 * (1 + sin(t / 1.0 + f / 9.0 + 2.1) );
    uint8_t blue = 8 * (1 + sin(t / 3.0 + k / 14.0 + 4.2) );
    led.setColorAt( t, red, green, blue );
  }
  led.show();

  j += random(1, 6) / 6.0;
  f += random(1, 6) / 6.0;
  k += random(1, 6) / 6.0;
}

/**
Temperature values
Src : https://github.com/search?q=TERMISTORNOMINAL+auriga&type=code
*/
float calculateTemp(int16_t In_temp)
{
  const int16_t TEMPERATURENOMINAL     = 25;    //Nominal temperature depicted on the datasheet
  const int16_t SERIESRESISTOR         = 10000; // Value of the series resistor
  const int16_t BCOEFFICIENT           = 3380;  // Beta value for our thermistor(3350-3399)
  const int16_t TERMISTORNOMINAL       = 10000; // Nominal temperature value for the thermistor
  
  float media;
  float temperatura;
  media = (float)In_temp;
  // Convert the thermal stress value to resistance
  media = 1023.0 / media - 1;
  media = SERIESRESISTOR / media;
  //Calculate temperature using the Beta Factor equation

  temperatura = media / TERMISTORNOMINAL;              // (R/Ro)
  temperatura = log(temperatura); // ln(R/Ro)
  temperatura /= BCOEFFICIENT;                         // 1/B * ln(R/Ro)
  temperatura += 1.0 / (TEMPERATURENOMINAL + 273.15);  // + (1/To)
  temperatura = 1.0 / temperatura;                     // Invert the value
  temperatura -= 273.15;                               // Convert it to Celsius
  return temperatura;
}



float tempTask(unsigned long currentTime) {
  static unsigned long lastTime = 0;
  const int rate = 200;
  static float temperature = 0.0;
  static int tempSensorValue = 0;
  
  if (currentTime - lastTime < rate)
  {
    return temperature;
  }
  
  lastTime = currentTime;
  
  tempSensorValue = analogRead(TEMP_PIN);
  temperature = calculateTemp(tempSensorValue);

  return temperature;
}