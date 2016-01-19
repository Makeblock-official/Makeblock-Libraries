/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    Knob.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/02
 * @brief   Description: This file is sample code for servo. Servo.h should be 
 * included before makeblock header file
 *
 * Function List:
 * 1. uint8_t Servo::attach()
 * 2. uint8_t Servo::write()
 *
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Mark Yan     2015/09/02    1.0.0          rebuild the old lib
 * </pre>
 */
#include "MeOrion.h"

MePort port(PORT_3);
Servo myservo;  // create servo object to control a servo
int16_t servopin =  port.pin2();//attaches the servo on PORT_3 SLOT2 to the servo object
int16_t potpin = A1;//  // analog pin used to connect the potentiometer
int16_t val;    // variable to read the value from the analog pin

void setup()
{
  myservo.attach(servopin);  // attaches the servo on servopin
}

void loop()
{
  val = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
  val = map(val, 0, 1023, 0, 179);     // scale it to use it with the servo (value between 0 and 180)
  myservo.write(val);                  // sets the servo position according to the scaled value
  delay(15);                           // waits for the servo to get there
}

