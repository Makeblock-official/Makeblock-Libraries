/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    WhiteBreathLightTest.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/02
 * @brief   Description: this file is sample code for RGB LED, It used for white breathing light
 *
 * Function List:
 * 1. bool MeRGBLed::setColor(uint8_t index, uint8_t red, uint8_t green, uint8_t blue) 
 * 2. void MeRGBLed::show()
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Mark Yan     2015/09/02    1.0.0          rebuild the old lib
 * </pre>
 */

#include "MeOrion.h"

MeRGBLed led1(PORT_3, SLOT1, 15);   /* parameter description: port, slot, led number */
MeRGBLed led2(PORT_3, SLOT2, 15);

int16_t bri = 0, st = 0;

void setup()
{

}

void loop()
{
  if(bri >= 100)
  {
    st = 1;
  }
  if(bri <= 0)
  {
    st = 0;
  }

  if(st)
  {
    bri--;
  }
  else
  {
    bri++;
  }
  for(int16_t t = 0; t < 15; t++)
  {
    led1.setColorAt(t, bri, bri, bri); /* parameter description: led number, red, green, blue, flash mode */
    led2.setColorAt(t, bri, bri, bri);
  }
  led1.show();
  led2.show();
  delay(20);
}

