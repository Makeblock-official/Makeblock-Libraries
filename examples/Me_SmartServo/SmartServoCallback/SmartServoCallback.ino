/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    SmartServoCallback.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2017/07/10
 * @brief   Description: this file is sample code for Smart servo device.
 *
 * Function List:
 * 1. boolean MeSmartServo::assignDevIdRequest(void);
 * 2. boolean MeSmartServo::moveTo(uint8_t dev_id,long angle_value,float speed,smartServoCb callback);
 * 3. boolean MeSmartServo::move(uint8_t dev_id,long angle_value,float speed,smartServoCb callback);
 *
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Mark Yan     2017/07/10    1.0.0          build the new
 * </pre>
 */
#include <SoftwareSerial.h>
#include "MeAuriga.h"

MeSmartServo mysmartservo(PORT5);   //UART2 is on port 5

long loopTime = 0;

void callback_test(uint8_t servoNum)
{
   switch(servoNum)
   {
     case 1:
       Serial.println("servo 1 has been reached!");
       break;
     case 2:
       Serial.println("servo 2 has been reached!");
       break;
   }
}

void setup()
{
  Serial.begin(115200);
  mysmartservo.begin(115200);
  delay(5);
  mysmartservo.assignDevIdRequest();
  delay(50);
  Serial.println("setup!");
  loopTime = millis();
  mysmartservo.moveTo(1,0,50);
  mysmartservo.moveTo(2,0,50);
  delay(2000);
}

void loop()
{
  mysmartservo.moveTo(1,360,50,callback_test);    //device ID, angle, speed;  relative angle move;
  mysmartservo.moveTo(2,360,50,callback_test);    //device ID, angle, speed;  relative angle move;
}

