/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MeLEDMatrix
 * \brief   Driver for Me LED Matrix module.
 * @file    MeLEDMatrix.h
 * @author  MakeBlock
 * @version V1.0.3
 * @date    2016/01/29
 * @brief   Header for MeLEDMatrix.cpp module
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
 * This file is a drive for Me LED Matrix device
 *
 * \par Method List:
 *
 *    1.    void MeLEDMatrix::clearScreen();
 *    2.    void MeLEDMatrix::setBrightness(uint8_t Bright);
 *    3.    void MeLEDMatrix::setColorIndex(bool Color_Number);
 *    4.    void MeLEDMatrix::drawBitmap(int8_t x, int8_t y, uint8_t Bitmap_Width, uint8_t *Bitmap);
 *    5.    void MeLEDMatrix::drawStr(int16_t X_position, int8_t Y_position, const char *str);
 *    6.    void MeLEDMatrix::showClock(uint8_t hour, uint8_t minute, bool point_flag);
 *    7.    void MeLEDMatrix::showNum(float value,uint8_t digits);
 *    8.    void MeLEDMatrix::reset(uint8_t port);
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * forfish         2015/11/09     1.0.0            Add description
 * Mark Yan        2016/01/19     1.0.1            Add some new symbol
 * Mark Yan        2016/01/27     1.0.2            Add digital printing
 * Mark Yan        2016/01/29     1.0.3            Fix issue when show integer number
 * </pre>
 *
 * @example Me_LEDMatrixTest.ino
 */
 
#ifndef _ME_LED_MATRIX_H_
#define _ME_LED_MATRIX_H_

#include "MePort.h"
#define PointOn   1
#define PointOff  0


#define LED_BUFFER_SIZE   16
#define STRING_DISPLAY_BUFFER_SIZE 20


//Define Data Command Parameters
#define Mode_Address_Auto_Add_1  0x40     //0100 0000 B
#define Mode_Permanent_Address   0x44     //0100 0100 B


//Define Address Command Parameters
#define ADDRESS(addr)  (0xC0 | addr)


typedef enum
{
  Brightness_0 = 0,
  Brightness_1,
  Brightness_2,
  Brightness_3,
  Brightness_4,
  Brightness_5,
  Brightness_6,
  Brightness_7,
  Brightness_8
}LED_Matrix_Brightness_TypeDef;



/*           Me LED Matrix 8X16            */
/**
 * Class: MeLEDMatrix
 * \par Description
 * Declaration of Class MeLEDMatrix.
 */
class MeLEDMatrix:public MePort
{
public:
/**
 * Alternate Constructor which can call your own function to map the LED Matrix to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
*/
  MeLEDMatrix();
  
/**
 * Alternate Constructor which can call your own function to map the LED Matrix to arduino port,
 * If the hardware serial was selected, we will used the hardware serial.
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
*/
  MeLEDMatrix(uint8_t port);
  
/**
 * Alternate Constructor which can call your own function to map the Buzzer to arduino port,
 * you can set any slot for the buzzer device. 
 * \param[in]
 *   SCK_Pin - The SCk of LED Matrix.
 * \param[in]
 *   DIN_Pin - Put in parameter.
 */
  MeLEDMatrix(uint8_t SCK_Pin, uint8_t DIN_Pin);

/**
 * \par Function
 *    clearScreen
 * \par Description
 *    Clear the screen.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \Return
 *    None.
 * \par Others
 *    None
 */
  void clearScreen();
  
/**
 * \par Function
 *    setBrightness
 * \par Description
 *    Set the brightness of LED Matrix.
 * \param[in]
 *    Bright - The brightness of LED Matrix.
 * \par Output
 *    None
 * \Return
 *    None.
 * \par Others
 *    None
 */
  void setBrightness(uint8_t Bright);
  
/**
 * \par Function
 *    setColorIndex
 * \par Description
 *    Set the color index for LED Matrix.
 * \param[in]
 *    Color_Number - The number of LED Matrix's color.
 * \par Output
 *    None
 * \Return
 *    None.
 * \par Others
 *    None
 */
  void setColorIndex(bool Color_Number);
  
/**
 * \par Function
 *    drawBitmap
 * \par Description
 *    Draw a bitmap.
 * \param[in]
 *    x - The x coordinate of bitmap.
 * \param[in]
 *    y - The y coordinate of bitmap.
 * \param[in]
 *    Bitmap_Width - The width of bitmap.
 * \param[in]
 *    Bitmap - A pointer to bitmap.
 * \par Output
 *    None
 * \Return
 *    None.
 * \par Others
 *    None
 */
  void drawBitmap(int8_t x, int8_t y, uint8_t Bitmap_Width, uint8_t *Bitmap);
  
/**
 * \par Function
 *    drawStr
 * \par Description
 *    Draw a string.
 * \param[in]
 *    x - The x coordinate for the beginning of string.
 * \param[in]
 *    y - The y coordinate for the beginning of string.
 * \param[in]
 *    str - A pointer to string.
 * \par Output
 *    None
 * \Return
 *    None.
 * \par Others
 *    None
 */
  void drawStr(int16_t X_position, int8_t Y_position, const char *str);
  
/**
 * \par Function
 *    showClock
 * \par Description
 *    Show the clock on LED Matrix.
 * \param[in]
 *    hour - The part of hour in clock.
 * \param[in]
 *    minute - The part of minute in clock.
 * \param[in]
 *    PointOn - Point on or not.
 * \par Output
 *    None
 * \Return
 *    None.
 * \par Others
 *    None
 */
  void showClock(uint8_t hour, uint8_t minute, bool = PointOn);

/**
 * \par Function
 *    showNum
 * \par Description
 *    Show the number on LED Matrix.
 * \param[in]
 *    value - The float data need show.
 * \param[in]
 *    digits - Number of digits to display.
 * \par Output
 *    None
 * \Return
 *    None.
 * \par Others
 *    None
 */
  void showNum(float value,uint8_t = 3);

/**
 * \par Function
 *    reset
 * \par Description
 *    Reset the port of LED Matrix.
 * \param[in]
 *    port - The port of LED Matrix.
 * \par Output
 *    None
 * \Return
 *    None.
 * \par Others
 *    None
 */
  void reset(uint8_t port);

private:
  uint8_t u8_SCKPin;
  uint8_t u8_DINPin;

  bool b_Color_Index;
  bool b_Draw_Str_Flag;

  uint8_t u8_Display_Buffer[LED_BUFFER_SIZE];

  int16_t i16_Str_Display_X_Position;
  int8_t i8_Str_Display_Y_Position;
  int16_t i16_Number_of_Character_of_Str;
  char i8_Str_Display_Buffer[STRING_DISPLAY_BUFFER_SIZE];

/**
 * \par Function
 *    writeByte
 * \par Description
 *    Write byte to LED Matrix.
 * \param[in]
 *    data - The data wrote to LED Matrix.
 * \par Output
 *    None
 * \Return
 *    None.
 * \par Others
 *    None
 */
  void writeByte(uint8_t data);
  
/**
 * \par Function
 *    writeBytesToAddress
 * \par Description
 *    Write byte to LED Matrix's address.
 * \param[in]
 *    Address - The address you want to write in LED Matrix.
 * \param[in]
 *    P_data - The pointer points to data.
 * \param[in]
 *    count_of_data - The length of data.
 * \par Output
 *    None
 * \Return
 *    None.
 * \par Others
 *    None
 */
  void writeBytesToAddress(uint8_t Address, const uint8_t *P_data, uint8_t count_of_data);
  
/**
 * \par Function
 *    showStr
 * \par Description
 *    Show the string in LED Matrix.
 * \par Output
 *    None
 * \Return
 *    None.
 * \par Others
 *    None
 */
  void showStr();

};

#endif
