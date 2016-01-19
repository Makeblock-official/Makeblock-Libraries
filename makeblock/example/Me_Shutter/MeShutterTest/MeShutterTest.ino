/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    MeShutterTest.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/01
 * @brief   Description: this file is sample code for Me Shutter device.
 *
 * Function List:
 * 1. void MeShutter::shotOn()
 * 2. void MeShutter::shotOff()
 * 3. void MeShutter::focusOn()
 * 4. void MeShutter::focusOff()
 *
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Mark Yan     2015/09/01    1.0.0          rebuild the old lib
 * </pre>
 */
#include "MeOrion.h"

// MeShutter module can only be connected to the PORT_3, PORT_4, PORT_6 on Orion board
MeShutter myshutter(PORT3);

void setup()
{
  /* initialize serial communications at 9600 bps */
  Serial.begin(9600);
}

void loop()
{
  if(Serial.available())
  {
    char a = Serial.read();
    if(a == 's')
    {
      myshutter.focusOn();
      delay(1000);
      myshutter.shotOn();
      delay(200);
      myshutter.shotOff();
      myshutter.focusOff();
    }
    if(a == 'f')
    {
      myshutter.focusOn();
      delay(1500);
      myshutter.focusOff();
    }
  }
}

