/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MePIRMotionSensor
 * \brief   Driver for Me PIR Motionn Sensor module.
 * @file    MePIRMotionSensor.h
 * @author  MakeBlock
 * @version V1.0.1
 * @date    2015/09/04
 * @brief   Header for MePIRMotionSensor.cpp
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
 *    1. void    MePIRMotionSensor::setpin(uint8_t ModePin,uint8_t SensorPin)
 *    2. void    MePIRMotionSensor::SetPirMotionMode(uint8_t ModePin)
 *    3. bool    MePIRMotionSensor::isHumanDetected();
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/07/24     1.0.0            Rebuild the old lib.
 * Rafael Lee       2015/09/04     1.0.1            Added some comments and macros.
 * </pre>
 *
 */

#ifndef MePIRMotionSensor_H
#define MePIRMotionSensor_H

#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>
#include "MeConfig.h"

#ifdef ME_PORT_DEFINED
#include "MePort.h"
#endif  // ME_PORT_DEFINED

/**
 * Class: MePIRMotionSensor
 * \par Description
 * Declaration of Class MePIRMotionSensor.
 */
#ifndef ME_PORT_DEFINED
class MePIRMotionSensor
#else // !ME_PORT_DEFINED
class MePIRMotionSensor : public MePort
#endif  // ME_PORT_DEFINED
{
public:
#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the Motion Sensor device to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
  MePIRMotionSensor();

/**
 * Alternate Constructor which can call your own function to map the Motion Sensor device to arduino port
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
  MePIRMotionSensor(uint8_t port);
#else // ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the Motion Sensor device to arduino port
 * \param[in]
 *   ModePin - arduino port for BIS0001's A port
 * \param[in]
 *   SensorPin - arduino port for BIS0001's Vo port
 */
  MePIRMotionSensor(uint8_t ModePin,uint8_t SensorPin);
#endif // ME_PORT_DEFINED
/**
 * \par Function
 *    setpin
 * \par Description
 *    Reset the PIR motion sensor available PIN by its arduino port.
 * \param[in]
 *   ModePin - arduino port for BIS0001's A port
 * \param[in]
 *   SensorPin - arduino port for BIS0001's Vo port
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    Set global variable _KeyPin and s2
 */
  void setpin(uint8_t ModePin,uint8_t SensorPin);

/**
 * \par Function
 *    SetPirMotionMode
 * \par Description
 *    Set PIR motion sensor's trigger mode.
 * \param[in]
 *    ModePin - 1: Retriggerable,   0: Unrepeatable trigger
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void SetPirMotionMode(uint8_t ModePin);

/**
 * \par Function
 *    isHumanDetected
 * \par Description
 *    Is human been detected.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    ture: human is detected 
 *    false: no human been detected 
 * \par Others
 *    None
 */
  bool isHumanDetected();
private:
  static volatile uint8_t _SensorPin;
  static volatile uint8_t _ModePin;
};
#endif // MePIRMotionSensor_H

