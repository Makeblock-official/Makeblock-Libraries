/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    MeGasSensorTest.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/09
 * @brief   Description: this file is sample code for Me gas snesor device.
 *
 * Function List:
 *    1. uint8_t MeGasSensor::readDigital(void)
 *    2. MeGasSensor::readAnalog(void)
 *
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Mark Yan     2015/09/09    1.0.0          rebuild the old lib
 * </pre>
 */
#include "MeOrion.h"

MeGasSensor GasSensor1(PORT_8);

void setup() 
{
  Serial.begin(115200);
}

void loop() 
{
  Serial.print("Analog Value is: ");
  Serial.print(GasSensor1.readAnalog());
  Serial.print("----Status: ");
  if(GasSensor1.readDigital() == Gas_Exceeded)
  {
    Serial.println("The concentration exceeds");
  }
  else if(GasSensor1.readDigital() == Gas_not_Exceeded)
  {
    Serial.println("The concentration of the gas in the range");
  }
  delay(200);
}
