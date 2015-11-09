/**
 * \par Copyright (C), 2012-2015, MakeBlock
 * \brief   Driver for Mbot DC Motor.
 * \class   MBotDCMotor
 * \file    MeMbotDCMotor.cpp
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/01
 * @brief   Driver for Mbot DC Motor.
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

#include "MeMbotDCMotor.h"

#ifdef ME_PORT_DEFINED
MBotDCMotor::MBotDCMotor(uint8_t port) : MeDCMotor(port)
{

}

void MBotDCMotor::move(int direction, int speed)
{
  int leftSpeed = 0;
  int rightSpeed = 0;
  if(direction==1)
  {
    leftSpeed = -speed;
    rightSpeed = speed;
  }
  else if(direction==2)
  {
    leftSpeed = speed;
    rightSpeed = -speed;
  }
  else if(direction==3)
  {
    leftSpeed = speed;
    rightSpeed = speed;
  }
  else if(direction==4)
  {
    leftSpeed = -speed;
    rightSpeed = -speed;
  }
  MeDCMotor::reset(M1);
  MeDCMotor::run(leftSpeed);
  MeDCMotor::reset(M2);
  MeDCMotor::run(rightSpeed);
}
#endif //ME_PORT_DEFINED

