/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    LimitSwitchTest.ino
 * @author  MakeBlock
 * @version V1.0.0
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

/* Private variables ---------------------------------------------------------*/
// Me_LimitSwitch module can only be connected to PORT_3, PORT_4, PORT_6, PORT_7, 
// PORT_8 of base shield or from PORT_3 to PORT_8 of baseboard.
MeLimitSwitch limitSwitch(PORT_6);

/* Private functions ---------------------------------------------------------*/
void setup()
{
  Serial.begin(9600);
  Serial.println("Start.");
}

void loop()
{
  if(limitSwitch.touched() )      // If the limit switch is touched, the  return value is true.
  {
    Serial.println("State: DOWN.");
    delay(1);
    while(limitSwitch.touched() )
    {
      ;// Repeat check the switch state, until released.
    }
    delay(2);
  }
  if(!limitSwitch.touched() )
  {
    Serial.println("State: UP.");
    delay(1);
    while(!limitSwitch.touched() )
    {
      ;
    }
    delay(2);
  }
}
