/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class MeGasSensor
 * \brief   Driver for Me gas snesor device.
 * @file    MeGasSensor.cpp
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/09
 * @brief   Driver for Me gas snesor device.
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
 * This file is a drive for gas snesor device.
 *
 * \par Method List:
 *
 *    1. void MeGasSensor::setpin(uint8_t digital_pin,uint8_t analog_pin)
 *    2. uint8_t MeGasSensor::readDigital(void)
 *    3. int16_t MeGasSensor::readAnalog(void)
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/09/09     1.0.0            Rebuild the old lib.
 * </pre>
 *
 * @example MeGasSensorTest.ino
 */
#include "MeGasSensor.h"

#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the gas snesor to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
MeGasSensor::MeGasSensor(void) : MePort(0)
{

}

/**
 * Alternate Constructor which can call your own function to map the gas snesor to arduino port
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
MeGasSensor::MeGasSensor(uint8_t port) : MePort(port)
{

}
#else // ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the gas snesor to arduino port,
 * it will assigned the input pin for gas snesor.
 * \param[in]
 *   digital_pin - arduino port for digital signal input
 * \param[in]
 *   analog_pin - arduino port for analog signal input
 */
MeGasSensor::MeGasSensor(uint8_t digital_pin,uint8_t analog_pin)
{
  _digital_pin = digital_pin;
  _analog_pin = analog_pin;
  pinMode(_digital_pin, INPUT);
}
#endif /* ME_PORT_DEFINED */

/**
 * \par Function
 *   setpin
 * \par Description
 *   Reset the gas snesor available PIN by its arduino port.
 * \param[in]
 *   digital_pin - arduino port for digital signal input
 * \param[in]
 *   analog_pin - arduino port for analog signal input
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
void MeGasSensor::setpin(uint8_t digital_pin,uint8_t analog_pin)
{
  _digital_pin = digital_pin;
  _analog_pin = analog_pin;
  pinMode(_digital_pin, INPUT);
#ifdef ME_PORT_DEFINED
  s1 = _analog_pin;
  s2 = _digital_pin;
#endif // ME_PORT_DEFINED
}

/**
 * \par Function
 *   readDigital
 * \par Description
 *   Read the digital signal of gas snesor, It is the result of comparison between the threshold
 *   value and the analog output. The threshold value can be adjusted using a rheostat
 * \par Output
 *   None
 * \return
 *   true - The gas concentration exceeds \n
 *   false - The concentration of the gas in the range
 * \par Others
 *   None
 */
uint8_t MeGasSensor::readDigital(void)
{
#ifdef ME_PORT_DEFINED
  return( MePort::dRead2() );
#else //ME_PORT_DEFINED
  return digitalRead(_digital_pin);
#endif //ME_PORT_DEFINED
}

/**
 * \par Function
 *   readAnalog
 * \par Description
 *   Read the analog signal of gas snesor.
 * \par Output
 *   None
 * \return
 *   The vlaue from gas snesor's analog output
 * \par Others
 *   None
 */
int16_t MeGasSensor::readAnalog(void)
{
#ifdef ME_PORT_DEFINED
  return( MePort::aRead1() );
#else //ME_PORT_DEFINED
  return analogRead(_analog_pin);
#endif //ME_PORT_DEFINED
}

