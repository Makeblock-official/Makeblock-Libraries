/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class MeGasSensor
 * \brief   Driver for Me gas snesor device.
 * @file    MeGasSensor.h
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/09
 * @brief   Header for MeGasSensor.cpp module
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
 */
#ifndef MeGasSensor_H
#define MeGasSensor_H

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
 * Class: MeGasSensor
 * \par Description
 * Declaration of Class MeGasSensor
 */
#ifndef ME_PORT_DEFINED  
class MeGasSensor
#else // !ME_PORT_DEFINED
class MeGasSensor : public MePort
#endif  // !ME_PORT_DEFINED
{
public:
#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the gas snesor to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
  MeGasSensor(void);

/**
 * Alternate Constructor which can call your own function to map the gas snesor to arduino port
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
  MeGasSensor(uint8_t port);
#else // ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the gas snesor to arduino port,
 * it will assigned the input pin for gas snesor.
 * \param[in]
 *   digital_pin - arduino port for digital signal input
 * \param[in]
 *   analog_pin - arduino port for analog signal input
 */
  MeGasSensor(uint8_t digital_pin,uint8_t analog_pin)
#endif // ME_PORT_DEFINED
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
  void setpin(uint8_t digital_pin,uint8_t analog_pin);

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
  uint8_t readDigital(void);

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
  int16_t readAnalog(void);
private:
  volatile uint8_t _digital_pin;
  volatile uint8_t _analog_pin;
};
#endif

