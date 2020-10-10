/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class MeVoice
 * \brief   Driver for Me voice device.
 * @file    MeVoice.cpp
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/12/15
 * @brief   Driver for Me voice device.
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
 * Description: this file is a drive for Me voice device, It supports
   The voice device provided by the MakeBlock company.
 *
 * \par Method List:
 *
 *    1. void MeVoice::begin(long speed)
 *    2. int16_t MeVoice::read(void)
 *    3. uint8_t MeVoice::getCode(void)
 *    4. void MeVoice::loop(void)
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/12/15     1.0.0            Build the new.
 * </pre>
 *
 * @example MeVoiceTest.ino
 */
#include "MeVoice.h"

volatile uint8_t MeVoice::_RxPin = 0;
volatile uint8_t MeVoice::_TxPin = 0;
volatile unsigned long MeVoice::last_time = 0;

#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the Voice device to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
MeVoice::MeVoice(void) : MeSerial(0)
{
}

/**
 * Alternate Constructor which can call your own function to map the Voice device to arduino port,
 * If the hardware serial was selected, we will used the hardware serial.
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
MeVoice::MeVoice(uint8_t port) : MeSerial(port)
{
  _RxPin = s2;
  _TxPin = s1;
}
#else // ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the Voice device to arduino port,
 * If the hardware serial was selected, we will used the hardware serial.
 * \param[in]
 *   rxPin - the rx pin of serial(arduino port)
 * \param[in]
 *   txpin - the pin used for check the pin pressed state(arduino port)
 * \param[in]
 *   inverse_logic - Whether the Serial level need inv.
 */
MeVoice::MeVoice(uint8_t rxPin, uint8_t txpin, bool inverse_logic)\
                        : MeSerial(rxPin, txpin, inverse_logic)
{
  _RxPin = rxPin;
  _TxPin = txpin;
}
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
void MeVoice::begin(long speed)
{
  MeSerial::begin(speed);
#ifdef ME_PORT_DEFINED
  pinMode(s2, INPUT);
  pinMode(s1, OUTPUT);
#else // ME_PORT_DEFINED
  pinMode(_RxPin, INPUT);
  pinMode(_TxPin, OUTPUT);
#endif // ME_PORT_DEFINED
}

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
int16_t MeVoice::read(void)
{
  int16_t val;
  val = MeSerial::read();     /* Read serial infrared data */
  val &= 0xff;
  return(val);
}

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
uint8_t MeVoice::getCode(void)
{
  return _irCode;
}

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
void MeVoice::loop(void)
{
  if(MeSerial::available() > 0)
  {
    last_time = millis();
    int r = read();
    if(r<0xff)
    {
      _irCode = r;
    }
  }
  else if((millis() - last_time) > 500)
  {
    _irCode = 0;
    last_time = millis();
  }
}

