/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MeLightSensor
 * \brief   Driver for Me-Light Sensor module.
 * @file    MeLightSensor.cpp
 * @author  MakeBlock
 * @version V1.0.2
 * @date    2015/11/09
 * @brief   Driver for Me-Light Sensor module.
 *
 * \par Copyright
 * This software is Copyright (C), 2012-2016, MakeBlock. Use is subject to license \n
 * conditions. The main licensing options available are GPL V2 or Commercial: \n
 *
 * \par Open Source Licensing GPL V2
 * This is the appropriate option if you want to share the source code of your \n
 * application with everyone you distribute it to, and you also want to give them \n
 * the right to share who uses it. If you wish to use this software under Open \n
 * Source Licensing, you must contribute all your source code to the open source \n
 * community in accordance with the GPL Version 2 when your application is \n
 * distributed. See http://www.gnu.org/copyleft/gpl.html
 *
 * \par Description
 *
 * \par Method List:
 *
 *    1. void    MeLightSensor::setpin(uint8_t ledPin, uint8_t sensorPin)
 *    2. int16_t MeLightSensor::read()
 *    3. void    MeLightSensor::lightOn()
 *    4. void    MeLightSensor::lightOff()
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/07/24     1.0.0            Rebuild the old lib.
 * Rafael Lee       2015/09/02     1.0.1            Added function setpin and some comments.
 * Mark Yan         2015/11/09     1.0.2            fix some comments error.
 * </pre>
 *
 * @example MeLightSensorTestResetPort.ino \n
 * @example MeLightSensorTest.ino \n
 * @example MeLightSensorTestWithLEDon.ino
 */

/* Private define ------------------------------------------------------------*/
#include "MeLightSensor.h"

/* Private functions ---------------------------------------------------------*/
#ifdef ME_PORT_DEFINED

/**
 * Alternate Constructor which can call your own function to map the light sensor to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
MeLightSensor::MeLightSensor(void) : MePort(0)
{

}

/**
 * Alternate Constructor which can call your own function to map the light sensor to arduino port
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
MeLightSensor::MeLightSensor(uint8_t port) : MePort(port)
{

}
#else // ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the light sensor to arduino port
 * \param[in]
 *   ledPin - arduino port for led input
 * \param[in]
 *   sensorPin - arduino port for sensor data read
 */
MeLightSensor::MeLightSensor(uint8_t ledPin, uint8_t sensorPin)
{
  _ledPin = ledPin;
  _sensorPin = sensorPin;

  pinMode(_ledPin, OUTPUT);
  pinMode(_sensorPin, INPUT);

  digitalWrite(_ledPin,LOW);
}
#endif //ME_PORT_DEFINED

/**
 * \par Function
 *    setpin
 * \par Description
 *    Set I2C data pin, clock pin and clear display. \n
 *    dataPin - the DATA pin for Seven-Segment LED module. \n
 *    clkPin  - the CLK pin for Seven-Segment LED module.
 * \param[in]
 *    ledPin - Which pin on board that LEDpin is connecting to.
 * \param[in]
 *    sensorPin - Which pin on board that sensorPin is connecting to.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    Set global variable _KeyPin and s2
 */
void MeLightSensor::setpin(uint8_t ledPin, uint8_t sensorPin)
{
  _ledPin = ledPin;
  _sensorPin = sensorPin;

  pinMode(_ledPin, OUTPUT);
  pinMode(_sensorPin, INPUT);

#ifdef ME_PORT_DEFINED
  s1 = _ledPin;
  s2 = _sensorPin;
#endif // ME_PORT_DEFINED
}

/**
 * \par Function
 *    read
 * \par Description
 *    Read analog value of light sensor.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    MePort::aRead2() - DAC value of current light sensor's output.
 * \par Others
 */
int16_t MeLightSensor::read(void)
{
#ifdef ME_PORT_DEFINED
  return(MePort::aRead2());
#else //ME_PORT_DEFINED
  return(analogRead(_sensorPin));
#endif //ME_PORT_DEFINED

}

/**
 * \par Function
 *    lightOn
 * \par Description
 *    Turn on the LED on the light sensor.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 */
void MeLightSensor::lightOn(void)
{
#ifdef ME_PORT_DEFINED
  MePort::dWrite1(HIGH);
#else //ME_PORT_DEFINED
  digitalWrite(_ledPin,HIGH);
#endif //ME_PORT_DEFINED
}

/**
 * \par Function
 *    lightOff
 * \par Description
 *    Turn off the LED on the light sensor.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 */
void MeLightSensor::lightOff(void)
{
#ifdef ME_PORT_DEFINED
  MePort::dWrite1(LOW);
#else //ME_PORT_DEFINED
  digitalWrite(_ledPin,LOW);
#endif //ME_PORT_DEFINED

}
