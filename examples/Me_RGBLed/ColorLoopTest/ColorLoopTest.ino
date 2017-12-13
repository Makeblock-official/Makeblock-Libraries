/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    ColorLoopTest.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/02
 * @brief   Description: this file is sample code for RGB LED, It used for random color change
 *
 * Function List:
 * 1. bool MeRGBLed::setColorAt(uint8_t index, uint8_t red, uint8_t green, uint8_t blue)
 * 2. void MeRGBLed::show()
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Mark Yan     2015/09/02    1.0.0          rebuild the old lib
 * </pre>
 */
#include "MeOrion.h"

MeRGBLed led(PORT_3);

float j, f, k;

void setup()
{

}

void loop()
{
  color_loop();
}

void color_loop()
{
  for(uint8_t t = 1; t < 15; t++)
  {
    uint8_t red	= 64 * (1 + sin(t / 2.0 + j / 4.0) );
    uint8_t green = 64 * (1 + sin(t / 1.0 + f / 9.0 + 2.1) );
    uint8_t blue = 64 * (1 + sin(t / 3.0 + k / 14.0 + 4.2) );
    led.setColorAt(t, red, green, blue);
  }
  led.show();
  j += random(1, 6) / 6.0;
  f += random(1, 6) / 6.0;
  k += random(1, 6) / 6.0;
}

