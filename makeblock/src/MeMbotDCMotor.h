/**
 * \par Copyright (C), 2012-2015, MakeBlock
 * \class   MBotDCMotor
 * \brief   Driver for Mbot DC Motor.
 * \file    MeMbotDCMotor.h
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/01
 * @brief   Header file for MeMbotDCMotor.cpp.
 *
 * \par Copyright
 * This software is Copyright (C), 2012-2015, MakeBlock. Use is subject to license \n
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
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/09/01     1.0.0            Rebuild the old lib.
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

class MBotDCMotor : public MeDCMotor
{
public:
  MBotDCMotor(uint8_t port);

  void move(int direction, int speed);
};
#endif //ME_PORT_DEFINED
#endif //MeMbotDCMotor_H

