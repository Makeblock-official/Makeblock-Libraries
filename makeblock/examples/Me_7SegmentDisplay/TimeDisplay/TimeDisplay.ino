/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    TimeDisplay.ino
 * @author  MakeBlock
 * @version V1.0.1
 * @date    2015/09/11
 * @brief   Description: this file is sample code for Seven-Segment LED device.
 *          It acts as a timer.
 * Function List:
 *    1. void Me7SegmentDisplay::init(void)
 *    2. void Me7SegmentDisplay::set(uint8_t = BRIGHT_2, uint8_t = ADDR_AUTO, \
                                        uint8_t = STARTADDR)
 *    3. void Me7SegmentDisplay::display(uint8_t DispData[]);
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/07/24     1.0.0            Rebuild the old lib.
 * Rafael Lee       2015/09/11     1.0.1            Added some comments and macros.
 * </pre>
 */

#include "MeOrion.h"

uint8_t		TimeDisp[] = { 0x00, 0x00, 0x00, 0x00 };
unsigned char	second;
unsigned char	minute		= 0;
unsigned char	hour		= 12;
long		lastTime	= 0;

Me7SegmentDisplay disp(PORT_6);

void setup()
{
  disp.set();
  disp.init();
}

void loop()
{
  if(millis() - lastTime >= 1000)
  {
    TimingISR();

    TimeUpdate();
    disp.display(TimeDisp);

    lastTime = millis();
  }
}

void TimingISR()
{
  second++;
  if(second == 60)
  {
    minute++;
    if(minute == 60)
    {
      hour++;
      if(hour == 24)
      {
        hour = 0;
      }
      minute = 0;
    }
    second = 0;
  }
}

void TimeUpdate(void)
{
  TimeDisp[0] = minute / 10;
  TimeDisp[1] = minute % 10;
  TimeDisp[2] = second / 10;
  TimeDisp[3] = second % 10;
}

