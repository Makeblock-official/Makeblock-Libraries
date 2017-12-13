/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class MeVoice
 * \brief   Driver for Me voice device.
 * @file    MeVoice.h
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/09
 * @brief   Header for for MeVoice.cpp module
 * \par Description
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
 * Description: this file is a drive for Me voice device, It supports
   the voice device provided by the MakeBlock company.
 *
 * \par Method List:
 *
 *    1. void MeVoice::begin(long speed)
 *    2. int16_t MeVoice::read(void)
 *    3. bool MeVoice::buttonState(void)
 *    4. uint8_t MeVoice::getCode(void)
 *    5. void MeVoice::loop(void)
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/12/15     1.0.0            Bulid the new .
 * </pre>
 *
 */

#ifndef MeVoice_H
#define MeVoice_H

#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>
#include "MeConfig.h"
#include "MeSerial.h"

/* NEC Code table */
#define CMD_FORWARD     (11)
#define CMD_BACK        (13)
#define CMD_LEFT        (7)
#define CMD_RIGHT       (9)
#define CMD_DIS_ON      (3)
#define CMD_DIS_OFF     (5)

#ifdef ME_PORT_DEFINED
#include "MePort.h"
#endif /* ME_PORT_DEFINED */

/**
 * Class: MeVoice
 * \par Description
 * Declaration of Class MeVoice
 */
#ifndef ME_PORT_DEFINED
class MeVoice
#else // !ME_PORT_DEFINED
class MeVoice : public MeSerial
#endif // !ME_PORT_DEFINED
{
public:
#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the Voice device to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
  MeVoice(void);

/**
 * Alternate Constructor which can call your own function to map the Voice device to arduino port,
 * If the hardware serial was selected, we will used the hardware serial.
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
  MeVoice(uint8_t port);
#else // ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the Voice device to arduino port,
 * If the hardware serial was selected, we will used the hardware serial.
 * \param[in]
 *   receivePin - the rx pin of serial(arduino port)
 * \param[in]
 *   keycheckpin - the pin used for check the pin pressed state(arduino port)
 * \param[in]
 *   inverse_logic - Whether the Serial level need inv.
 */
  MeVoice(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic);
#endif // ME_PORT_DEFINED

/**
 * \par Function
 *   begin
 * \par Description
 *   Sets the speed (baud rate) for the serial communication. Supported baud 
 *   rates is 57600 bps
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void begin(long = 9600);

/**
 * \par Function
 *   read
 * \par Description
 *   Return a character that was received on the RX pin of the software serial port. 
 *   Note that only one SoftwareSerial instance can receive incoming data at a time 
 *  (select which one with the listen() function).
 * \par Output
 *   None
 * \return
 *   The character read, or -1 if none is available
 * \par Others
 *   None
 */
  int16_t read(void);

/**
 * \par Function
 *   getCode
 * \par Description
 *   Get the button code
 * \par Output
 *   None
 * \return
 *   Return the button code
 * \par Others
 *   None
 */
  uint8_t getCode(void);

/**
 * \par Function
 *   loop
 * \par Description
 *   This function used with getCode, it should called in the main loop
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   This function used with getCode
 */
  void loop(void);
private:
  static volatile uint8_t _RxPin;
  static volatile uint8_t _TxPin;
  static volatile unsigned long last_time;
  uint8_t _irCode;
};

#endif

