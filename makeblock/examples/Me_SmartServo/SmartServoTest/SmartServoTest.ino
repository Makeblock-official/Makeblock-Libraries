/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    SmartServoTest.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2016/08/31
 * @brief   Description: this file is sample code for Smart servo device.
 *
 * Function List:
 * 1. boolean MeSmartServo::assignDevIdRequest(void);
 * 2. boolean MeSmartServo::moveTo(uint8_t dev_id,long angle_value,float speed);
 * 3. boolean MeSmartServo::move(uint8_t dev_id,long angle_value,float speed);
 * 4. long MeSmartServo::getAngleRequest(uint8_t devId);
 * 5. float MeSmartServo::getSpeedRequest(uint8_t devId);
 * 6. float MeSmartServo::getVoltageRequest(uint8_t devId);
 * 7. float MeSmartServo::getTempRequest(uint8_t devId);
 * 8. float MeSmartServo::getCurrentRequest(uint8_t devId);
 *
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Mark Yan     2016/08/31    1.0.0          build the new
 * </pre>
 */
#include <SoftwareSerial.h>
#include "MeAuriga.h"

MeSmartServo mysmartservo(PORT5);   //UART2 is on port 5

long loopTime = 0;

void setup()
{
  Serial.begin(115200);
  mysmartservo.begin(115200);
  delay(5);
  mysmartservo.assignDevIdRequest();
  delay(50);
  Serial.println("setup!");
  loopTime = millis();
}

void loop()
{
  mysmartservo.moveTo(1,360,50);   //device ID, angle, speed;  absolute angle move;
  loopTime = millis();
  while(millis() - loopTime < 2000)
  {
    Serial.print("angle:");
    Serial.print(mysmartservo.getAngleRequest(1));
    Serial.print(" speed:");
    Serial.print(mysmartservo.getSpeedRequest(1));
    Serial.print(" voltage:");
    Serial.print(mysmartservo.getVoltageRequest(1));
    Serial.print(" temp:");
    Serial.print(mysmartservo.getTempRequest(1));
    Serial.print(" Current:");
    Serial.println(mysmartservo.getCurrentRequest(1));
  }
  mysmartservo.move(1,-360,50);    //device ID, angle, speed;  relative angle move;
  loopTime = millis();
  while(millis() - loopTime < 2000)
  {
    Serial.print("angle:");
    Serial.print(mysmartservo.getAngleRequest(1));
    Serial.print(" speed:");
    Serial.print(mysmartservo.getSpeedRequest(1));
    Serial.print(" voltage:");
    Serial.print(mysmartservo.getVoltageRequest(1));
    Serial.print(" temp:");
    Serial.print(mysmartservo.getTempRequest(1));
    Serial.print(" Current:");
    Serial.println(mysmartservo.getCurrentRequest(1));
  }
}

