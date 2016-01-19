/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    MeJoystickTest.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/01
 * @brief   Description: this file is sample code for Me Joystick device.
 *
 * Function List:
 * 1. int16_t MeJoystick::readX()
 * 2. int16_t MeJoystick::readY()
 * 3. void CalCenterValue(int16_t x_offset,int16_t y_offset);
 * 4. float MeJoystick::angle()
 * 5. float MeJoystick::OffCenter()
 *
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Mark Yan     2015/09/01    1.0.0          rebuild the old lib
 * </pre>
 */
#include "MeOrion.h"

MeJoystick joystick(PORT_6);

int16_t x = 0;    /* a variable for the Joystick's x value */
int16_t y = 0;    /* a variable for the Joystick's y value */
float angle = 0;       /* The relative angle of XY */
float OffCenter = 0;    /* offset with the center */

void setup()
{
  /* initialize serial communications at 9600 bps */
  Serial.begin(9600);
}

void loop()
{
  /* read the both joystick axis values: */
  x = joystick.readX();
  y = joystick.readY();
  angle = joystick.angle();
  OffCenter = joystick.OffCenter();

  /* print the results to the serial monitor: */
  Serial.print("Joystick X = ");
  Serial.print(x);
  Serial.print("  Joystick Y = ");
  Serial.print(y);
  Serial.print("  angle =");
  Serial.print(angle);
  Serial.print("  OffCenter= ");
  Serial.println(OffCenter);
  /* wait 10 milliseconds before the next loop */
  delay(10);
}

