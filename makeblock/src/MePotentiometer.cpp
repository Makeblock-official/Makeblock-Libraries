/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MePotentiometer
 * \brief   Driver for Me potentiometer module.
 * @file    MePotentiometer.cpp
 * @author  MakeBlock
 * @version V1.0.1
 * @date    2015/09/08
 * @brief   Driver for Me potentiometer module.
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
 *    1. void     MePotentiometer::setpin(uint8_t potentiometerPin); 
 *    2. uint16_t MePotentiometer::read(void); 
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/07/24     1.0.0            Rebuild the old lib.
 * Rafael Lee       2015/09/08     1.0.1            Added some comments and macros.
 * </pre>
 *
 * @example PotentiometerTest.ino
 */

/* Includes ------------------------------------------------------------------*/
#include "MePotentiometer.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the Me potentiometer device to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
MePotentiometer::MePotentiometer(void) : MePort(0)
{

}

/**
 * Alternate Constructor which can call your own function to map the Me potentiometer device to arduino port
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
MePotentiometer::MePotentiometer(uint8_t port) : MePort(port)
{

}
#else // ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the Me potentiometer device to arduino port
 * \param[in]
 *   potentiometerPin - arduino port for potentiometer output port
 */
MePotentiometer::MePotentiometer(uint8_t potentiometerPin)
{
  _potentiometerPin = potentiometerPin;
  pinMode(_potentiometerPin, INPUT);
}
#endif // ME_PORT_DEFINED 
/**
 * \par Function
 *    setpin
 * \par Description
 *    Reset the potentiometer device available PIN by its arduino port.
 * \param[in]
 *   potentiometerPin - arduino port for potentiometer output port
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MePotentiometer::setpin(uint8_t potentiometerPin)
{
  _potentiometerPin = potentiometerPin;
  pinMode(_potentiometerPin, INPUT);
#ifdef ME_PORT_DEFINED
  s2 = potentiometerPin;
#endif // ME_PORT_DEFINED
}

/**
 * \par Function
 *    read()
 * \par Description
 *    Read DAC value of Me potentiometer module.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    The value of potentiometer device(0-1024)
 * \par Others
 *    None
 */
uint16_t MePotentiometer::read(void)
{
#ifdef ME_PORT_DEFINED
  return(MePort::aRead2() );
#else // ME_PORT_DEFINED
  return analogRead(_potentiometerPin);
#endif // ME_PORT_DEFINED
}

