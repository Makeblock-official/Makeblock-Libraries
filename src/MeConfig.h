/**
 * \mainpage Makeblock library for Arduino
 *
 * \par Description
 *
 * This is the library provided by makeblock. \n
 * It provides drivers for all makeblock RJ25 jack interface modules. \n
 *
 * The latest version of this documentation can see from here \n
 * http://learn.makeblock.cc/Makeblock-library-for-Arduino/index.html
 *
 * Package can be download from https://codeload.github.com/Makeblock-official/Makeblock-Libraries/zip/master \n
 * If you are familiar with git, you also can clone it from https://github.com/Makeblock-official/Makeblock-Libraries
 *
 * \par Installation
 *
 * Install the package in the normal way: unzip the distribution zip file to the libraries \n
 * sub-folder of your sketchbook or Arduino, \n
 * copy files in makeblock/src folder to arduino/libraries/Makeblock/
 *
 * \par Donations
 *
 * This library is offered under GPLv2 license for those who want to use it that way. \n
 * Additional information can be found at http://www.gnu.org/licenses/old-licenses/gpl-2.0.html \n
 * We are tring hard to keep it up to date, fix bugs free and to provide free support on our site. \n
 *
 * \par Copyright
 *
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
 * \par History:
 * <pre>
 * Author           Time           Version          Descr
 * Mark Yan         2015/07/24     3.0.0            Rebuild the old lib.
 * Rafael Lee       2015/09/02     3.1.0            Added some comments and macros.
 * Lawrence         2015/09/09     3.2.0            Include some Arduino's official headfiles which path specified.
 * Mark Yan         2015/11/02     3.2.1            fix bug on MACOS.
 * Mark Yan         2016/01/21     3.2.2            fix some library bugs.
 * Mark Yan         2016/05/17     3.2.3            add support for MegaPi and Auriga Board.
 * Mark Yan         2016/07/27     3.2.4            fix some JIRA issue, add PID motion for Megapi/Auriga on board encoder motor
 * </pre>
 *
 * \author  Mark Yan (myan@makeblock.com)
 */


/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \brief   Configuration file of library.
 * \file    Meconfig.h
 * @author  MakeBlock
 * @version V1.0.4
 * @date    2015/11/03
 * @brief   Configuration file of library.
 * \par Copyright
 *
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
 * Define macro ME_PORT_DEFINED. \n
 * Define other macros if `__AVR__` defined.
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/07/24         1.0.0            Rebuild the old lib.
 * Rafael Lee       2015/09/02         1.0.1            Added some comments and macros. Fixed some bug and add some methods.
 * Lawrence         2015/09/09         1.0.2            Include some Arduino's official headfiles which path specified.
 * Mark Yan         2015/11/02         1.0.3            fix bug on MACOS.
 * Mark Yan         2015/11/03         1.0.4            fix Segmentation symbols error for file path.
 * </pre>
 */


#ifndef MeConfig_H
#define MeConfig_H

#include <utility/Servo.h>
#include <utility/Wire.h>
#include <utility/EEPROM.h>
#include <utility/SoftwareSerial.h>
#include <utility/SPI.h>

#define ME_PORT_DEFINED

#if defined(__AVR__)
#define MePIN_TO_BASEREG(pin)               ( portInputRegister (digitalPinToPort (pin) ) )
#define MePIN_TO_BITMASK(pin)               ( digitalPinToBitMask (pin) )
#define MeIO_REG_TYPE                       uint8_t
#define MeIO_REG_ASM                        asm ("r30")
#define MeDIRECT_READ(base, mask)           ( ( (*(base) ) & (mask) ) ? 1 : 0)
#define MeDIRECT_MODE_INPUT(base, mask)     ( (*( (base) + 1) ) &= ~(mask) ), ( (*( (base) + 2) ) |= (mask) ) // INPUT_PULLUP
#define MeDIRECT_MODE_OUTPUT(base, mask)    ( (*( (base) + 1) ) |= (mask) )
#define MeDIRECT_WRITE_LOW(base, mask)      ( (*( (base) + 2) ) &= ~(mask) )
#define MeDIRECT_WRITE_HIGH(base, mask)     ( (*( (base) + 2) ) |= (mask) )
#endif // __AVR__

#endif // MeConfig_H

