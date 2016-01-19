/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    MeLightSensorTestWithLEDon.ino
 * @author  MakeBlock
 * @version V1.0.1
 * @date    2015/09/10
 * @brief   Description: this file is sample program for Me Light Sensor module.
 *
 * Function List:
 *    1. void    MeLightSensor::lightOn();
 *    2. int16_t MeLightSensor::read();
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
MeLightSensor lightSensor(PORT_6);
int value = 0;      // a variable for the lightSensor's value


/* Private functions ---------------------------------------------------------*/
/**
 * \par Function
 *    setup
 * \par Description
 *    Run once, initialize serial port.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None.
 * \par Others
 *    None
 */
void setup()
{
  // initialize serial communications at 9600 bps
  Serial.begin(9600);
  lightSensor.lightOn();
}


/**
 * \par Function
 *    loop
 * \par Description
 *    Run continuously, print light sensor's DAC value .
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None.
 * \par Others
 *    None
 */
void loop()
{
  // read the lightSensor value:
  value = lightSensor.read();

  // print the results to the serial monitor:
  Serial.print("value = ");
  Serial.println(value);
  // wait 100 milliseconds before the next loop
  delay(100);
}
