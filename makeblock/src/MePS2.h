/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class MePS2
 * \brief   Driver for MePS2 handle device.
 * @file    MePS2.h
 * @author  MakeBlock
 * @version V1.0.4
 * @date    2016/10/08
 * @brief   Header for MePS2.cpp module
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
 * This file is a drive for Me PS2 handle device, The MePS2 inherited the 
 * MeSerial class from SoftwareSerial.
 *
 * \par Method List:
 *    1. uint8_t MePS2::readBuffer(int16_t index);
 *    2. void MePS2::writeBuffer(int16_t index,uint8_t c);
 *    3. void MePS2::readSerial(void);
 *    4. boolean MePS2::readjoystick(void);
 *    5. int16_t MePS2::MeAnalog(uint8_t button);
 *    6. boolean MePS2::ButtonPressed(uint8_t button);
 *    7. void MePS2::loop(void);
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 *  Scott wang      2016/09/18         1.0.0            Build the new lib.
 *  Scott           2016/09/20         1.0.1            Correct the receive error.
 *  Scott           2016/09/22         1.0.2            Correct the connect error.
 *  Scott           2016/09/23         1.0.3            Add BUTTON_L and BUTTON_R.
 *  Mark Yan        2016/10/08         1.0.4            Modify data format.
 * </pre>
 *
 */
#ifndef MePS2_H
#define MePS2_H

#include <stdbool.h>
#include <stdint.h>
#include <Arduino.h>
#include "MeConfig.h"
#include "MeSerial.h"

#ifdef ME_PORT_DEFINED
#include "MePort.h"
#endif 

#define MeJOYSTICK_RX           6
#define MeJOYSTICK_RY           8
#define MeJOYSTICK_LX           2
#define MeJOYSTICK_LY           4

#define MeJOYSTICK_R1           1
#define MeJOYSTICK_R2           3
#define MeJOYSTICK_L1           5
#define MeJOYSTICK_L2           7
#define MeJOYSTICK_TRIANGLE     9
#define MeJOYSTICK_XSHAPED      10
#define MeJOYSTICK_SQUARE       11
#define MeJOYSTICK_ROUND        12
#define MeJOYSTICK_START        13
#define MeJOYSTICK_UP           14
#define MeJOYSTICK_DOWN         15
#define MeJOYSTICK_LEFT         16
#define MeJOYSTICK_RIGHT        17
#define MeJOYSTICK_SELECT       18
#define MeJOYSTICK_MODE         19
#define MeJOYSTICK_BUTTON_L     20
#define MeJOYSTICK_BUTTON_R     21
#define MeJOYSTICK_INIT_VALUE   0
#define MeJOYSTICK_ANALOG_ERROR 0

/**
 * Class: MePS2
 * \par Description
 * Declaration of Class MePS2.
 */
#ifndef ME_PORT_DEFINED
  class MePS2
#else /* !ME_PORT_DEFINED */
  class MePS2 : public MeSerial
#endif  /* !ME_PORT_DEFINED */

{
public:

#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the MePS2 to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
  MePS2();

/**
 * Alternate Constructor which can call your own function to map the MePS2 to arduino port,
 * If the hardware serial was selected, we will used the hardware serial.
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
  MePS2(uint8_t port);
#else  // ME_PORT_DEFINED

/**
 * Alternate Constructor which can call your own function to map the MePS2 to arduino port,
 * If the hardware serial was selected, we will used the hardware serial.
 * \param[in]
 *   receivePin - the rx pin of serial(arduino port)
 * \param[in]
 *   transmitPin - the tx pin of serial(arduino port)
 * \param[in]
 *   inverse_logic - Whether the Serial level need inv.
 */	
  MePS2(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic);
#endif  // ME_PORT_DEFINED

/**
 * \par Function
 *    readSerial
 * \par Description
 *    Read the Serial data.
 * \param[in]
 *   None
 * \par Output
 *    None
 * \par Return
 *   None
 * \par Others
 *    Save the data to _serialRead.
 */
  void readSerial(void);

/**
 * \par Function
 *    readBuffer
 * \par Description
 *    Read the buffer data.
 * \param[in]
 *    index
 * \par Output
 *    None
 * \par Return
 *    buffer
 * \par Others
 *    None
 */
  uint8_t readBuffer(int16_t index);

/**
 * \par Function
 *    writeBuffer
 * \par Description
 *    Write the datas to the buffer.
 * \param[in]
 *    index,c
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void writeBuffer(int16_t index,uint8_t c);

/**
 * \par Function
 *    readjoystick
 * \par Description
 *    This function is used to read the handle datas.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    Returns 0 if no data ready, 1 if data ready.
 * \par Others
 *    None
 */
  boolean readjoystick(void);

/**
 * \par Function
 *    MeAnalog
 * \par Description
 *    Read the handle remote sensing analog value.
 * \param[in]
 *    Button
 * \par Output
 *    None
 * \par Return
 *    Analog value(0~255). if none return 128.
 * \par Others
 *    None
 */
  int16_t MeAnalog(uint8_t button);

/**
 * \par Function
 *    ButtonPressed
 * \par Description
 *   This function is used to check the button whether to press.
 * \param[in]
 *    Button
 * \par Output
 *    None
 * \par Return
 *   Return button state.
 * \par Others
 *    None
 */
  boolean ButtonPressed(uint8_t button);

/**
 * \par Function
 *   loop
 * \par Description
 *   Save the MePS2 datas to the datalist.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void loop(void);

private:
  uint8_t _serialRead;
  uint8_t _dataLen;
  uint8_t _index = 0;
  uint8_t _prevc = 0;
  uint8_t buffer[16];
  uint8_t ps2_data_list[23];
  uint8_t ps2_data_list_bak[23];
  long _lasttime;

  boolean _isStart;
  boolean _isAvailable;
  boolean _isReady;
};
#endif