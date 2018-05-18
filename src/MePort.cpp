/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class MePort
 * \brief   Port Mapping for RJ25
 * @file    MePort.cpp
 * @author  MakeBlock
 * @version V1.0.1
 * @date    2015/09/09
 * @brief   Port Mapping for RJ25
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
 * Mark Yan         2015/09/01           1.0.0            Rebuild the old lib.
 * Lawrence         2015/09/09           1.0.1            Add a input parameter of function dRead1 and dRead2.
 * </pre>
 */
#include "MePort.h"

/***********************Port*********************/
/**
 * Alternate Constructor which can call your own function to map the MePort to arduino port,
 * no pins are used or initialized here
 */
MePort::MePort(void)
{
  s1 = mePort[0].s1;
  s2 = mePort[0].s2;
  _port = 0;
}

/**
 * Alternate Constructor which can call your own function to map the MePort to arduino port,
 * no pins are used or initialized here, but PWM frequency set to 976 Hz
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
MePort::MePort(uint8_t port)
{
  s1 = mePort[port].s1;
  s2 = mePort[port].s2;
  _port = port;
}

/**
 * Alternate Constructor which can call your own function to map the MePort to arduino port,
 * no pins are used or initialized here, but PWM frequency set to 976 Hz
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 * \param[in]
 *   slot - SLOT1 or SLOT2
 */
MePort::MePort(uint8_t port, uint8_t slot)
{
  s1 = mePort[port].s1;
  s2 = mePort[port].s2;
  _port = port;
  _slot = slot;
}

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
uint8_t MePort::getPort()
{
  return(_port);
}

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
uint8_t MePort::getSlot(void)
{
  return(_slot);
}

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
bool MePort::dRead1(uint8_t mode)
{
  bool val;
  pinMode(s1, mode);
  val = digitalRead(s1);
  return(val);
}

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
bool MePort::dRead2(uint8_t mode)
{
  bool val;
  pinMode(s2, mode);
  val = digitalRead(s2);
  return(val);
}

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
bool MePort::dpRead1(void)
{
  bool val;
  pinMode(s1, INPUT_PULLUP);
  val = digitalRead(s1);
  return(val);
}

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
bool MePort::dpRead2(void)
{
  bool val;
  pinMode(s2, INPUT_PULLUP);
  val = digitalRead(s2);
  return(val);
}

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
void MePort::dWrite1(bool value)
{
  pinMode(s1, OUTPUT);
  digitalWrite(s1, value);
}

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
void MePort::dWrite2(bool value)
{
  pinMode(s2, OUTPUT);
  digitalWrite(s2, value);
}

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
int16_t MePort::aRead1(void)
{
  int16_t val;
  pinMode(s1, INPUT);
  val = analogRead(s1);
  return(val);
}

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
int16_t MePort::aRead2(void)
{
  int16_t val;
  pinMode(s2, INPUT);
  val = analogRead(s2);
  return(val);
}

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
void MePort::aWrite1(int16_t value)
{
  analogWrite(s1, value);
}

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
void MePort::aWrite2(int16_t value)
{
  analogWrite(s2, value);
}

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
void MePort::reset(uint8_t port)
{
  s1 = mePort[port].s1;
  s2 = mePort[port].s2;
  _port = port;
}

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
void MePort::reset(uint8_t port, uint8_t slot)
{
  s1 = mePort[port].s1;
  s2 = mePort[port].s2;
  _port = port;
  _slot = slot;
}

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
uint8_t MePort::pin1(void)
{
  return(s1);
}

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
uint8_t MePort::pin2(void)
{
  return(s2);
}

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
uint8_t MePort::pin(void)
{
  return(_slot == SLOT_1 ? s1 : s2);
}

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
uint8_t MePort::pin(uint8_t port, uint8_t slot)
{
  return(slot == SLOT_1 ? mePort[port].s1 : mePort[port].s2);
}

