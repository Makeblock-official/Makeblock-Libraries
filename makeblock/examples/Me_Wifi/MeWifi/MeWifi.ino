/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    MeWifi.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/09
 * @brief   Description: this file is sample code for Me WiFi module.
 *          The wifi inherited the MeSerial class from SoftwareSerial.
 *
 * \par Method List:
 *  inherited from MeSerial
 *
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Mark Yan     2015/09/09    1.0.0          rebuild the old lib
 * </pre>
 */
#include "MeOrion.h"
#include <SoftwareSerial.h>

MeWifi Wifi(PORT_4);

void setup()
{
  Serial.begin(9600);
  Wifi.begin(9600);
  Serial.println("Wifi Start!");
}

void loop()
{
  char inDat;
  char outDat;
  if(Wifi.available() )
  {
    char c = Wifi.read();
    Serial.print(c);
  }
  if(Serial.available() )
  {
    outDat = Serial.read();
    Wifi.write(outDat);
  }
}

