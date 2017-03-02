/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    MePS2Test2.ino
 * @author  MakeBlock
 * @version V1.0.2
 * @date    2016/09/23
 * @brief   Description: this file is sample code for MePS2.
 *
 * Function List:
 *    1. int16_t MePS2::MeAnalog(uint8_t button);
 *    2. boolean MePS2::ButtonPressed(uint8_t button);
 *    3. void MePS2::loop(void);
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 *  Scott wang       2016/09/18          1.0.0         Build the lib.
 *  Scott wang       2016/09/19          1.0.1         Revise the  rocker.
 *  Scott            2016/09/23          1.0.2         Add BUTTON_L and BUTTON_R.
 * </pre>
 */

#include "MeAuriga.h"
#include "Arduino.h"
#include <SoftwareSerial.h>

MePS2 MePS2(PORT_16);  //Auriga should set to PORT_16

MeRGBLed led;

void setup() {
  MePS2.begin(115200);
  led.setpin(44);        //Auriga on board RGB LED is on pin 44.  
  led.setColor(0,0,0,0); //When power on, clear the led.
  led.show();
}

void loop() {
  MePS2.loop();
  if (MePS2.ButtonPressed(MeJOYSTICK_L1))
  {  
    led.setColor(0,60,0,0);
    led.show();
  }
  if (MePS2.ButtonPressed(MeJOYSTICK_L2))
  {
    led.setColor(0,0,0,0);
    led.show();
  }
}
