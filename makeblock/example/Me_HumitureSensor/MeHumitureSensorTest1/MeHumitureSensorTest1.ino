/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    MeHumitureSensorTest1.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/07
 * @brief   Description: this file is sample code for humiture sensor device.
 *
 * Function List:
 *    1. void MeHumiture::update(void)
 *    2. uint8_t MeHumiture::getHumidity(void)
 *    3. uint8_t MeHumiture::getTemperature(void)
 *
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Mark Yan     2015/09/07    1.0.0          rebuild the old lib
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
  Serial.print( humiture.getHumidity() );
  Serial.print(", Temperature (oC) =");
  Serial.println( humiture.getTemperature() );
  Serial.println("###########################");
  delay(1000);
}