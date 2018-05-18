/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    SerialControlStepper.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/11/19
 * @brief   Description: this file is sample code for Stepper Driver device.
 *
 * Function List:
 *
 *    1. void MeStepper::moveTo(long absolute); 
 *    2. void MeStepper::move(long relative);
 *    3. boolean MeStepper::run();
 *    4. void MeStepper::setMaxSpeed(float speed);
 *    5. void MeStepper::setAcceleration(float acceleration);
 *
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Mark Yan     2015/09/07    1.0.0          rebuild the old lib.
 * forfish      2015/11/19    1.0.0          add some descriptions.
 * </pre>
 */

#include "MeOrion.h"
#include <SoftwareSerial.h>

MeStepper stepper(PORT_1); 

void setup()
{  
  Serial.begin(9600);
  // Change these to suit your stepper if you want
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(20000);
}

void loop()
{
  if(Serial.available())
  {
    char a = Serial.read();
    switch(a)
    {
      case '0':
      stepper.moveTo(0);
      break;
      case '1':
      stepper.moveTo(200);
      break;
      case '2':
      stepper.move(50);
      break;
      case '3':
      stepper.move(100);
      break;
      case '4':
      stepper.move(200);
      break;
      case '5':
      stepper.move(400);
      break;
      case '6':
      stepper.move(600);
      break;
      case '7':
      stepper.move(4000);
      break;
      case '8':
      stepper.move(8000);
      break;
      case '9':
      stepper.move(3200);
      break;
    }
  }
  stepper.run();
}
