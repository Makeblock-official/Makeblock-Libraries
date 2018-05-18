/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    MePS2Test.ino
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

#include "MeMegaPi.h"
#include "Arduino.h"
#include "SoftwareSerial.h"
MePS2 MePS2(PORT_15);
void setup() {
  Serial.begin(115200);
  MePS2.begin(115200);
}

void loop() {
  MePS2.loop();
  if (MePS2.ButtonPressed(MeJOYSTICK_UP))
  {
    Serial.println("UP is pressed!");
  }
  if (MePS2.ButtonPressed(MeJOYSTICK_DOWN))
  {
    Serial.println("DOWN is pressed!");
  }
  if (MePS2.ButtonPressed(MeJOYSTICK_LEFT))
  {
    Serial.println("LEFT is pressed!");
  }
  if (MePS2.ButtonPressed(MeJOYSTICK_RIGHT))
  {
    Serial.println("RIGHT is pressed!");
  }
  if (MePS2.ButtonPressed(MeJOYSTICK_L1))
  {
    Serial.println("L1 is pressed!");
  }
  if (MePS2.ButtonPressed(MeJOYSTICK_L2))
  {
    Serial.println("L2 is pressed!");
  }
  if (MePS2.ButtonPressed(MeJOYSTICK_R1))
  {
    Serial.println("R1 is pressed!");
  }
  if (MePS2.ButtonPressed(MeJOYSTICK_R2))
  {
    Serial.println("R2 is pressed!");
  }
  if (MePS2.ButtonPressed(MeJOYSTICK_TRIANGLE))
  {
    Serial.println("TRIANGLE is pressed!");
  }
  if (MePS2.ButtonPressed(MeJOYSTICK_XSHAPED))
  {
    Serial.println("XSHAPED is pressed!");
  }
  if (MePS2.ButtonPressed(MeJOYSTICK_SQUARE))
  {
    Serial.println("SQUARE is pressed!");
  }
  if (MePS2.ButtonPressed(MeJOYSTICK_ROUND))
  {
    Serial.println("ROUND is pressed!");
  }
  if (MePS2.ButtonPressed(MeJOYSTICK_START))
  {
    Serial.println("START is pressed!");
  }
  if (MePS2.ButtonPressed(MeJOYSTICK_SELECT))
  {
    Serial.println("SELECT is pressed!");
  }
  if (MePS2.ButtonPressed(MeJOYSTICK_MODE))
  {
    Serial.println("MODE is pressed!");
  }
  if (MePS2.ButtonPressed(MeJOYSTICK_BUTTON_L))
  {
    Serial.println("BUTTON_L is pressed!");
  }
  if (MePS2.ButtonPressed(MeJOYSTICK_BUTTON_R))
  {
    Serial.println("BUTTON_R is pressed!");
  }
  
  if(MePS2.MeAnalog(MeJOYSTICK_RX))     
  {
    Serial.print("MePS2_RX value is: ");
    Serial.println(MePS2.MeAnalog(MeJOYSTICK_RX),DEC);
  }   
  if( MePS2.MeAnalog(MeJOYSTICK_RY))
  {
    Serial.print("MePS2_RY value is: ");
    Serial.println(MePS2.MeAnalog(MeJOYSTICK_RY),DEC);
  }
  if(MePS2.MeAnalog(MeJOYSTICK_LX))    
  {
    Serial.print("MePS2_LX value is: ");
    Serial.println(MePS2.MeAnalog(MeJOYSTICK_LX),DEC);
  }
  if(MePS2.MeAnalog(MeJOYSTICK_LY))  
  {
    Serial.print("MePS2_LY value is: ");
    Serial.println(MePS2.MeAnalog(MeJOYSTICK_LY),DEC);
  }
}
