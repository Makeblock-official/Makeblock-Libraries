/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   Me7SegmentDisplay
 * \brief   Driver for Me 7-Segment Serial Display module.
 * @file    Me7SegmentDisplay.cpp
 * @author  Frankie.Chu, MakeBlock
 * @version V1.0.5
 * @date    2016/07/27
 * @brief   Driver for Me 7 Segment Serial Display module.
 *
 * \par Copyright
 * This software is Copyright (C), 2012-2018, MakeBlock. Use is subject to license \n
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
 * Driver for Me 7 Segment Serial Display module.
 * \par Method List:
 *
 *    1.    void    Me7SegmentDisplay::init(void);
 *    2.    void    Me7SegmentDisplay::set(uint8_t brightness, uint8_t SetData, uint8_t SetAddr);
 *    3.    void    Me7SegmentDisplay::reset(uint8_t port);
 *    4.    void    Me7SegmentDisplay::setpin(uint8_t dataPin, uint8_t clkPin);
 *    5.    void    Me7SegmentDisplay::write(uint8_t SegData[]);
 *    6.    void    Me7SegmentDisplay::write(uint8_t BitAddr, uint8_t SegData);
 *    7.    void    Me7SegmentDisplay::display(uint16_t value);
 *    8.    void    Me7SegmentDisplay::display(int16_t value);
 *    9.    void    Me7SegmentDisplay::display(float value);
 *    10.   void    Me7SegmentDisplay::display(long value);
 *    11.   int16_t Me7SegmentDisplay::checkNum(float v,int16_t b);
 *    12.   void    Me7SegmentDisplay::display(double value, uint8_t digits);
 *    13.   void    Me7SegmentDisplay::display(uint8_t DispData[]);
 *    14.   void    Me7SegmentDisplay::display(uint8_t BitAddr, uint8_t DispData);
 *    15.   void    Me7SegmentDisplay::display(uint8_t BitAddr, uint8_t DispData, uint8_t point_on);
 *    16.   void    Me7SegmentDisplay::clearDisplay(void);
 *    17.   void    Me7SegmentDisplay::setBrightness(uint8_t brightness);
 *    18.   void    Me7SegmentDisplay::coding(uint8_t DispData[]);
 *    19.   uint8_t Me7SegmentDisplay::coding(uint8_t DispData);
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * makeblock        2013/08/08     0.0.1            According to hardware differences, initial revision                     
 * Mark Yan         2015/07/24     1.0.0            Rebuild the old lib.
 * Rafael Lee       2015/09/02     1.0.1            Added some comments and macros. Some bug fixed in coding function.
 * Mark Yan         2015/10/29     1.0.2            fix issue when display negative data.
 * Mark Yan         2015/11/09     1.0.3            fix some comments error.
 * Mark Yan         2015/11/12     1.0.4            fix driver API.
 * Mark Yan         2016/07/27     1.0.5            add display to support long type.
 * </pre>
 *
 * @example NumberDisplay.ino
 * @example NumberFlow.ino
 * @example TimeDisplay.ino
 *
 * DERIVED FROM
 * //  Author:Frankie.Chu
 * //  Date:9 April,2012
 * //
 * //  This library is free software; you can redistribute it and/or
 * //  modify it under the terms of the GNU Lesser General Public
 * //  License as published by the Free Software Foundation; either
 * //  version 2.1 of the License, or (at your option) any later version.
 * //
 * //  This library is distributed in the hope that it will be useful,
 * //  but WITHOUT ANY WARRANTY; without even the implied warranty of
 * //  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * //  Lesser General Public License for more details.
 * //
 * //  You should have received a copy of the GNU Lesser General Public
 * //  License along with this library; if not, write to the Free Software
 * //  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

/* Includes ------------------------------------------------------------------*/
#include "Me7SegmentDisplay.h"

/* Private variables ---------------------------------------------------------*/
const uint8_t TubeTab[] PROGMEM = 
{
  0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, //0-9
  0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71,                         //'A', 'B', 'C', 'D', 'E', 'F',
  0xbf, 0x86, 0xdb, 0xcf, 0xe6, 0xed, 0xfd, 0x87, 0xff, 0xef, //0.-9.
  0xf7, 0xfc, 0xb9, 0xde, 0xf9, 0xf1,                         //'A.', 'B.', 'C.', 'D.', 'E.', 'F.',
  0, 0x40                                                     //' ','-'
};

#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the 7-Segment display to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
Me7SegmentDisplay::Me7SegmentDisplay(void) : MePort()
{

}

/**
 * Alternate Constructor which can call your own function to map the 7-Segment display to arduino port,
 * the slot1 will be used for data pin and slot2 will used for clk pin.
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
Me7SegmentDisplay::Me7SegmentDisplay(uint8_t port) : MePort(port)
{
  _dataPin = s1;
  _clkPin = s2;
  pinMode(_clkPin, OUTPUT);
  pinMode(_dataPin, OUTPUT);
  set();
  clearDisplay();
}
#else // ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the 7-Segment display to arduino port. 
 * \param[in]
 *    dataPin - The DATA pin for Seven-Segment LED module(arduino port).
 * \param[in]
 *    clkPin - The CLK pin for Seven-Segment LED module(arduino port).
 */
Me7SegmentDisplay::Me7SegmentDisplay(uint8_t dataPin, uint8_t clkPin)
{
  _dataPin = dataPin;
  _clkPin = clkPin;
  pinMode(_clkPin, OUTPUT);
  pinMode(_dataPin, OUTPUT);
  set();
  clearDisplay();
}
#endif // ME_PORT_DEFINED

#ifdef ME_PORT_DEFINED

/**
 * \par Function
 *    reset
 * \par Description
 *    Reset the port for Seven-Segment LED module, the prot here is RJ25 port from PORT_1 to M2.
 * \param[in]
 *    port - RJ25 port from PORT_1 to M2
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    Set global variable _dataPin and _clkPin
 */
void Me7SegmentDisplay::reset(uint8_t port)
{
  _port = port;
  s2    = mePort[port].s2;
  s1    = mePort[port].s1;
  _clkPin = s2;
  _dataPin = s1;
  pinMode(_clkPin, OUTPUT);
  pinMode(_dataPin, OUTPUT);
  set();
  clearDisplay();
}
#endif // ME_PORT_DEFINED

/**
 * \par Function
 *    setpin
 * \par Description
 *    Reset Seven-Segment LED module's data pin and its clock pin then clear display.
 * \param[in]
 *    dataPin - The DATA pin for Seven-Segment LED module(arduino port).
 * \param[in]
 *    clkPin - The CLK pin for Seven-Segment LED module(arduino port).
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    Set global variable _KeyPin and _clkPin
 */
void Me7SegmentDisplay::setpin(uint8_t dataPin, uint8_t clkPin)
{
  _dataPin = dataPin;
  _clkPin = clkPin;
  pinMode(_clkPin, OUTPUT);
  pinMode(_dataPin, OUTPUT);
#ifdef ME_PORT_DEFINED
  s1 = dataPin;
  s2 = clkPin;
#endif // ME_PORT_DEFINED
}

/**
 * \Function
 *    clearDisplay
 * \Description
 *    Clear display.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 */
void Me7SegmentDisplay::clearDisplay(void)
{
  uint8_t buf[4] = { ' ', ' ', ' ', ' ' };
  display(buf);
}

/**
 * \par Function
 *    init
 * \par Description
 *    Initialization the display, here just call function clearDisplay.
 * \par Output  
 *    None
 * \return
 *    None
 * \others
 *    None
 */
void Me7SegmentDisplay::init(void)
{
  clearDisplay();
}

/**
 * \par Function
 *    writeByte
 * \par Description
 *    Write one byte to TM1637.
 * \param[in]
 *    wr_data - Data to write to module.
 * \par Output
 *    None
 * \return
 *    None
 * \others
 *    None
 */
void Me7SegmentDisplay::writeByte(uint8_t wr_data)
{
  uint8_t i;
  uint8_t cnt0;
  for (i = 0; i < 8; i++)  //sent 8bit data
  {
    digitalWrite(_clkPin, LOW);
    if (wr_data & 0x01)
    {
      digitalWrite(_dataPin, HIGH); //LSB first
    }
    else
    {
      digitalWrite(_dataPin, LOW);
    }
    wr_data >>= 1;
    digitalWrite(_clkPin, HIGH);
  }
  digitalWrite(_clkPin, LOW); //wait for ACK
  digitalWrite(_dataPin, HIGH);
  digitalWrite(_clkPin, HIGH);
  pinMode(_dataPin, INPUT);
  while (digitalRead(_dataPin))
  {
    cnt0 += 1;
    if (cnt0 == 200)
    {
      pinMode(_dataPin, OUTPUT);
      digitalWrite(_dataPin, LOW);
      cnt0 = 0;
    }
    //pinMode(_dataPin,INPUT);
  }
  pinMode(_dataPin, OUTPUT);

}

/**
 * \par Function
 *    start
 * \par Description
 *    Send start signal to TM1637
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \others
 *    None
 */
void Me7SegmentDisplay::start(void)
{
  digitalWrite(_clkPin, HIGH); //send start signal to TM1637
  digitalWrite(_dataPin, HIGH);
  digitalWrite(_dataPin, LOW);
  digitalWrite(_clkPin, LOW);
}

/**
 * \par Function
 *    stop
 * \par Description
 *    Send the stop signal to TM1637.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \others
 *    None
 */
void Me7SegmentDisplay::stop(void)
{
  digitalWrite(_clkPin, LOW);
  digitalWrite(_dataPin, LOW);
  digitalWrite(_clkPin, HIGH);
  digitalWrite(_dataPin, HIGH);
}

/**
 * \par Function
 *    write
 * \par Description
 *    White data array to certain address.
 * \param[in]
 *    SegData[] - Data array to write to module.
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void Me7SegmentDisplay::write(uint8_t SegData[])
{
  uint8_t i;
  start();    // Start signal sent to TM1637 from MCU.
  writeByte(ADDR_AUTO);
  stop();
  start();
  writeByte(Cmd_SetAddr);
  for (i = 0; i < 4; i++)
  {
    writeByte(SegData[i]);
  }
  stop();
  start();
  writeByte(Cmd_DispCtrl);
  stop();
}

/**
 * \par Function
 *    write
 * \par Description
 *    Write data to certain address.
 * \param[in]
 *    BitAddr - Bit address of data.
 * \param[in]
 *    SegData - Data to display.
 * \par Output
 *    None
 * \return
 *    None
 * \others
 *    None
 */
void Me7SegmentDisplay::write(uint8_t BitAddr, uint8_t SegData)
{
  start();    // start signal sent to TM1637 from MCU
  writeByte(ADDR_FIXED);
  stop();
  start();
  writeByte(BitAddr | STARTADDR);
  writeByte(SegData);
  stop();
  start();
  writeByte(Cmd_DispCtrl);
  stop();
}

/**
 * \par Function
 *    display
 * \par Description
 *    Display certain value, and this value type is uint16_t
 * \param[in]
 *    value - Value to display.
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void Me7SegmentDisplay::display(uint16_t value)
{
  display((int16_t)value);
}

/**
 * \par Function
 *    display
 * \par Description
 *    Display certain value, and this value type is int16_t
 * \param[in]
 *    value - Value to display.
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void Me7SegmentDisplay::display(int16_t value)
{
  display((double)value, 0);
}

/**
 * \par Function
 *    display
 * \par Description
 *    Display certain value, and this value type is float
 * \param[in]
 *    value - Value to display.
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void Me7SegmentDisplay::display(float value)
{
  uint8_t i=0;
  bool isStart = false;
  uint8_t index = 0;
  uint8_t disp[]={0,0,0,0};
  bool isNeg = false;
  if((float)value<0)
  {
    isNeg = true;
    value = -value;
    disp[0] = 0x21;
    index++;
  }
  for(i=0;i<7;i++)
  {
    int n = checkNum(value,3-i);
    if(n>=1||i==3)
    {
      isStart=true;
    }
    if(isStart)
	{
      if(i==3)
	  {
        disp[index]=n+0x10;
      }
	  else
	  {
        disp[index]=n;
      }
      index++;
    }
    if(index>3)
    {
      break;
    }
  }
  display(disp);
}

/**
 * \par Function
 *    display
 * \par Description
 *    Display certain value, and this value type is long
 * \param[in]
 *    value - Value to display.
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void Me7SegmentDisplay::display(long value)
{
  display((double)value, 0);
}

/**
 * \par Function
 *    checkNum
 * \par Description
 *    Extraction values to be displayed of float data
 * \param[in]
 *    v - Value to display.
 * \param[in]
 *    b - Value to display.
 * \par Output
 *    None
 * \return
 *    The data removal of the decimal point
 * \par Others
 *    None
 */
int16_t Me7SegmentDisplay::checkNum(float v,int16_t b)
{
  if(b>=0)
  {
    return floor((v-floor(v/pow(10,b+1))*(pow(10,b+1)))/pow(10,b));
  }
  else
  {
    b=-b;
    int i=0;
    for(i=0;i<b;i++)
    {
      v = v*10;
    }
    return ((int)(v)%10);
  }
}

/**
 * \par Function
 *    display
 * \par Description
 *    Display double number.
 * \param[in]
 *    value - Value to display.
 * \param[in]
 *    digits - Number of digits to display.
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void Me7SegmentDisplay::display(double value, uint8_t digits)
{
Posotion_1:
  uint8_t buf[4] = { ' ', ' ', ' ', ' ' };
  uint8_t tempBuf[4];
  uint8_t b = 0;
  uint8_t bit_num = 0;
  uint8_t int_num = 0;
  uint8_t isNeg = 0;
  double number = value;
  if (number >= 9999.5 || number <= -999.5)
  {
    buf[0] = ' ';
    buf[1] = ' ';
    buf[2] = ' ';
    buf[3] = 0x0e;
  }
  else
  {
    // Handle negative numbers
    if (number < 0.0)
    {
      number = -number;
      isNeg = 1;
    }
    // Round correctly so that print(1.999, 2) prints as "2.00"
    double rounding = 0.5;
    for (uint8_t i = 0; i < digits; ++i)
    {
      rounding /= 10.0;
    }
    number += rounding;

    // Extract the integer part of the number and print it
    uint16_t int_part = (uint16_t)number;
    double remainder = number - (double)int_part;
    do
    {
      uint16_t m = int_part;
      int_part /= 10;
      int8_t c = m - 10 * int_part;
      tempBuf[int_num] = c;
      int_num++;
    }
    while (int_part);

    bit_num = isNeg + int_num + digits;

    if (bit_num > 4)
    {
      bit_num = 4;
      digits = 4 - (isNeg + int_num);
      goto Posotion_1;
    }
    b = 4 - bit_num;
    if (isNeg)
    {
      buf[b++] = 0x21; // '-' display minus sign
    }
    for (uint8_t i = int_num; i > 0; i--)
    {
      buf[b++] = tempBuf[i - 1];
    }
    // Print the decimal point, but only if there are digits beyond
    if (digits > 0)
    {
      buf[b - 1] += 0x10;  // display '.'
      // Extract digits from the remainder one at a time
      while (digits-- > 0)
      {
        remainder *= 10.0;
        int16_t toPrint = int16_t(remainder);
        buf[b++] = toPrint;
        remainder -= toPrint;
      }
    }
  }
  display(buf);
}

/**
 * \par Function
 *    display
 * \par Description
 *    Display 8 bit number array.
 * \param[in]
 *    DispData[] - The data that needs to be displayed store in this array.
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void Me7SegmentDisplay::display(uint8_t DispData[])
{
  uint8_t SegData[4];
  uint8_t i;
  for (i = 0; i < 4; i++)
  {
    SegData[i] = DispData[i];
  }
  coding(SegData);
  write(SegData);
}

/**
 * \par Function
 *    display
 * \par Description
 *    Display data to certain digit.
 * \param[in]
 *    BitAddr - Address to display.
 * \param[in]
 *    DispData - Data to display.
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void Me7SegmentDisplay::display(uint8_t BitAddr, uint8_t DispData)
{
  uint8_t SegData;

  if ((DispData >= 'A' && DispData <= 'F'))
  {
    DispData = DispData - 'A' + 10;
  }
  else if ((DispData >= 'a' && DispData <= 'f'))
  {
    DispData = DispData - 'a' + 10;
  }
  SegData = coding(DispData);
  write(BitAddr, SegData);
}

/**
 * \par Function
 *    display
 * \par Description
 *    Display data to certain digit.
 * \param[in]
 *    BitAddr - Address to display.
 * \param[in]
 *    DispData - Data to display.
 * \param[in]
 *    point_on - Display the point or not.
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void Me7SegmentDisplay::display(uint8_t BitAddr, uint8_t DispData, uint8_t point_on)
{
  uint8_t SegData;

  if ((DispData >= 'A' && DispData <= 'F'))
  {
    DispData = DispData - 'A' + 10;
  }
  else if ((DispData >= 'a' && DispData <= 'f'))
  {
    DispData = DispData - 'a' + 10;
  }
  if(point_on == POINT_ON )
  {
    SegData = coding(DispData+0x10);
  }
  else
  {
    SegData = coding(DispData);
  }
  write(BitAddr, SegData);
}
/**
 * \par Function
 *    set
 * \par Description
 *    Set brightness, data and address.
 * \param[in]
 *    brightness - Brightness, defined in Me7SegmentDisplay.h from BRIGHTNESS_0 to BRIGHTNESS_7.
 * \param[in]
 *    SetData - First address for display data
 * \param[in]
 *    SetAddr - First address for display
 * \par Output
 *    Cmd_SetData - Private variable Cmd_SetData of class Me7SegmentDisplay.
 *    Cmd_SetAddr - Private variable Cmd_SetAddr of class Me7SegmentDisplay.
 *    Cmd_DispCtrl - Control command for Me 7 Segment Serial Display module.
 * \return
 *    None
 * \par Others
 *    None
 */
void Me7SegmentDisplay::set(uint8_t brightness, uint8_t SetData, uint8_t SetAddr)
{
  Cmd_SetData = SetData;
  Cmd_SetAddr = SetAddr;
  Cmd_DispCtrl = SEGDIS_ON + brightness;//Set brightness, take effect next display cycle.
}

/**
 * \par Function
 *    setBrightness
 * \par Description
 *    Set brightness.
 * \param[in]
 *    brightness - Brightness, defined in Me7SegmentDisplay.h from BRIGHTNESS_0 to BRIGHTNESS_7.
 * \par Output
 *    Cmd_DispCtrl - Control command for Me 7 Segment Serial Display module.
 * \return
 *    None
 * \par Others
 *    None
 */
void Me7SegmentDisplay::setBrightness(uint8_t brightness)
{
  Cmd_DispCtrl = SEGDIS_ON + brightness;
}

/**
 * \par Function
 *    coding
 * \par Description
 *    Set display data using look up table.
 * \param[in]
 *    DispData[] - DataArray to display.
 * \par Output
 *    DispData[] - DataArray be transcoded.
 * \return
 *    None
 * \par Others
 *    None
 */
void Me7SegmentDisplay::coding(uint8_t DispData[])
{
  for (uint8_t i = 0; i < 4; i++)
  {
    if (DispData[i] >= sizeof(TubeTab) / sizeof(*TubeTab))
    {
      DispData[i] = 32; // Change to ' '(space)
    }
    //DispData[i] = TubeTab[DispData[i]];
    DispData[i] = pgm_read_byte(&TubeTab[DispData[i]]);//+ PointData;
  }
}

/**
 * \par Function
 *    coding
 * \par Description
 *    Return display data from look up table.
 * \param[in]
 *    DispData - Data need be transcoded.
 * \par Output
 *    None
 * \return
 *    Return the value of transcoding
 * \par Others
 *    None
 */
uint8_t Me7SegmentDisplay::coding(uint8_t DispData)
{
  if (DispData >= sizeof(TubeTab) / sizeof(*TubeTab))
  {
    DispData = 32; // Change to ' '(space)
  }
  //DispData = TubeTab[DispData];//+ PointData;
  DispData = pgm_read_byte(&TubeTab[DispData]);//+ PointData;
  return DispData;
}
