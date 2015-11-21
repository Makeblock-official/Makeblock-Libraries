/**
 * \par Copyright (C), 2012-2015, MakeBlock
 * @file    TestStepperDriver.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/07
 * @brief   Description: this file is sample code for Stepper Driver device.
 *
 * Function List:
 *
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Mark Yan     2015/09/07    1.0.0          rebuild the old lib.
 * forfish      2015/11/19    1.0.0          add some descriptions.
 * </pre>
 */

#include "MeOrion.h"

int dirPin = mePort[PORT_1].s1;//the direction pin connect to Base Board PORT1 SLOT1
int stpPin = mePort[PORT_1].s2;//the Step pin connect to Base Board PORT1 SLOT2

void setup()
{
  pinMode(dirPin, OUTPUT);
  pinMode(stpPin, OUTPUT);
}

void step(boolean dir,int steps)
{
  digitalWrite(dirPin,dir);
  delay(50);
  for(int i=0;i<steps;i++)
  {
    digitalWrite(stpPin, HIGH);
    delayMicroseconds(800);
    digitalWrite(stpPin, LOW);
    delayMicroseconds(800); 
  }
}

void loop()
{
  step(1,200);//run 200 step
  delay(1000);
  step(0,200);
  delay(1000);
}
