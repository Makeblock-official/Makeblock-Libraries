/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   Me7SegmentDisplay
 * \brief   Driver for Me 7-Segment Serial Display module.
 * @file    Me7SegmentDisplay.h
 * @author  Frankie.Chu, MakeBlock
 * @version V1.0.5
 * @date    2016/07/27
 * @brief   Header file for Me7SegmentDisplay.cpp.
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
 * Rafael Lee       2015/09/02     1.0.1            Added some comments and macros.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef Me7SegmentDisplay_H
#define Me7SegmentDisplay_H

//************definitions for TM1637*********************
#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>
#include "MeConfig.h"

#ifdef ME_PORT_DEFINED
#include "MePort.h"
#endif // ME_PORT_DEFINED

/* Exported constants --------------------------------------------------------*/
/******************definitions for TM1637**********************/
const uint8_t   ADDR_AUTO = 0x40;   //Automatic address increment mode
const uint8_t   ADDR_FIXED = 0x44;   //Fixed address mode
const uint8_t   STARTADDR = 0xc0;   //start address of display register
const uint8_t   SEGDIS_ON = 0x88;   //diplay on
const uint8_t   SEGDIS_OFF = 0x80;   //diplay off
/**** definitions for the clock point of the digit tube *******/
const uint8_t POINT_ON = 1;
const uint8_t POINT_OFF = 0;
/**************definitions for brightness***********************/
const uint8_t BRIGHTNESS_0 = 0;
const uint8_t BRIGHTNESS_1 = 1;
const uint8_t BRIGHTNESS_2 = 2;
const uint8_t BRIGHTNESS_3 = 3;
const uint8_t BRIGHTNESS_4 = 4;
const uint8_t BRIGHTNESS_5 = 5;
const uint8_t BRIGHTNESS_6 = 6;
const uint8_t BRIGHTNESS_7 = 7;
///@brief Class for numeric display module

/**
 * Class: Me7SegmentDisplay
 * \par Description
 * Declaration of Class Me7SegmentDisplay.
 */
#ifndef ME_PORT_DEFINED
class Me7SegmentDisplay
#else // ME_PORT_DEFINED
class Me7SegmentDisplay :public MePort
#endif // ME_PORT_DEFINED
{
public:
#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the 7-Segment display to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
  Me7SegmentDisplay(void);
  
/**
 * Alternate Constructor which can call your own function to map the 7-Segment display to arduino port,
 * the slot1 will be used for data pin and slot2 will used for clk pin.
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
  Me7SegmentDisplay(uint8_t port);
#else // ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the 7-Segment display to arduino port. 
 * \param[in]
 *    dataPin - The DATA pin for Seven-Segment LED module(arduino port).
 * \param[in]
 *    clkPin - The CLK pin for Seven-Segment LED module(arduino port).
 */
  Me7SegmentDisplay(uint8_t dataPin, uint8_t clkPin);
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
  void reset(uint8_t port);
#endif // ME_PORT_DEFINED
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
  void init(void); // Clear display
  
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
  void set(uint8_t = BRIGHTNESS_2, uint8_t = ADDR_AUTO, uint8_t = STARTADDR);// Take effect next display cycle.
  
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
  void setpin(uint8_t dataPin, uint8_t clkPin);

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
  void write(uint8_t SegData[]);
  
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
  void write(uint8_t BitAddr, uint8_t SegData);
  
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
  void display(uint16_t value);
  
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
  void display(int16_t value);

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
  void display(float value);

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
  void display(long value);

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
  int16_t checkNum(float v,int16_t b);

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
  void display(double value, uint8_t = 1);

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
  void display(uint8_t DispData[]);

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
  void display(uint8_t BitAddr, uint8_t DispData);

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
  void display(uint8_t BitAddr, uint8_t DispData, uint8_t point_on);

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
 *    None
 */
  void clearDisplay(void);
  
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
  void setBrightness(uint8_t brightness);
  
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
  void coding(uint8_t DispData[]);

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
  uint8_t coding(uint8_t DispData);
private:
  uint8_t Cmd_SetData;
  uint8_t Cmd_SetAddr;
  uint8_t Cmd_DispCtrl;
  bool _PointFlag; //_PointFlag=1:the clock point on

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
  void writeByte(uint8_t wr_data);// Write 8 bits data to tm1637.
  
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
  void start(void);// Send start bits
  void point(bool PointFlag);// Whether to light the clock point ":". Take effect next display cycle.
 
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
  void stop(void); // Send stop bits.
  uint8_t _clkPin;
  uint8_t _dataPin;
};
#endif
