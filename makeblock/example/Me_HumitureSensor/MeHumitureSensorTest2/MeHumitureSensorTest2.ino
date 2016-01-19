/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    MeHumitureSensorTest2.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/11/18
 * @brief   Description: this file is sample code for humiture sensor device.
 *
 * Function List:
 *    1. void MeHumiture::update(void)
 *    2. uint8_t MeHumiture::getHumidity(void)
 *    3. uint8_t MeHumiture::getTemperature(void)
 *    6. double MeHumiture::getFahrenheit(void)
 *    7. double MeHumiture::getKelvin(void)
 *    8. double MeHumiture::getdewPoint(void)
 *    9. double MeHumiture::getPointFast()
 *
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Mark Yan     2015/09/07    1.0.0          rebuild the old lib
 * forfish      2015/11/18    1.0.1          Add some functions.
 * </pre>
 */
#include "MeOrion.h"

MeHumiture humiture(PORT_6);

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  humiture.update();
  Serial.print("Humidity (%) =");
  Serial.println( humiture.getHumidity() );
  Serial.print("Temperature (oC) =");
  Serial.println( humiture.getTemperature() );
  Serial.print("Fahrenheit (oF) =");
  Serial.println( humiture.getFahrenheit() );
  Serial.print("Kelvin (K) =");
  Serial.println( humiture.getKelvin() );
  Serial.print("dewPoint (oC) =");
  Serial.println( humiture.getdewPoint() );
  //Serial.print("dewPointFast=");
  //Serial.println( humiture.getPointFast() );
  Serial.println("###########################");
  delay(1000);
}