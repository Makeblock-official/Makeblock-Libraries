/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MeLimitSwitch
 * \brief   Driver for Me_LimitSwitch module.
 * @file    MeLimitSwitch.cpp
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/04
 * @brief   Driver for Me LimitSwitch module.
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
 *
 * \par Method List:
 *
 *    1. void MeLimitSwitch::setpin(uint8_t switchPin);
 *    2. bool MeLimitSwitch::touched();
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/07/24     1.0.0            Rebuild the old lib.
 * Rafael Lee       2015/09/04     1.0.1            Added some comments and macros.
 * </pre>
 *
 * @example LimitSwitchTest.ino \n
 *          MicroSwitchTest.ino \n
 */

/* Includes ------------------------------------------------------------------*/
#include "MeLimitSwitch.h"


/* Private functions ---------------------------------------------------------*/
#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the limit switch to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
MeLimitSwitch::MeLimitSwitch(void) : MePort(0)
{

}

/**
 * Alternate Constructor which can call your own function to map the limit switch to arduino port,
 * the slot2 will be used here.
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
MeLimitSwitch::MeLimitSwitch(uint8_t port) : MePort(port)
{
  pinMode(s2, INPUT_PULLUP);
}

/**
 * Alternate Constructor which can call your own function to map the limit switch to arduino port,
 * you can set any slot for the limit switch device. 
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 * \param[in]
 *   slot - SLOT1 or SLOT2
 */
MeLimitSwitch::MeLimitSwitch(uint8_t port, uint8_t slot) : MePort(port)
{
  reset(port, slot);
  if (getSlot() == SLOT1)
  {
    pinMode(s1, INPUT_PULLUP);
  }
  else
  {
    pinMode(s2, INPUT_PULLUP);
  }
}
#else // ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the limit switch to arduino port,
 * \param[in]
 *   switchPin - arduino port for switch detect pin.
 */
MeLimitSwitch::MeLimitSwitch(uint8_t switchPin)
{
  _switchPin = switchPin;
  pinMode(s2, INPUT_PULLUP);
}
#endif // ME_PORT_DEFINED

/**
 * \par Function
 *    setpin
 * \par Description
 *    Reset the limit switch available PIN by its arduino port.
 * \param[in]
 *    switchPin - arduino port for switch detect pin.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeLimitSwitch::setpin(uint8_t switchPin)
{
  _switchPin = switchPin;
  pinMode(_switchPin, INPUT_PULLUP);
#ifdef ME_PORT_DEFINED
  if (getSlot() == SLOT1)
  {
    s1 = switchPin;
  }
  else
  {
    s2 = switchPin;
  }
#endif // ME_PORT_DEFINED
}

/**
 * \par Function
 *    touched
 * \par Description
 *    Get switch value from selected _slot defined by MePort.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \Return
 *    True if module is touched.
 * \par Others
 *    None
 */
bool MeLimitSwitch::touched(void)
{
#ifdef ME_PORT_DEFINED
  return(!(getSlot() == SLOT1 ? digitalRead(s1) : digitalRead(s2)));
#else // ME_PORT_DEFINED
  return digitalRead(_switchPin);
#endif // ME_PORT_DEFINED
}

