/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    PIRMotionSensorTest.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/10
 * @brief   Description: this file is sample program for PIR Motion module.
 *
 * Function List:
 * 1. void    MePIRMotionSensor::SetPirMotionMode(uint8_t ModePin)
 * 2. bool    MePIRMotionSensor::isHumanDetected();
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/07/24     1.0.0            Rebuild the old lib.
 * Rafael Lee       2015/09/10     1.0.1            Added some comments and macros.
 * </pre>
 */

/* Includes ------------------------------------------------------------------*/
#include "MeOrion.h"

/* Private variables ---------------------------------------------------------*/
MePIRMotionSensor myPIRsensor(PORT_3);

void setup()
{
  Serial.begin(9600);
  myPIRsensor.SetPirMotionMode(1);   //Continuous Trigger mode
}

void loop()
{
  if(myPIRsensor.isHumanDetected() )
  {
    Serial.println("People Motion Detected");
  }
  delay(20);
}

