/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MePotentiometer
 * \brief   Driver for Me potentiometer module.
 * @file    MePotentiometer.h
 * @author  MakeBlock
 * @version V1.0.1
 * @date    2015/09/09
 * @brief   Header for MePotentiometer.cpp.
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
 * Rafael Lee       2015/09/09     1.0.1            Added some private variables and methods.
 * </pre>
 */

#ifndef MePotentiometer_H
#define MePotentiometer_H

#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>
#include "MeConfig.h"

#ifdef ME_PORT_DEFINED
#include "MePort.h"
#endif // ME_PORT_DEFINED

/**
 * Class: MePotentiometer
 * \par Description
 * Declaration of Class MePotentiometer.
 */
#ifndef ME_PORT_DEFINED
class MePotentiometer
#else // !ME_PORT_DEFINED
class MePotentiometer : public MePort
#endif  // !ME_PORT_DEFINED
{
public:
#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the Me potentiometer device to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
  MePotentiometer(void);

/**
 * Alternate Constructor which can call your own function to map the Me potentiometer device to arduino port
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
  MePotentiometer(uint8_t port);
#else // ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the Me potentiometer device to arduino port
 * \param[in]
 *   potentiometerPin - arduino port for potentiometer output port
 */
  MePotentiometer(uint8_t potentiometerPin);
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
  void setpin(uint8_t potentiometerPin);

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
  uint16_t read(void);
private:
  volatile uint8_t _potentiometerPin;
};
#endif // MePotentiometer_H
