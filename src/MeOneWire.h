/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class MeOneWire
 * \brief   Driver for 1-wire protocol.
 * @file    MeOneWire.h
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/04
 * @brief   Header for MeOneWire.cpp module
 *
 * \par Copyright
 * This software is Copyright (C), 2012-2016, MakeBlock. Use is subject to license \n
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
 * This file is a drive for 1-wire protocol, 1-Wire communication protocol
 * was developed by Dallas Semiconductor owned by Maxim. This protocol allows
 * communication of multiple chips to one host with minimal pin count.
 * The protocol is called 1-Wire because it uses 1 wire to transfer data.
 * 1-Wire architecture uses pull up resistor to pull voltage of data line at
 * master side.
 *
 * \par Method List:
 *
 *    1. bool  MeOneWire::readIO(void)
 *    2. void MeOneWire::reset(uint8_t pin)
 *    3. uint8_t MeOneWire::reset(void)
 *    4. void MeOneWire::select(const uint8_t rom[8])
 *    5. void MeOneWire::skip(void)()
 *    6. void MeOneWire::write(uint8_t v, uint8_t power)
 *    7. void MeOneWire::write_bytes(const uint8_t *buf, uint16_t count, bool power)
 *    8. uint8_t MeOneWire::read(void)
 *    9. void MeOneWire::read_bytes(uint8_t *buf, uint16_t count)
 *    10. void MeOneWire::write_bit(uint8_t v)
 *    11. uint8_t MeOneWire::read_bit(void)
 *    12. void MeOneWire::depower(void)
 *    13. void MeOneWire::reset_search(void)
 *    14. void MeOneWire::target_search(uint8_t family_code)
 *    15. uint8_t MeOneWire::search(uint8_t *newAddr)
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/09/04     1.0.0            Rebuild the old lib.
 * </pre>
 *
 * DERIVED FROM
 * Copyright (c) 2007, Jim Studt  (original old version - many contributors since)
 * The latest version of this library may be found at:
 *   http://www.pjrc.com/teensy/td_libs_OneWire.html
 * Version 2.1:
 *   Arduino 1.0 compatibility, Paul Stoffregen
 *   Improve temperature example, Paul Stoffregen
 *   DS250x_PROM example, Guillermo Lovato
 *   PIC32 (chipKit) compatibility, Jason Dangel, dangel.jason AT gmail.com
 *   Improvements from Glenn Trewitt:
 *   - crc16() now works
 *   - check_crc16() does all of calculation/checking work.
 *   - Added read_bytes() and write_bytes(), to reduce tedious loops.
 *   - Added ds2408 example.
 *   Delete very old, out-of-date readme file (info is here)
 * Version 2.0: Modifications by Paul Stoffregen, January 2010:
 * http://www.pjrc.com/teensy/td_libs_OneWire.html
 *   Search fix from Robin James
 *     http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1238032295/27#27
 *   Use direct optimized I/O in all cases
 *   Disable interrupts during timing critical sections
 *     (this solves many random communication errors)
 *   Disable interrupts during read-modify-write I/O
 *   Reduce RAM consumption by eliminating unnecessary
 *     variables and trimming many to 8 bits
 *   Optimize both crc8 - table version moved to flash
 * Modified to work with larger numbers of devices - avoids loop.
 * Tested in Arduino 11 alpha with 12 sensors.
 * 26 Sept 2008 -- Robin James
 * http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1238032295/27#27
 * Updated to work with arduino-0008 and to include skip() as of
 * 2007/07/06. --RJL20
 * Modified to calculate the 8-bit CRC directly, avoiding the need for
 * the 256-byte lookup table to be loaded in RAM.  Tested in arduino-0010
 * -- Tom Pollard, Jan 23, 2008
 * Jim Studt's original library was modified by Josh Larios.
 * Tom Pollard, pollard@alum.mit.edu, contributed around May 20, 2008
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * Much of the code was inspired by Derek Yerger's code, though I don't
 * think much of that remains.  In any event that was..
 *     (copyleft) 2006 by Derek Yerger - Free to distribute freely.
 * The CRC code was excerpted and inspired by the Dallas Semiconductor
 * sample code bearing this copyright.
 * //---------------------------------------------------------------------------
 * // Copyright (C) 2000 Dallas Semiconductor Corporation, All Rights Reserved.
 * //
 * // Permission is hereby granted, free of charge, to any person obtaining a
 * // copy of this software and associated documentation files (the "Software"),
 * // to deal in the Software without restriction, including without limitation
 * // the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * // and/or sell copies of the Software, and to permit persons to whom the
 * // Software is furnished to do so, subject to the following conditions:
 * //
 * // The above copyright notice and this permission notice shall be included
 * // in all copies or substantial portions of the Software.
 * //
 * // THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * // OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * // MERCHANTABILITY,  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * // IN NO EVENT SHALL DALLAS SEMICONDUCTOR BE LIABLE FOR ANY CLAIM, DAMAGES
 * // OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * // ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * // OTHER DEALINGS IN THE SOFTWARE.
 * //
 * // Except as contained in this notice, the name of Dallas Semiconductor
 * // shall not be used except as stated in the Dallas Semiconductor
 * // Branding Policy.
 * //--------------------------------------------------------------------------
 */
#ifndef MeOneWire_H
#define MeOneWire_H

#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>
#include "MeConfig.h"

/**
 * Class: MeOneWire
 * \par Description
 * Declaration of Class MeOneWire.
 */
class MeOneWire
{
private:
  MeIO_REG_TYPE   bitmask;
  volatile MeIO_REG_TYPE *baseReg;
  /* global search state */
  uint8_t ROM_NO[8];
  uint8_t LastDiscrepancy;
  uint8_t LastFamilyDiscrepancy;
  uint8_t LastDeviceFlag;

public:
/**
 * Alternate Constructor which can call your own function to map the MeOneWire to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
  MeOneWire(void);

/**
 * Alternate Constructor which can call your own function to map the MeOneWire to arduino port,
 * it will assigned the shot PIN and focus pin.
 * \param[in]
 *   pin - arduino port for 1-wire(should digital pin)
 */
  MeOneWire(uint8_t pin);

/**
 * \par Function
 *   readIO
 * \par Description
 *   Read a bit from 1-wire data port
 * \par Output
 *   None 
 * \return
 *   return the bit value we read
 * \par Others
 *   None
 */
  bool readIO(void);

/**
 * \par Function
 *   reset
 * \par Description
 *   Reset the available PIN for 1-wire
 * \param[in]
 *   pin - arduino port(should digital pin)
 * \par Output
 *   None 
 * \return
 *   None
 * \par Others
 *   None
 */
  void reset(uint8_t pin);

/**
 * \par Function
 *   reset
 * \par Description
 *   Perform the MeOneWire reset function.  We will wait up to 250uS for
 *   the bus to come high, if it doesn't then it is broken or shorted.
 * \param[in]
 *   v - The bit value need be written
 * \par Output
 *   None 
 * \return
 *   Returns 1 if a device asserted a presence pulse, 0 otherwise.
 * \par Others
 *   None
 */
  uint8_t reset(void);

/**
 * \par Function
 *   select
 * \par Description
 *   Issue a 1-Wire rom select command.
 * \param[in]
 *   rom[8] - 64bit ROM code.
 * \par Output
 *   None 
 * \return
 *   None
 * \par Others
 *   None
 */
  void select(const uint8_t rom[8]);

/**
 * \par Function
 *   skip
 * \par Description
 *   Issue a 1-Wire rom skip command.
 * \par Output
 *   None 
 * \return
 *   None
 * \par Others
 *   None
 */
  void skip(void);

/**
 * \par Function
 *   write
 * \par Description
 *   Write a byte of data
 * \param[in]
 *   v - The value need be written
 * \param[in]
 *   power - Should we need active drivers to raise the pin high
 * \par Output
 *   None 
 * \return
 *   None
 * \par Others
 *   None
 */
  void write(uint8_t v, uint8_t power = 0);

/**
 * \par Function
 *   write_bytes
 * \par Description
 *   Write certain number of data
 * \param[in]
 *   buf - The buffer used to store the data that need be written
 * \param[in]
 *   count - The count of the bytes we need to write
 * \param[in]
 *   power - Should we need active drivers to raise the pin high
 * \par Output
 *   None 
 * \return
 *   None
 * \par Others
 *   None
 */
  void write_bytes(const uint8_t *buf, uint16_t count, bool power = 0);

/**
 * \par Function
 *   read
 * \par Description
 *   Read a byte of data
 * \par Output
 *   None 
 * \return
 *   The read data(8-bit data)
 * \par Others
 *   None
 */
  uint8_t read(void);

/**
 * \par Function
 *   read_bytes
 * \par Description
 *   Read certain number of data
 * \param[out]
 *   buf - The buffer used to store the read data
 * \param[in]
 *   count - The count of the bytes we need to read
 * \par Output
 *   None 
 * \return
 *   None
 * \par Others
 *   None
 */
  void read_bytes(uint8_t *buf, uint16_t count);

/**
 * \par Function
 *   write_bit
 * \par Description
 *   Write a bit. The bus is always left powered at the end, see
 *   note in write() about that.
 * \param[in]
 *   v - The bit value need be written
 * \par Output
 *   None 
 * \return
 *   None
 * \par Others
 *   None
 */
  void write_bit(uint8_t v);

/**
 * \par Function
 *   read_bit
 * \par Description
 *   Read a bit. Port and bit is used to cut lookup time and provide
 *   more certain timing
 * \par Output
 *   None 
 * \return
 *   return the bit value we read
 * \par Others
 *   None
 */
  uint8_t read_bit(void);

/**
 * \par Function
 *   depower
 * \par Description
 *   Stop forcing power onto the bus. You only need to do this if
 *   you used the 'power' flag to write() or used a write_bit() call
 *   and aren't about to do another read or write. You would rather
 *   not leave this powered if you don't have to, just in case
 *   someone shorts your bus.
 * \par Output
 *   None 
 * \return
 *   None
 * \par Others
 *   None
 */
  void depower(void);

/**
 * \par Function
 *   reset_search
 * \par Description
 *   If we need search a new device, we need call this function to clear 
 *   the search state and all stored values  
 * \par Output
 *   Reset all stored values 
 * \return
 *   None
 * \par Others
 *   None
 */
  void reset_search(void);

/**
 * \par Function
 *   target_search
 * \par Description
 *   Setup the search to find the device type 'family_code' on the next call
 *   of search(*newAddr) if it is present.
 * \param[in]
 *   family_code - the device type we need search
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void target_search(uint8_t family_code);

/**
 * \par Function
 *   search
 * \par Description
 *   Perform a search. If this function returns a '1' then it has
 *   enumerated the next device and you may retrieve the ROM from the
 *   MeOneWire::address variable. If there are no devices, no further
 *   devices, or something horrible happens in the middle of the
 *   enumeration then a '0' is returned. If a new device is found then
 *   its address is copied to newAddr.  Use MeOneWire::reset_search() to
 *   start over. you can get the algorithm from
 *   doc\Me_Temperature\Datasheet\1-Wire_Search_Algorithm.pdf
 * \param[out]
 *   newAddr - The adrress to store the ROM data 
 * \par Output
 *   None
 * \return
 *   true - device found, ROM number in ROM_NO buffer\r\n
 *   false - device not found, end of search
 * \par Others
 *   None
 */
  uint8_t search(uint8_t *newAddr);
};

#endif

