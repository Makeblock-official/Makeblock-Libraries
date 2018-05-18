/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class MeOnBoardTemp
 * \brief   Driver for Auriga On Board Temperature device.
 * @file    MeOnBoardTemp.cpp
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2016/04/01
 * @brief   Driver for Auriga On Board Temperature device.
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
 * This file is a drive for On Board Temperature device.
 *
 * \par Method List:
 *
 *    1. void MeOnBoardTemp::setpin(uint8_t digital_pin,uint8_t analog_pin);
 *    2. int16_t MeOnBoardTemp::readAnalog(void)
 *    3. float MeOnBoardTemp::readValue(void);
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2016/04/01     1.0.0            build the new.
 * </pre>
 *
 * @example MeOnBoardTempTest.ino
 */
#include "MeOnBoardTemp.h"

const int16_t TEMPERATURENOMINAL     = 25;    //Nominl temperature depicted on the datasheet
const int16_t SERIESRESISTOR         = 10000; // Value of the series resistor
const int16_t BCOEFFICIENT           = 3380;  // Beta value for our thermistor(3350-3399)
const int16_t TERMISTORNOMINAL       = 10000; // Nominal temperature value for the thermistor

#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the On Board Temperature to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
MeOnBoardTemp::MeOnBoardTemp(void) : MePort(0)
{

}

/**
 * Alternate Constructor which can call your own function to map the On Board Temperature to arduino port
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
MeOnBoardTemp::MeOnBoardTemp(uint8_t port) : MePort(port)
{

}
#else // ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the On Board Temperature to arduino port,
 * it will assigned the input pin for On Board Temperature.
 * \param[in]
 *   analog_pin - arduino port for analog signal input
 */
MeOnBoardTemp::MeOnBoardTemp(uint8_t analog_pin)
{
  _analog_pin = analog_pin;
}
#endif /* ME_PORT_DEFINED */

/**
 * \par Function
 *   setpin
 * \par Description
 *   Reset the analog available PIN by its arduino port.
 * \param[in]
 *   analog_pin - arduino port for analog signal input
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
void MeOnBoardTemp::setpin(uint8_t analog_pin)
{
  _analog_pin = analog_pin;
#ifdef ME_PORT_DEFINED
  s2 = _analog_pin;
#endif // ME_PORT_DEFINED
}

/**
 * \par Function
 *   readAnalog
 * \par Description
 *   Read the analog signal of On Board Temperature.
 * \par Output
 *   None
 * \return
 *   The vlaue from On Board Temperature's analog output
 * \par Others
 *   None
 */
int16_t MeOnBoardTemp::readAnalog(void)
{
#ifdef ME_PORT_DEFINED
  return( MePort::aRead2() );
#else //ME_PORT_DEFINED
  return analogRead(_analog_pin);
#endif //ME_PORT_DEFINED
}

/**
 * \par Function
 *   readValue
 * \par Description
 *   Read the value of On Board Temperature.
 * \par Output
 *   None
 * \return
 *   The vlaue from On Board Temperature.
 * \par Others
 *   None
 */
float MeOnBoardTemp::readValue(void)
{
  int16_t In_temp;
#ifdef ME_PORT_DEFINED
  In_temp = MePort::aRead2();
#else //ME_PORT_DEFINED
  In_temp = analogRead(_analog_pin);
#endif //ME_PORT_DEFINED
  float media;
  float temperatura;
  media = (float)In_temp;
  // Convert the thermal stress value to resistance
  media = 1023.0 / media - 1;
  media = SERIESRESISTOR / media;
  //Calculate temperature using the Beta Factor equation

  temperatura = media / TERMISTORNOMINAL;              // (R/Ro)
  temperatura = log(temperatura); // ln(R/Ro)
  temperatura /= BCOEFFICIENT;                         // 1/B * ln(R/Ro)
  temperatura += 1.0 / (TEMPERATURENOMINAL + 273.15);  // + (1/To)
  temperatura = 1.0 / temperatura;                     // Invert the value
  temperatura -= 273.15;                               // Convert it to Celsius
  return temperatura;
}

