/**
 * \par Copyright (C), 2012-2017, MakeBlock
 * \class   MeColorSensor
 * \brief   Driver for MeColorSensor module.
 * @file    MeColorSensor.cpp
 * @author  MakeBlock
 * @version V1.0.4
 * @date    2017/01/17
 * @brief   Driver for MeColorSensor module.
 *
 * \par Copyright
 * This software is Copyright (C), 2012-2017, MakeBlock. Use is subject to license \n
 * conditions. The main licensing options available are GPL V2 or Commercial: \n
 *
 * \par Open Source Licensing +GPL V2
 * This is the appropriate option if you want to share the source code of your \n
 * application with everyone you distribute it to, and you also want to give them \n
 * the right to share who uses it. If you wish to use this software under Open \n
 * Source Licensing, you must contribute all your source code to the open source \n
 * community in accordance with the GPL Version 2 when your application is \n
 * distributed. See http://www.gnu.org/copyleft/gpl.html
 *
 * \par Description
 * This file is a drive for MeColorSensor module, It supports MeColorSensor V1.0 device provided
 * by MakeBlock.
 *
 * \par Method List:
 *
 *    1. void MeColorSensor::SensorInit(void)
 *    2. uint8_t MeColorSensor::ReportId(void)
 *    3. void MeColorSensor::ColorDataRead(void)
 *    4. void MeColorSensor::TurnOnLight(void)
 *    5. void MeColorSensor::TurnOffLight(void)
 *    6. uint16_t MeColorSensor::ReturnRedData(void)
 *    7. uint16_t MeColorSensor::ReturnGreenData(void)
 *    8. uint16_t MeColorSensor::ReturnBlueData(void)
 *    9. uint16_t MeColorSensor::ReturnColorData(void)
 *    10. long MeColorSensor::ColorIdentify(void)
 *    11. long MeColorSensor::ReturnColorCode(void)
 *    12. uint16_t MeColorSensor::calculateColorTemperature(void)
 *    13. uint16_t MeColorSensor::calculateLux(void)
 *    14. int8_t MeColorSensor::writeReg(int16_t reg, uint8_t data)
 *    15. int8_t MeColorSensor::readData(uint8_t start, uint8_t *buffer, uint8_t size)
 *    16. int8_t MeColorSensor::writeData(uint8_t start, const uint8_t *pData, uint8_t size);
 *    17. uint8_t MeColorSensor::Returnresult(void);
 *    18. uint8_t MeColorSensor::ReturnGrayscale(void);
 *    19. uint16_t MeColorSensor::ReturnColorhue(void);
 *    20. uint8_t MeColorSensor::MAX(uint8_t r,uint8_t g,uint8_t b);
 *    21. uint8_t MeColorSensor::MIN(uint8_t r,uint8_t g,uint8_t b);
 *    22. void MeColorSensor::TurnOffmodule(void);
 *    23. void MeColorSensor::TurnOnmodule(void);
 *    24. uint8_t MeColorSensor::ColorDataReadOnebyOne();
 *    
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 *  zzipeng         2017/01/17          1.0.0         complete the driver code.
 *  zzipeng         2017/04/03          1.0.1         only detect six colors.
 *  zzipeng         2017/04/10          1.0.2         only detect seven colors and add methods named MeColorSensor::TurnOffmodule(void),MeColorSensor::TurnOnmodule.
 *  zzipeng         2017/04/20          1.0.3         add methods MeColorSensor::ColorDataReadOnebyOne();
 *  Lanweiting      2017/06/23          1.0.4         Canonical the code format.
 * </pre>
 *
 * @example MeColorSensorTest.ino
 */

/* Includes ------------------------------------------------------------------*/
#include "MeColorSensor.h"

/* Private functions ---------------------------------------------------------*/
#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the MeColorSensor to arduino port,
 * no pins are used or initialized here
 */
MeColorSensor::MeColorSensor(void) : MePort(0)
{
  Device_Address = COLORSENSOR_DEFAULT_ADDRESS;
}

/**
 * Alternate Constructor which can call your own function to map the MeColorSensor to arduino port,
 * \param[in]
 *   port - RJ25 port number
 */
MeColorSensor::MeColorSensor(uint8_t port) : MePort(port)
{
  Device_Address = COLORSENSOR_DEFAULT_ADDRESS;
}

/**
 * Alternate Constructor which can call your own function to map the MeColorSensor to arduino port
 * and change the i2c device address
 * \param[in]
 *   port - RJ25 port number
 * \param[in]
 *   address - the i2c address you want to set
 */
MeColorSensor::MeColorSensor(uint8_t port, uint8_t address) : MePort(port)
{
  Device_Address = address;
}
#else  // ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the _AD0 and _INT to arduino port,
 * \param[in]
 *   _AD0 - arduino gpio number
 * \param[in]
 *   _INT - arduino gpio number
 */
MeColorSensor::MeColorSensor(uint8_t _AD0, uint8_t _INT)
{
  Device_Address = COLORSENSOR_DEFAULT_ADDRESS;
  _AD0 = _AD0;
  _INT = _INT;
}

/**
 * Alternate Constructor which can call your own function to map the _AD0 and _INT to arduino port
 * and change the i2c device address.
 * \param[in]
 *   _AD0 - arduino gpio number
 * \param[in]
 *   _INT - arduino gpio number
 * \param[in]
 *   address - the i2c address you want to set
 */
MeColorSensor::MeColorSensor(uint8_t AD0, uint8_t INT, uint8_t address)
{
  Device_Address = address;
  _AD0 = AD0;
  _INT = INT;
}
#endif /* ME_PORT_DEFINED */
/**
 * \par Function
 *   SensorInit
 * \par Description
 *   Initialize the MeColorSensor.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   You can check the bh1745 datasheet for the registor address.
 */
void MeColorSensor::SensorInit(void)
{
  MeColorSensor::TurnOnmodule();   //power on
  MeColorSensor::TurnOnLight();    //light on
  Wire.begin();
  writeReg(SYSTEM_CONTROL, SW_RESET | INT_RESET);  //SW reset and INT reset
  writeReg(MODE_CONTROL1, MEASURE_160MS);          //selet 160ms measure frequency
  writeReg(MODE_CONTROL2, 0x10);                   //active and set rgb measure gain
  writeReg(MODE_CONTROL3, 0x02);
  writeReg(INTERRUPT, 0x00);
  writeReg(PERSISTENCE, 0x01);
}

/**
 * \par Function
 *   ReportId
 * \par Description
 *   Report the MeColorSensor Module ID.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   return default ID 0xE0
 * \par Others
 *   None
 */
uint8_t MeColorSensor::ReportId(void)
{
  uint8_t temp = 0;
  readData(MANUFACTURER_ID,&temp,1);
  return temp;
}

/**
 * \par Function
 *   ColorDataRead
 * \par Description
 *   read the MeColorSensor module color data.
 * \param[in]
 *   None
 * \par Output
 *   color RGB value
 * \return
 *   None
 * \par Others
 *   None
 */
void MeColorSensor::ColorDataRead(void)
{
  uint8_t ColorData[8] = {0};
  readData(RED_DATA_LSBs, ColorData, sizeof(ColorData));
  Redvalue   = (uint16_t)ColorData[1] << 8 | ColorData[0];
  Greenvalue = (uint16_t)ColorData[3] << 8 | ColorData[2];
  Bluevalue  = (uint16_t)ColorData[5] << 8 | ColorData[4];
  Colorvalue = (uint16_t)ColorData[7] << 8 | ColorData[6];
}

/**
 * \par Function
 *   ColorDataReadOnebyOne
 * \par Description
 *   one by on to read the MeColorSensor module color data.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   return 0 is detected ID,else return Black
 * \par Others
 *   None
 */
uint8_t MeColorSensor::ColorDataReadOnebyOne(void)
{
  uint8_t id = 0;
  MeColorSensor::SensorInit();
  id = MeColorSensor::ReportId();
  if(id != CHIP_ID)
  {
      return BLACK;
  }
  delay(160);//delay 160ms tramsfer time
  MeColorSensor::ColorDataRead();
  MeColorSensor::TurnOffmodule();//power off
  return 0;
}

/**
 * \par Function
 *   ReturnColorCode
 * \par Description
 *   Return the MeColorSensor Color Code.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   return color RGB code
 * \par Others
 *   None
 */
long MeColorSensor::ReturnColorCode(void)
{
  long colorcode = 0;
  uint16_t r,g,b;
  r = Redvalue / 20;
  g = Greenvalue / 30;
  b = Bluevalue / 20;
  if(r>255) 
  {
    r=255;
  }
  if(g>255) 
  {
    g=255;
  }
  if(b>255) 
  {
    b=255;
  }
  colorcode = (long)((long)r<<16) | ((long)g<<8) | (long)b;
  return colorcode;
}

/**
 * \par Function
 *   ColorIdentify
 * \par Description
 *   Identify Color for the Module.when you want to use two modules, you can use this methods to get data.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   return Color result
 * \par Others
 *   None
 */
uint8_t MeColorSensor::ColorIdentify(void)
{
  uint8_t result,r,g,b;
  if(MeColorSensor::ColorDataReadOnebyOne())
  {
    return BLACK;//id return error!
  }
  /*********************************/
  if(Redvalue < 1200 && Greenvalue < 1700 && Bluevalue < 1200)
  {
    if(Bluevalue < Redvalue && Redvalue <= Greenvalue)  
    {
      if((Greenvalue + Redvalue + Bluevalue >300) && (Greenvalue > 1.5*Redvalue) && (Greenvalue > Bluevalue + Bluevalue))
      {
        result = GREEN;
      }
      else if((Greenvalue > Bluevalue + Bluevalue) && (Redvalue > Bluevalue + Bluevalue) && (Redvalue +  Greenvalue + Bluevalue > 120))
      {
        result = YELLOW;
      }
      else if(Redvalue > 400 && Greenvalue > 580 && Bluevalue > 320)
      {
        result = WHITE;
      }
      else
      {
        result = BLACK;
      }
    }
    else if(Greenvalue < Redvalue && Bluevalue < Greenvalue)  
    {
      if(Redvalue + Greenvalue + Bluevalue > 220)
      {
        if(Greenvalue < 2.4 * Bluevalue)
        {
          result = RED;
        }
        
        else if(Greenvalue >= 2.4 * Bluevalue)
        {
          result = RED;
        }
        else
        {
          result = BLACK;
        }
      }
      else
      {
        result = BLACK;
      }
    }
    else if(Greenvalue <= Bluevalue && Redvalue < Bluevalue)
    {
      result = BLUE;
    }
    else if(Redvalue <= Greenvalue && Bluevalue >= Redvalue)
    {
      if((Redvalue + Greenvalue + Bluevalue) > 400)
      {
        if(Greenvalue > 2.2 * Redvalue && Greenvalue > 2.2 * Bluevalue)
        {
          result = GREEN;
        }
        else
        {
          result = BLUE;
        }
      }
      else
      {
        result = BLACK;
      }
    }
    else if((Greenvalue < (Redvalue + Bluevalue)) && ((Greenvalue + Redvalue + Bluevalue) > 700))
    {
      result = WHITE;
    }
    else if((Greenvalue + Redvalue + Bluevalue) < 1000)
    {
      result = BLACK;
    }
    else
    {
      result = BLACK;
    }
  }
  /*****************************/
  else if(Bluevalue > Greenvalue && Bluevalue > Redvalue) 
  {
    result = BLUE;
  }
  else if((Redvalue > Greenvalue) && (Greenvalue > Bluevalue) &&  (Redvalue > Greenvalue))
  {
    result = RED;
  }
  /****************************/
  else
  {
    r = Redvalue / Colorvalue;
    g = Greenvalue / Colorvalue;
    b = Bluevalue / Colorvalue;
    if(r >= 9 && g <= 4 && b <= 1)
    {
      result = RED;
    }
    else if(r > 10 && g <= 4 && b <= 4)
    {
      result = RED;
    }
    else if(r < 3 && g <= 4 && b < 3)
    {
      result = BLACK;
    }
    else if(r <= 5 && g > 10 && b < 5)
    {
      result = GREEN;
    }
    else if((r < 5 && g < 5 && b > 10) || (r <= 3 && g >= 8 && b >= 8))
    {
      result = BLUE;
    }
    else if(r >= 5 && g > 6 && b >= 7)
    {
      result = BLUE;
    }
    else if(r <= 4 && g >= 10 && b >= 6)
    {
      result = BLUE;
    }
    else if(r <= 8 && r >= 6 && g >= 6 && b < 2)
    {
      result = YELLOW;
    }
    else if(r <= 8 && r <= 8 && b < 2)
    {
      //result = RED;
    }
    else if(r >= 10 && g <= 7 && b < 2)
    {
      result = RED;
    }
    else if(r >= 4 && g >= 9 && b >= 4)
    {
      result = WHITE;
    }
    else
    {
      result = WHITE;
    }
  }
  return result;
}

/**
 * \par Function
 *   Returnresult
 * \par Description
 *  Identify Color for the Module.when you use just one module, you can use this methods to get data.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   return color result
 * \par Others
 *   None
 */
uint8_t MeColorSensor::Returnresult(void)
{
  static uint8_t cnt_;
  static uint16_t temp[3];
  uint8_t result,r,g,b;

  MeColorSensor::ColorDataRead();

  /*********************************/
  if(Redvalue < 1200 && Greenvalue < 1700 && Bluevalue < 1200)
  {
    if(Greenvalue + Redvalue + Bluevalue <700)
    {
        result = BLACK;
    }
    else if(Bluevalue < Redvalue && Redvalue <= Greenvalue) 
    {
        if((Greenvalue + Redvalue + Bluevalue > 300) && (Greenvalue > 1.5 * Redvalue) && (Greenvalue > Bluevalue + Bluevalue))
        {
            result = GREEN;
        }
        else if((Greenvalue > Bluevalue + Bluevalue) && (Redvalue > Bluevalue + Bluevalue) && (Redvalue +  Greenvalue + Bluevalue > 120))
        {
            result = YELLOW;
        }
        else if(Redvalue > 450 && Greenvalue > 580 && Bluevalue > 320)
        {
            result = WHITE;
        }
        else
        {
            result = BLACK;
        }
    }
    else if(Greenvalue < Redvalue && Bluevalue < Greenvalue)
    {
        if((Redvalue + Greenvalue + Bluevalue > 500) && Redvalue > 300)
        {
            result = RED;
        }
        else
        {
            result = BLACK;
        }
    }
    else if(Greenvalue <= Bluevalue && Redvalue < Bluevalue)
    {
        result = BLUE;
    }
    else if(Redvalue <= Greenvalue && Bluevalue >= Redvalue)
    {
        if(Redvalue + Greenvalue + Bluevalue > 400)
        {
            if(Greenvalue > 2.2 * Redvalue && Greenvalue > 2.2 * Bluevalue)
            {
                result = GREEN;
            }
            else
            {
                result = BLUE;
            }
        }
        else
        {
            result = BLACK;
        }
    }
    else if((Greenvalue < Redvalue + Bluevalue) && (Greenvalue + Redvalue + Bluevalue >700))
    {
        result = WHITE;
    }
    else
    {
        result = BLACK;
    }
  }
  /*****************************/
  else
  {
    r = Redvalue / Colorvalue;
    g = Greenvalue / Colorvalue;
    b = Bluevalue / Colorvalue;

    if(r >= 9 && g <= 4 && b <= 1)
    {
      result = RED;
    }
    else if(r > 10 && g <= 4 && b <= 4)
    {
      result = RED;
    }
    else if(r < 3 && g <= 4 && b < 3)
    {
      result = BLACK;
    }
    else if(r <= 5 && g > 10 && b < 5)
    {
      result = GREEN;
    }
    else if((r < 5 && g < 5 && b > 10) || (r <= 3 && g >= 8 && b >= 8))
    {
      result = BLUE;
    }
    else if(r >= 5 && g > 6 && b >= 7)
    {
      result = BLUE;
    }
    else if(r <= 4 && g >= 10 && b >= 6)
    {
      result = BLUE;
    }
    else if(r <= 8 && r >= 6 && g >= 6 && b < 2)
    {
      result = YELLOW;
    }
    else if(r >= 10 && g <= 7 && b < 2)
    {
      result = RED;
    }
    else if(r >= 4 && g >= 9 && b >= 4)
    {
      result = WHITE;
    }
    else
    {
      result = WHITE;
    }
  }
  /***************move filter***************/
  temp[cnt_++] = result;
  if(cnt_>=3) 
  {
    cnt_ = 0;
  }
  return (temp[0] + temp[1] + temp[2])/3;
}

/**
 * \par Function
 *   ReturnGrayscale
 * \par Description
 *  Return Color Grayscale.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   return Color Grayscale value
 * \par Others
 * Gray = R*0.299 + G*0.587 + B*0.114
 * Gray = (r*38 + g*75 + b*15)>>7;  
 */
uint8_t MeColorSensor::ReturnGrayscale(void)
{
  uint8_t r,g,b;
  uint16_t gray = 0;

  r = Redvalue>>8;
  g = Greenvalue>>8;
  b = Bluevalue>>8;

  gray = (r * 38 + g * 75 + b * 15) >> 4;
  if(gray > 255) 
  {
    gray = 255;
  }
  return gray;
}

/**
 * \par Function
 *   ReturnColorhue
 * \par Description
 *   Return Color hue.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   return color hue
 * \par Others
 *   None
 */
uint16_t MeColorSensor::ReturnColorhue(void)
{
  uint8_t r,g,b,max,min,c;
  uint16_t h;

  r = Redvalue>>8;
  g = Greenvalue>>8;
  b = Bluevalue>>8;

  if((r == g) && (g == b))
  {
    h = 0;
  }
  else
  {
    max = MAX(r,g,b);
    min = MIN(r,g,b);
    c = max - min;
    if(max == r)
    {
      h = ((g-b) / c % 6)*60; 
    }
    else if(max == g)
    {
      h = ((b-r) / c + 2) * 60; 
    }
    else if(min == b)
    {
      h = ((r-g) / c + 4)*60; 
    }
  }
  return h;
}
  
/**
 * \par Function
 *   ReturnRedData
 * \par Description
 *   Return Color Red value.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   return red value
 * \par Others
 *   None
 */
uint16_t MeColorSensor::ReturnRedData(void)
{
  return Redvalue;
}

/**
 * \par Function
 *   ReturnGreenData
 * \par Description
 *   Return Color Green value.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   return green value
 * \par Others
 *   None
 */
uint16_t MeColorSensor::ReturnGreenData(void)
{
  return Greenvalue;
}

/**
 * \par Function
 *   ReturnBlueData
 * \par Description
 *   Return Color Blue value.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   return blue value
 * \par Others
 *   None
 */
uint16_t MeColorSensor::ReturnBlueData(void)
{
  return Bluevalue;
}

/**
 * \par Function
 *   ReturnColorData
 * \par Description
 *   Return Color data value.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   return color data value
 * \par Others
 *   None
 */
uint16_t MeColorSensor::ReturnColorData(void)
{
  return Colorvalue;
}

/**
 * \par Function
 *  TurnOnLight
 * \par Description
 *  Turn On the MeColorSensor module Light.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
void MeColorSensor::TurnOnLight(void)
{
  MePort::dWrite1(1);
}

/**
 * \par Function
 *  TurnOffLight
 * \par Description
 *  Turn Off the MeColorSensor module Light.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
void MeColorSensor::TurnOffLight(void)
{
  MePort::dWrite1(0);
}
  
/**
 * \par Function
 *  TurnOffmodule
 * \par Description
 *  Turn Off the MeColorSensor module.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
void MeColorSensor::TurnOffmodule(void)
{
MePort::dWrite2(1);
}

/**
 * \par Function
 *  TurnOnmodule
 * \par Description
 *  Turn On the MeColorSensor module.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
void MeColorSensor::TurnOnmodule(void)
{
  MePort::dWrite2(0);//power on
}

/**
 * \par Function
 *  writeReg
 * \par Description
 *  write the MeColorSensor module register.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   Return the error code.
 *   the definition of the value of variable return_value:
 *   0:success
 *   1:BUFFER_LENGTH is shorter than size
 *   2:address send, nack received
 *   3:data send, nack received
 *   4:other twi error
 *   refer to the arduino official library twi.c
 * \par Others
 *   To set the register for initializing.
 */
int8_t MeColorSensor::writeReg(int16_t reg, uint8_t data)
{
  int8_t return_value = 0;

  return_value = writeData(reg, &data, 1);
  
  return(return_value);
}

/**
 * \par Function
 *   readData
 * \par Description
 *   Write the data to i2c device.
 * \param[in]
 *   start - the address which will write the data to.
 * \param[in]
 *   pData - the head address of data array.
 * \param[in]
 *   size - set the number of data will be written to the devide.
 * \par Output
 *   None
 * \return
 *   Return the error code.
 *   the definition of the value of variable return_value:
 *   0:success
 *   1:BUFFER_LENGTH is shorter than size
 *   2:address send, nack received
 *   3:data send, nack received
 *   4:other twi error
 *   refer to the arduino official library twi.c
 * \par Others
 *   Calling the official i2c library to read data.
 */
int8_t MeColorSensor::readData(uint8_t start, uint8_t *buffer, uint8_t size)
{
  int16_t i = 0;
  int8_t return_value = 0;
  Wire.beginTransmission(Device_Address);
  return_value = Wire.write(start);

  if(return_value != 1)
  {
    return(I2C_ERROR);
  }
  return_value = Wire.endTransmission(false);

  if(return_value != 0)
  {
    return(return_value);
  }
  delayMicroseconds(1);
  /* Third parameter is true: relase I2C-bus after data is read. */
  Wire.requestFrom(Device_Address, size, (uint8_t)true);
  
  while(Wire.available() && i < size)
  {
    buffer[i++] = Wire.read();
  }
  delayMicroseconds(1);
  
  if(i != size)
  {
    return(I2C_ERROR);
  }
  return 0; 
}

/**
 * \par Function
 *   writeData
 * \par Description
 *   Write the data to i2c device.
 * \param[in]
 *   start - the address which will write the data to.
 * \param[in]
 *   pData - the head address of data array.
 * \param[in]
 *   size - set the number of data will be written to the devide.
 * \par Output
 *   None
 * \return
 *   Return the error code.
 *   the definition of the value of variable return_value:
 *   0:success
 *   1:BUFFER_LENGTH is shorter than size
 *   2:address send, nack received
 *   3:data send, nack received
 *   4:other twi error
 *   refer to the arduino official library twi.c
 * \par Others
 *   Calling the official i2c library to write data.
 */
int8_t MeColorSensor::writeData(uint8_t start, const uint8_t *pData, uint8_t size)
{
  int8_t return_value = 0;
  Wire.beginTransmission(Device_Address);
  return_value = Wire.write(start); 
  if(return_value != 1)
  {
      return(I2C_ERROR);
  }
  Wire.write(pData, size);  
  return_value = Wire.endTransmission(true); 
  return return_value;                      
}

/**
 * \par Function
 *   MAX
 * \par Description
 *   find the max one from r g b.
 * \param[in]
 *  r g b  
 * \par Output
 *   None
 * \return
 *   Return the max one.
 * \par Others
 */
uint8_t MeColorSensor::MAX(uint8_t r, uint8_t g, uint8_t b)
{
  uint8_t max;
  if(r >= g)
  {
    if(r >= b)
    {
      max = r;
    }
    else
    {
      max = b;
    }
  }
  else if(g >= b)
  {
    max = g;
  }
  else
  {
    max = b;
  }
  return max;
}

/**
 * \par Function
 *   MIN
 * \par Description
 *   find the min one from r g b.
 * \param[in]
 *  r g b  
 * \par Output
 *   None
 * \return
 *   Return the min one.
 * \par Others
 */
uint8_t MeColorSensor::MIN(uint8_t r, uint8_t g, uint8_t b)
{
  uint8_t min;
  if(r <= g)
  {
    if(r <= b)
    {
      min = r;
    }
    else
    {
      min = b;
    }
  }
  else if(g <= b)
  {
    min = g;
  }
  else
  {
    min = b;
  }
  return min;
}




