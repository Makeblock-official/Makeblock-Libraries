/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    NumberDisplay.ino
 * @author  MakeBlock
 * @version V1.0.1
 * @date    2015/09/10
 * @brief   Description: this file is sample code for Seven-Segment Display device.
 *          It displays number from 0 to 100.
 * Function List:
 *    1. void    Me7SegmentDisplay::display(int16_t value)
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/07/24     1.0.0            Rebuild the old lib.
 * Rafael Lee       2015/09/10     1.0.1            Added some comments and macros.
 * </pre>
 */
#include "MeOrion.h"

Me7SegmentDisplay disp(PORT_6);

int i = 0;
void setup()
{
}

void loop()
{
  if(i > 100)
  {
    i = 0;
  }
  disp.display(i++);
  delay(100);
}

