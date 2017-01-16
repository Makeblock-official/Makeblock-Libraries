/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class MePort
 * \brief   Port Mapping for RJ25
 * @file    MePort.h
 * @author  MakeBlock
 * @version V1.0.3
 * @date    2016/09/20
 * @brief   Header for MePort.cpp module
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
 * This file is a drive for MakeBlock rj25 port.
 *
 * \par Method List:
 *
 *    1. uint8_t MePort::getSlot()
 *    2. uint8_t MePort::getSlot()
 *    3. bool MePort::dRead1(uint8_t mode)
 *    4. bool MePort::dRead2(uint8_t mode)
 *    5. bool MePort::dpRead1(void)
 *    6. bool MePort::dpRead1(void)
 *    7. void MePort::dWrite1(bool value)
 *    8. void MePort::dWrite2(bool value)
 *    9. int16_t MePort::aRead1()
 *    10. int16_t MePort::aRead2()
 *    11. void MePort::aWrite1(int16_t value)
 *    12. void MePort::aWrite2(int16_t value)
 *    13. void MePort::reset(uint8_t port)
 *    14. void MePort::reset(uint8_t port, uint8_t slot)
 *    15. uint8_t MePort::pin1()
 *    16. uint8_t MePort::pin2()
 *    17. uint8_t MePort::pin()
 *    18. uint8_t MePort::pin(uint8_t port, uint8_t slot)
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`         `<Descr>`
 * Mark Yan         2015/09/01          1.0.0            Rebuild the old lib.
 * Lawrence         2015/09/09          1.0.1            Add a input parameter of function dRead1 and dRead2.
 * Scott wang       2016/09/18          1.0.2            Add the PORT[15].
 * Scott            2016/09/20          1.0.3            Add the PORT[16].
 * </pre>
 */
#ifndef MEPORT_H_
#define MEPORT_H_

#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include "MeConfig.h"

/**
 * A structure to represent MePort Signal.
 */
typedef struct
{
  uint8_t s1;
  uint8_t s2;
} MePort_Sig;

extern MePort_Sig mePort[17];  // mePort[0] is nonsense

#define NC (0)  //use UART RX for NULL port

#define PORT_1  (0x01)
#define PORT_2  (0x02)
#define PORT_3  (0x03)
#define PORT_4  (0x04)
#define PORT_5  (0x05)
#define PORT_6  (0x06)
#define PORT_7  (0x07)
#define PORT_8  (0x08)
#define PORT_9  (0x09)
#define PORT_10 (0x0a)
#define M1      (0x09)
#define M2      (0x0a)
#define PORT_11 (0x0b)
#define PORT_12 (0x0c)
#define PORT_13 (0x0d)
#define PORT_14 (0x0e)
#define PORT_15 (0x0f)
#define PORT_16 (0x10)

#ifdef MeMbot_H
#define PORT_RGB           (0x05)
#define PORT_LightSensor   (0x06)
#endif

#define SLOT1       (1)
#define SLOT2       (2)
#define SLOT3       (3)
#define SLOT4       (4)
#define SLOT_1  SLOT1
#define SLOT_2  SLOT2
#define SLOT_3  SLOT3
#define SLOT_4  SLOT4

#ifndef FALSE
#define FALSE   (0)
#endif

#ifndef TRUE
#define TRUE    (1)
#endif

/**
 * Class: MePort
 *
 * \par Description
 * Declaration of Class MePort
 */
class MePort
{
public:

/**
 * Alternate Constructor which can call your own function to map the MePort to arduino port,
 * no pins are used or initialized here
 */
  MePort(void);

/**
 * Alternate Constructor which can call your own function to map the MePort to arduino port,
 * no pins are used or initialized here, but PWM frequency set to 976 Hz
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
  MePort(uint8_t port);

/**
 * Alternate Constructor which can call your own function to map the MePort to arduino port,
 * no pins are used or initialized here, but PWM frequency set to 976 Hz
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 * \param[in]
 *   slot - SLOT1 or SLOT2
 */
  MePort(uint8_t port, uint8_t slot);

/**
 * \par Function
 *   getPort
 * \par Description
 *   Get current valid port of current RJ25 object
 * \par Output
 *   None
 * \return
 *   Port bumber from PORT_1 to M2
 * \par Others
 *   None
 */
  uint8_t getPort(void);

/**
 * \par Function
 *   getSlot
 * \par Description
 *   Get current valid slot of current RJ25 object's port
 * \par Output
 *   None
 * \return
 *   Slot bumber SLOT1 or SLOT2
 * \par Others
 *   None
 */
  uint8_t getSlot(void);

/**
 * \par Function
 *   dRead1
 * \par Description
 *   Read the digital input value on slot1 of current RJ25 object's port
 * \param[in]
 *   mode - digital input mode INPUT or INPUT_PULLUP
 * \par Output
 *   None
 * \return
 *   Digital input value
 * \par Others
 *   None
 */
  bool dRead1(uint8_t mode = INPUT);

/**
 * \par Function
 *   dRead2
 * \par Description
 *   Read the digital input value on slot2 of current RJ25 object's port
 * \param[in]
 *   mode - digital input mode INPUT or INPUT_PULLUP
 * \par Output
 *   None
 * \return
 *   Digital input value
 * \par Others
 *   None
 */
  bool dRead2(uint8_t mode = INPUT);

/**
 * \par Function
 *   dpRead1
 * \par Description
 *   Read the digital input value on slot1 of current RJ25 object's port, the input 
 *   mode set as INPUT_PULLUP.
 * \par Output
 *   None
 * \return
 *   Digital input value
 * \par Others
 *   None
 */
  bool dpRead1(void);

/**
 * \par Function
 *   dpRead2
 * \par Description
 *   Read the digital input value on slot2 of current RJ25 object's port, the input 
 *   mode set as INPUT_PULLUP.
 * \par Output
 *   None
 * \return
 *   Digital input value
 * \par Others
 *   None
 */
  bool dpRead2(void);

/**
 * \par Function
 *   dWrite1
 * \par Description
 *   Set the digital output value on slot1 of current RJ25 object's port
 * \param[in]
 *   value - digital output value HIGH or LOW
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void dWrite1(bool value);

/**
 * \par Function
 *   dWrite2
 * \par Description
 *   Set the digital output value on slot2 of current RJ25 object's port
 * \param[in]
 *   value - digital output value HIGH or LOW
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void dWrite2(bool value);

/**
 * \par Function
 *   aRead1
 * \par Description
 *   Read the analog value on slot1 of current RJ25 object's port
 * \par Output
 *   None
 * \return
 *   Analog value from 0-1023
 * \par Others
 *   None
 */
  int16_t aRead1(void);

/**
 * \par Function
 *   aRead2
 * \par Description
 *   Read the analog value on slot2 of current RJ25 object's port
 * \par Output
 *   None
 * \return
 *   Analog value from 0-1023
 * \par Others
 *   None
 */
  int16_t aRead2(void);

/**
 * \par Function
 *   aWrite1
 * \par Description
 *   Set the PWM output value on slot1 of current RJ25 object's port
 * \param[in]
 *   value - Analog value between 0 to 255
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void aWrite1(int16_t value);

/**
 * \par Function
 *   aWrite2
 * \par Description
 *   Set the PWM output value on slot2 of current RJ25 object's port
 * \param[in]
 *   value - Analog value between 0 to 255
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void aWrite2(int16_t value);

/**
 * \par Function
 *   reset
 * \par Description
 *   Reset the RJ25 available PIN by its port
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void reset(uint8_t port);

/**
 * \par Function
 *   reset
 * \par Description
 *   Reset the RJ25 available PIN by its port and slot
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 * \param[in]
 *   slot - SLOT1 or SLOT2
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void reset(uint8_t port, uint8_t slot);

/**
 * \par Function
 *   pin1
 * \par Description
 *   Return the arduino pin number of current RJ25 object's slot1
 * \par Output
 *   None
 * \return
 *   The PIN number of arduino
 * \par Others
 *   None
 */
  uint8_t pin1(void);

/**
 * \par Function
 *   pin2
 * \par Description
 *   Return the arduino pin number of current RJ25 object's slot2
 * \par Output
 *   None
 * \return
 *   The PIN number of arduino
 * \par Others
 *   None
 */
  uint8_t pin2(void);

/**
 * \par Function
 *   pin
 * \par Description
 *   Return the arduino pin number of current RJ25 object's port, if the RJ25 module
 *   have one available PIN.
 * \par Output
 *   None
 * \return
 *   The PIN number of arduino
 * \par Others
 *   None
 */
  uint8_t pin(void);

/**
 * \par Function
 *   pin
 * \par Description
 *   Return the arduino pin number of current RJ25 object's port
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 * \param[in]
 *   slot - SLOT1 or SLOT2
 * \par Output
 *   None
 * \return
 *   The PIN number of arduino
 * \par Others
 *   None
 */
  uint8_t pin(uint8_t port, uint8_t slot);

protected:

/**
 *  \par Description
 *  Variables used to store the slot1 gpio number
 */
  uint8_t s1;

/**
 *  \par Description
 *  Variables used to store the slot2 gpio number
 */
  uint8_t s2;

/**
 *  \par Description
 *  Variables used to store the port
 */

  uint8_t _port;

/**
 *  \par Description
 *  Variables used to store the slot
 */
  uint8_t _slot;
};
#endif // MEPORT_H_
