/**
 * \par Copyright (C), 2012-2015, MakeBlock
 * @file    MeWifi.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/09
 * @brief   Description: this file is sample code for Me WiFi module.
 *          The wifi inherited the MeSerial class from SoftwareSerial.
 *
 * Function List:
 * 1. void MeBluetooth::begin()
 * 2. int16_t MeBluetooth::available()
 * 3. size_t MeSerial::write(uint8_t byte)
 * 4. int16_t MeSerial::read(void)
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

