/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    UltrasonicSensorTest.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/01
 * @brief   Description: this file is sample code for Me ultrasonic sensor module.
 *
 * Function List:
 * 1. double MeUltrasonicSensor::distanceCm(uint16_t MAXcm)
 *
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Mark Yan     2015/09/01    1.0.0          rebuild the old lib
 * </pre>
 */

// Do not forget to adapt the include path to your robot
// #include "MeAuriga.h"
// #include "MeMCore.h"
// #include "MeOrion.h"
// #include "MeMegaPi.h"
#include "MeOrion.h"

 /* Orion : Ultrasonic module can ONLY be connected to port 3, 4, 6, 7, 8 of base shield. */
MeUltrasonicSensor ultraSensor(PORT_7);

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.print("Distance : ");
  Serial.print(ultraSensor.distanceCm() );
  Serial.println(" cm");
  delay(100); /* the minimal measure interval is 100 milliseconds */
}

