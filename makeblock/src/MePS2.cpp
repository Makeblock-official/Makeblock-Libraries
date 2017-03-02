/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class MePS2
 * \brief   Driver for MePS2 handle device.
 * @file    MePS2.cpp
 * @author  MakeBlock
 * @version V1.0.4
 * @date    2016/10/08
 * @brief   Driver for MePS2 device.
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
 * @example MePS2Test.ino
 */


#include "MePS2.h"
#include "MeSerial.h"

#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the MePS2 to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
MePS2::MePS2() : MeSerial(0)
{
  _isReady = false;
  _isAvailable = false;
  _isStart = false;
  _lasttime = millis();
}

/**
 * Alternate Constructor which can call your own function to map the MePS2 to arduino port,
 * If the hardware serial was selected, we will used the hardware serial.
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
 MePS2::MePS2(uint8_t port) : MeSerial(port)
{
  _isReady = false;
  _isAvailable = false;
  _isStart = false;
  _lasttime = millis();
}
#else // ME_PORT_DEFINED
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
MePS2::MePS2(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic)\
                        : MeSerial(receivePin, transmitPin, inverse_logic)
{
  _isReady = false;
  _isAvailable = false;
  _isStart = false;
  _lasttime = millis();
}
#endif  // ME_PORT_DEFINED


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
uint8_t MePS2::readBuffer(int16_t index)
{
  _index=index;
  return buffer[_index];
}

/**
 * \par Function
 *    writeBuffer
 * \par Description
 *    Write the data to the buffer.
 * \param[in]
 *    index,c
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MePS2::writeBuffer(int16_t index,uint8_t c)
{
  _index=index;
  buffer[_index]=c;
}

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
void MePS2::readSerial(void)
{
  _isAvailable = false;
  if(MeSerial::available() > 0)
  {
    _isAvailable = true;
    _serialRead = MeSerial::read();
  }
}

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
boolean MePS2::readjoystick(void)
{
  boolean result = false;
  if(millis() - _lasttime > 200)
  {
    _isReady = false;
    _isStart=false;
    _prevc = 0x00;
    buffer[2] = buffer[4] = buffer[6] =buffer[8] =0x80;
    buffer[1] = buffer[3] = buffer[5] =buffer[7] =0x00;
  }
  readSerial();
  while(_isAvailable)
  {
    _lasttime = millis();
    unsigned char c = _serialRead & 0xff;
    if((c == 0x55) && (_isStart == false))
    {
      if(_prevc == 0xff)
      {
        _index=1;
        _isStart = true;
      }
    }
    else
    {
      _prevc = c;
      if(_isStart)
      {
        writeBuffer(_index,c);
      }
    }
    _index++;
    if((_isStart == false) && (_index > 12))
    {
      _index=0; 
      _isStart=false;
      buffer[2] = buffer[4] = buffer[6] =buffer[8] =0x80;
      buffer[1] = buffer[3] = buffer[5] =buffer[7] =0x00;
    }
    else if(_isStart && (_index > 9))
    {
      uint8_t checksum;
      checksum = buffer[2]+buffer[3]+buffer[4]+buffer[5]+buffer[6]+buffer[7]+buffer[8];
      if(checksum == buffer[9])
      {
        _isReady = true;
       	_isStart = false;
       	_index = 0;
       	result = true;
      }
      else
      {
        _isStart = false;
        _index = 0;
        _prevc = 0x00;
        _isStart=false;
        result = false;
      }
    }
    readSerial();
   }
   return result;
}

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
 *    Analog value(-128~127). if none return 0.
 * \par Others
 *    None
 */
int16_t MePS2::MeAnalog(uint8_t button)
{
  int16_t result;
  if (!_isReady)
  {
    return (MeJOYSTICK_ANALOG_ERROR);
  }
  else
  {
    if(button == MeJOYSTICK_RX || button == MeJOYSTICK_RY || button == MeJOYSTICK_LX || button == MeJOYSTICK_LY)
    {
      result = 2*(ps2_data_list[button]-128);
      if((button == MeJOYSTICK_RY) || (button == MeJOYSTICK_LY))
      {
        result = -result;
      }
      if((result == -256) || (result == -254))
      {
        result = -255;
      }
      else if((result == 254) || (result == 256))
      {
        result = 255;
      }
      return result;
    }
    else
    {
      return MeJOYSTICK_INIT_VALUE;
    }
  }
}

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
boolean MePS2::ButtonPressed(uint8_t button) 
{
  if (!_isReady)
  {
    return ps2_data_list_bak[button];
  }
  else
  {
    ps2_data_list_bak[button] = ps2_data_list[button];
    return  ps2_data_list[button];
  }
}

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
void MePS2::loop(void)
{
  if(readjoystick())
  {
    ps2_data_list[MeJOYSTICK_LX] = buffer[2];
    ps2_data_list[MeJOYSTICK_LY] = buffer[4];
    ps2_data_list[MeJOYSTICK_RX] = buffer[6];
    ps2_data_list[MeJOYSTICK_RY] = buffer[8];
    ps2_data_list[MeJOYSTICK_R1] = (buffer[3] & 0x01) == 0x01 ? true : false;
    ps2_data_list[MeJOYSTICK_R2] = (buffer[3] & 0x02) == 0x02 ? true : false;
    ps2_data_list[MeJOYSTICK_L1] = (buffer[3] & 0x04) == 0x04 ? true : false; 
    ps2_data_list[MeJOYSTICK_L2] = (buffer[3] & 0x08) == 0x08 ? true : false;
    ps2_data_list[MeJOYSTICK_MODE] = (buffer[3] & 0x10) ==0x10 ? true : false;
    ps2_data_list[MeJOYSTICK_TRIANGLE] = (buffer[5] & 0x01) == 0x01 ? true : false;
    ps2_data_list[MeJOYSTICK_XSHAPED] = (buffer[5] & 0x02) == 0x02 ? true : false;
    ps2_data_list[MeJOYSTICK_SQUARE] = (buffer[5] & 0x04) == 0x04 ? true : false;
    ps2_data_list[MeJOYSTICK_ROUND] = (buffer[5] & 0x08) == 0x08 ? true : false;
    ps2_data_list[MeJOYSTICK_START] = (buffer[5] & 0x10) == 0x10 ? true : false;
    ps2_data_list[MeJOYSTICK_UP] = (buffer[7] & 0x01) == 0x01 ? true : false;
    ps2_data_list[MeJOYSTICK_DOWN] = (buffer[7] & 0x02) == 0x02 ? true : false ;
    ps2_data_list[MeJOYSTICK_LEFT] = (buffer[7] & 0x04) == 0x04 ? true : false ;
    ps2_data_list[MeJOYSTICK_RIGHT] = (buffer[7] & 0x08) == 0x08 ? true : false ;
    ps2_data_list[MeJOYSTICK_SELECT] = (buffer[7] & 0x10) == 0x10 ? true : false ;
    ps2_data_list[MeJOYSTICK_BUTTON_L] = (buffer[3] & 0x20) == 0x20 ? true : false ;
    ps2_data_list[MeJOYSTICK_BUTTON_R] = (buffer[7] & 0x20) == 0x20 ? true : false ;
  }
}
