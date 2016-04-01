/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class MeOnBoardTemp
 * \brief   Driver for Auriga On Board Temperature device.
 * @file    MeOnBoardTemp.h
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2016/04/01
 * @brief   Header for MeOnBoardTemp.cpp module
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
 */
#ifndef MeOnBoardTemp_H
#define MeOnBoardTemp_H

#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>
#include "MeConfig.h"


#ifdef ME_PORT_DEFINED
#include "MePort.h"
#endif // ME_PORT_DEFINED

#define Gas_Exceeded     (0x00)
#define Gas_not_Exceeded (0x01)

/**
 * Class: MeOnBoardTemp
 * \par Description
 * Declaration of Class MeOnBoardTemp
 */
#ifndef ME_PORT_DEFINED  
class MeOnBoardTemp
#else // !ME_PORT_DEFINED
class MeOnBoardTemp : public MePort
#endif  // !ME_PORT_DEFINED
{
public:
#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the On Board Temperature to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
  MeOnBoardTemp(void);

/**
 * Alternate Constructor which can call your own function to map the On Board Temperature to arduino port
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
  MeOnBoardTemp(uint8_t port);
#else // ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the On Board Temperature to arduino port,
 * it will assigned the input pin for On Board Temperature.
 * \param[in]
 *   analog_pin - arduino port for analog signal input
 */
  MeOnBoardTemp(uint8_t analog_pin)
#endif // ME_PORT_DEFINED
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
  void setpin(uint8_t analog_pin);

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
  int16_t readAnalog(void);

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
  float readValue(void);

private:
  volatile uint8_t _analog_pin;
};
#endif

