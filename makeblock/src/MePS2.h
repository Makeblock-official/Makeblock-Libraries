/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class MePS2
 * \brief   Driver for MePS2 handle device.
 * @file    MePS2.h
 * @author  MakeBlock
 * @version V1.0.1
 * @date    2016/09/23
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
 *    1. int8_t MePS2::readBuffer(int16_t index);
 *    2. void MePS2::writeBuffer(int16_t index,uint8_t c);
 *    3. void MePS2::readSerial(void);
 *    4. boolean MePS2::readjoystick(void);
 *    5. int MePS2::MeAnalog(uint8_t button);
 *    6. boolean MePS2::ButtonPressed(uint8_t button);
 *    7. void MePS2::loop(void);

 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Scott wang        2016/09/18          1.0.0        build the new lib.
 * Scott             2016/09/23          1.0.1        Add BUTTON_L and BUTTON_R.
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

#define MePS2_RX 6
#define MePS2_RY 8
#define MePS2_LX 2
#define MePS2_LY 4

#define R1 1
#define R2 3
#define L1 5
#define L2 7
#define TRIANGLE 9
#define XSHAPED 10
#define SQUARE 11
#define ROUND 12
#define START 13
#define UP 14
#define DOWN 15
#define LEFT 16
#define RIGHT 17
#define SELECT 18
#define MODE 19
#define BUTTON_L 20
#define BUTTON_R 21
#define SET_ANALOG_VALUE 0
#define ANALOG_ERROR -999

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
  long _lasttime;

  boolean _isStart;
  boolean _isAvailable;
  boolean _isReady;
};
#endif