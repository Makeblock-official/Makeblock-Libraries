/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    RJ25AdapterTest.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/02
 * @brief   Description: this file is sample code for RJ25 adapter module
 *
 * Function List:
 * 1. bool MeRGBLed::dWrite1(bool value)
 * 2. bool MeRGBLed::dRead2()
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Mark Yan     2015/09/02    1.0.0          rebuild the old lib
 * </pre>
 */

#include "MeOrion.h"

MePort output(PORT_4);


MePort input(PORT_3);


int val;
void setup()
{
  Serial.begin(9600);
}

void loop()
{
  output.dWrite1(HIGH);   /* SLOT1 HIGH */
  delay(500);
  output.dWrite1(LOW);    /* SLOT1 LOW */
  delay(500);

  val = input.dRead1();   /* read SLOT1 level */
  Serial.print("val=");
  Serial.println(val);
}

