/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    MicroSwitchTest.ino
 * @author  MakeBlock
 * @version V1.0.1
 * @date    2015/09/10
 * @brief   Description: this file is sample program for Limit Switch module.
 *
 * Function List:
 *    1. void    bool MeLimitSwitch::touched();
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/07/24     1.0.0            Rebuild the old lib.
 * Rafael Lee       2015/09/10     1.0.1            Added some comments and macros.
 * </pre>
 */

/* Includes ------------------------------------------------------------------*/
#include "MeOrion.h"

/* Private functions ---------------------------------------------------------*/
/* Me_LimitSwitch module can only be connected to PORT_3, PORT_4, PORT_6, 
// PORT_7,PORT_8 of base shield or from PORT_3 to PORT_8 of baseboard. */
MeLimitSwitch limitSwitch1(PORT_3, SLOT1);      // connecte to Me RJ25 Adapter SLOT1


MeLimitSwitch limitSwitch2(PORT_3, SLOT2);      // connecte to Me RJ25 Adapter SLOT2


void setup()
{
  Serial.begin(9600);
  Serial.println("Start.");
}

void loop()
{
  if(limitSwitch1.touched() )     // If the limit switch is touched, the  return value is true.
  {
    delay(10);
    Serial.println("limitSwitch1 DOWN.");
  }
  if(limitSwitch2.touched() )     // If the limit switch is touched, the  return value is true.
  {
    delay(10);
    Serial.println("limitSwitch2 DOWN.");
  }
}
