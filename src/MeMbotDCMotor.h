/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \brief   Driver for Mbot DC Motor.
 * \file    MeMbotDCMotor.h
 * @author  MakeBlock
 * @version V1.0.1
 * @date    2015/11/09
 * @brief   Header for MeMbotDCMotor.cpp module
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
 * This file is Hardware adaptation layer between Mbot board and all
 * MakeBlock drives
 * 
 * \par Method List:
 *
 *    1. void MBotDCMotor::move(int direction, int speed);
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/09/01     1.0.0            Rebuild the old lib.
 * Mark Yan         2015/11/09     1.0.1            fix some comments error.
 * </pre>
 */

#ifndef MeMbotDCMotor_H
#define MeMbotDCMotor_H

#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>
#include "MeConfig.h"

#ifdef ME_PORT_DEFINED
#include "MePort.h"
#endif

#ifdef ME_PORT_DEFINED
#include "MeDCMotor.h"

/**
 * Class: MBotDCMotor
 * \par Description
 * Declaration of Class MBotDCMotor.
 */
class MBotDCMotor : public MeDCMotor
{
public:
/**
 * Alternate Constructor which can call your own function to map the MBot DCMotor to arduino port,
 * the slot2 pin will be used here since specify slot is not be set.
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
  MBotDCMotor(uint8_t port);

/**
 * \par Function
 *    move
 * \par Description
 *    MBot DCMotor moves with the speed and direction.
 * \param[in]
 *    direction - The direction of the MBot DCMotor's movement
 * \param[in]
 *    speed - The speed of MBot DCMotor's movement.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void move(int direction, int speed);
};
#endif //ME_PORT_DEFINED
#endif //MeMbotDCMotor_H

