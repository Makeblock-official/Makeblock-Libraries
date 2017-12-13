/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \brief   Test
 * @file    MeLightSensorTestResetPort.ino
 * @author  MakeBlock
 * @version V1.0.1
 * @date    2015/09/05
 * @brief   Test Me Light Sensor module with port reconfiguration.
 *
 * Function List:
 *    1. int16_t MeLightSensor::read();
 *    2. void  MeLightSensor::reset(uint8_t port);
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/07/24     1.0.0            rebuild the old lib
 * Rafael Lee       2015/09/05     1.0.1            First Version.
 * </pre>
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "MeOrion.h"

/* Private variables ---------------------------------------------------------*/
//Instantiate class MeLightSensor
MeLightSensor lightSensor(PORT_6);
//Me Light Sensor can only be connected to PORT_6, PORT_7, PORT_8 on Orion Board of Makeblock.
// lightSensor's value
int value = 0;

/* Private functions ---------------------------------------------------------*/
/**
 * \par Function
 *    setup
 * \par Description
 *    Run once, initialize serial port and reset port.
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
  // Initialize serial communication using 9600 bps
  Serial.begin(9600);
  // Reset the port of instance lightSensor to PORT_8
  lightSensor.reset(PORT_8);
  // Reset lightSensor's port to PORT_8.
}

/**
 * \par Function
 *    loop
 * \par Description
 *    Run continuously, print light sensor's DAC value.
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
  // Read the lightSensor's value
  value = lightSensor.read();

  // Print results to serial monitor
  Serial.print("value = ");
  Serial.println(value);
  // Wait 100 milliseconds before next loop
  delay(100);
}
