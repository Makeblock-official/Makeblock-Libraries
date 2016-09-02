/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    Me4ButtonTest.ino
 * @author  MakeBlock
 * @version V1.0.1
 * @date    2015/09/02
 * @brief   Description: this file is sample program for 4 Button module.
 *
 * Function List:
 * 1. uint8_t Me4Button::pressed()
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/07/24     1.0.0            Rebuild the old lib.
 * Rafael Lee       2015/09/02     1.0.1            Added some comments and macros.
 * </pre>
 */

/* Includes ------------------------------------------------------------------*/
#include <MeOrion.h>

/* Private variables ---------------------------------------------------------*/

/**
 * Me4Button module can only be connected to PORT_6, PORT_7, PORT_8 on base 
 * shield and Orion board.
 */
Me4Button btn(PORT_8);
// Me4Button module can only be connected to PORT_6,
// PORT_7, PORT_8 on base shield and Orion board.
uint8_t keyPressed = KEY_NULL;
uint8_t keyPressedPrevious = KEY_NULL;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  keyPressedPrevious = keyPressed;
  keyPressed = btn.pressed();
  if (keyPressedPrevious != keyPressed)
  {
    if (keyPressed == KEY_1)
    {
      Serial.println("KEY1 pressed");
    }
    if (keyPressed == KEY_2)
    {
      Serial.println("KEY2 pressed");
    }
    if (keyPressed == KEY_3)
    {
      Serial.println("KEY3 pressed");
    }
    if (keyPressed == KEY_4)
    {
      Serial.println("KEY4 pressed");
    }
    if (keyPressed == KEY_NULL)
    {
      Serial.println("KEY NULL");
    }
  }
}
