/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    TemperatureTest.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/07
 * @brief   Description: this file is sample code for temperature sensor device.
 *
 * Function List:
 *    1. bool MeTemperature::temperature()
 *
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Mark Yan     2015/09/07    1.0.0          rebuild the old lib
 * </pre>
 */
#include "MeOrion.h"

MeTemperature myTemp(PORT_8, SLOT2);

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.print("Temperature=");
  Serial.println(myTemp.temperature() );
  delay(1000);
}

