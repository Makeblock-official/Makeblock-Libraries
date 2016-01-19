/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    NumberFlow.ino
 * @author  MakeBlock
 * @version V1.0.1
 * @date    2015/09/02
 * @brief   Description: this file is sample code for Seven-Segment LED device.
 *          It test the number flow display
 * Function List:
 *    1. void Me7SegmentDisplay::init(void)
 *    2. void Me7SegmentDisplay::set(uint8_t = BRIGHT_2, uint8_t = ADDR_AUTO, \
                                        uint8_t = STARTADDR)
 *    3. void Me7SegmentDisplay::display(uint8_t BitAddr, uint8_t DispData);
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/07/24     1.0.0            Rebuild the old lib.
 * Rafael Lee       2015/09/02     1.0.1            Added some comments and macros.
 * </pre>
 */

/* Includes ------------------------------------------------------------------*/
#include "MeOrion.h"

/* Private variables ---------------------------------------------------------*/
Me7SegmentDisplay disp(PORT_6);

/* Private functions ---------------------------------------------------------*/
void setup()
{
  disp.init();
  disp.set(BRIGHTNESS_2);
}

void loop()
{
  int8_t NumTab[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }; // 0~9,A,b,C,d,E,F
  uint8_t ListDisp[4];
  unsigned char	i = 0;
  unsigned char	count = 0;
  delay(150);
  while(1)
  {
    i = count;
    count++;
    if(count == sizeof(NumTab) )
    {
      count = 0;
    }
    for(unsigned char BitSelect = 0; BitSelect < 4; BitSelect++)
    {
      ListDisp[BitSelect] = NumTab[i];
      i++;
      if(i == sizeof(NumTab) )
      {
        i = 0;
      }
    }
    disp.display( (uint8_t)0, ListDisp[0]);
    disp.display( (uint8_t)1, ListDisp[1]);
    disp.display( (uint8_t)2, ListDisp[2]);
    disp.display( (uint8_t)3, ListDisp[3]);
    delay(300);
  }
}

