/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    TouchSensorTest.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/07
 * @brief   Description: this file is sample code for Me touch sensor device.
 *
 * Function List:
 *    1. bool MeTouchSensor::touched()
 *    2. void MeTouchSensor::SetTogMode(uint8_t TogMode)
 *
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Mark Yan     2015/09/07    1.0.0          rebuild the old lib
 * </pre>
 */
#include "MeOrion.h"

// Me_LimitSwitch module can only be connected to PORT_3, PORT_4, PORT_6,PORT_7,PORT_8 of base shield
// or from PORT_3 to PORT_8 of baseboard.
MeTouchSensor MeTouchSensor(PORT_6);

void setup()
{
  Serial.begin(9600);
  Serial.println("Start.");
  MeTouchSensor.SetTogMode(0);
}

void loop()
{
  if(MeTouchSensor.touched() )      // If the limit switch is touched, the  return value is true.
  {
    Serial.println("State: DOWN.");
    delay(1);
    while(MeTouchSensor.touched() )
    {
      ;                             // Repeat check the switch state, until released
    }
    delay(2);
  }
  if(!MeTouchSensor.touched() )
  {
    Serial.println("State: UP.");
    delay(1);
    while(!MeTouchSensor.touched() )
    {
      ;
    }
    delay(2);
  }
}

