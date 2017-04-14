/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MeGyro
 * \brief   Driver for MeColorSensor module.
 * @file    MeColorSensor.cpp
 * @author  MakeBlock
 * @version V1.0.2
 * @date    2017/01/17
 * @brief   Driver for MeColorSensor module.
 *
 * \par Copyright
 * This software is Copyright (C), 2012-2016, MakeBlock. Use is subject to license \n
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
 *    9. uint16_t MeColorSensor::ReturnClearData(void)
 *    10.long MeColorSensor::ColorIdentify(void)
 *    11.long MeColorSensor::ReturnColorCode(void)
 *    12.uint16_t MeColorSensor::calculateColorTemperature(void)
 *    13.uint16_t MeColorSensor::calculateLux(void)
 *    14.int8_t MeColorSensor::writeReg(int16_t reg, uint8_t data)
 *    15.int8_t MeColorSensor::readData(uint8_t start, uint8_t *buffer, uint8_t size)
 *    16.int8_t MeColorSensor::writeData(uint8_t start, const uint8_t *pData, uint8_t size);
 *    17.uint8_t MeColorSensor::Returnresult(void);
 *    18.uint8_t MeColorSensor::ReturnGrayscale(void);
 *    19.uint16_t MeColorSensor::ReturnColorhue(void);
 *    20.uint8_t MeColorSensor::MAX(uint8_t r,uint8_t g,uint8_t b);
 *    21.uint8_t MeColorSensor::MIN(uint8_t r,uint8_t g,uint8_t b);
 *    22.void MeColorSensor::TurnOffmodule(void);
 *    23.void MeColorSensor::TurnOnmodule(void);
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 *  zzipeng         2017/01/17          1.0.0         complete the driver code.
 *  zzipeng         2017/04/03          1.0.1         only detect six colors.
 *  zzipeng         2017/04/10          1.0.2         only detect seven colors and add methods named MeColorSensor::TurnOffmodule(void),MeColorSensor::TurnOnmodule.
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
 * no pins are used or initialized here, but PWM frequency set to 976 Hz
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
MeColorSensor::MeColorSensor(uint8_t port) : MePort(port)
{
  Device_Address = COLORSENSOR_DEFAULT_ADDRESS;
}

/**
 * Alternate Constructor which can call your own function to map the MeColorSensor to arduino port
 * and change the i2c device address
 * no pins are used or initialized here, but PWM frequency set to 976 Hz
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
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
 * no pins are used or initialized here
 * \param[in]
 *   _AD0 - arduino gpio number
 * \param[in]
 *   _INT - arduino gpio number
 */
MeColorSensor::MeColorSensor(uint8_t AD0, uint8_t INT)
{
  Device_Address = COLORSENSOR_DEFAULT_ADDRESS;
  _AD0 = AD0;
  _INT = INT;
}

/**
 * Alternate Constructor which can call your own function to map the _AD0 and _INT to arduino port
 * and change the i2c device address, no pins are used or initialized here
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
#endif // ME_PORT_DEFINED
/**
 * \par Function
 *   begin
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
   MeColorSensor::TurnOnmodule();//power on
   MeColorSensor::TurnOnLight();//light on
  //  MePort::aWrite1(0x32);
   delay(20);
   Wire.begin();
   writeReg(SYSTEM_CONTROL, SW_RESET|INT_RESET);//SW reset and int reset
   writeReg(MODE_CONTROL1, MEASURE_160MS);//selet 160ms measure fre
   writeReg(MODE_CONTROL2, 0x10);//0x92active and set rgb measure gain
   writeReg(MODE_CONTROL3, 0x02);
   writeReg(INTERRUPT, 0x00);
   writeReg(PERSISTENCE, 0x01);
}
   /**
 * \par Function
 *   begin
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
uint8_t MeColorSensor::ReportId(void)
{
  uint8_t temp = 0;
  readData(SYSTEM_CONTROL,&temp,1);
  return temp;
}
   /**
 * \par Function
 *   begin
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
  void MeColorSensor::ColorDataRead(void)
  {
    uint8_t ColorData[8] = {0};
    readData(RED_DATA_LSBs, ColorData, sizeof(ColorData));
    Redvalue   = (uint16_t)ColorData[1]<<8|ColorData[0];
    Greenvalue = (uint16_t)ColorData[3]<<8|ColorData[2];
    Bluevalue  = (uint16_t)ColorData[5]<<8|ColorData[4];
    Clearvalue = (uint16_t)ColorData[7]<<8|ColorData[6];
  }
/**
 * \par Function
 *   begin
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
  long MeColorSensor::ReturnColorCode(void)
  {
    long colorcode = 0;
    uint16_t r,g,b;
    // r = Redvalue>>8;
    // g = Greenvalue>>8;
    // b = Bluevalue>>8;
    r = Redvalue>>4;
    g = Greenvalue>>4;
    b = Bluevalue>>4;
    if(r>255) r=255;
    if(g>255) g=255;
    if(b>255) b=255;
    colorcode = (long)((long)r<<16)|((long)g<<8)|(long)b;
    return colorcode;
  }
   /**
 * \par Function
 *   ColorIdentify
 * \par Description
 *  when you want to use two modules, you can use this methods to get data.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   You can check the bh1745 datasheet for the registor address.
 */
 uint8_t MeColorSensor::ColorIdentify(void)
 {
    // static uint8_t cnt;
    // static uint16_t Temp_[3];
    uint8_t result,r,g,b;
    MeColorSensor::SensorInit();
    delay(160);//160ms tramsfer time
    MeColorSensor::ColorDataRead();
    MeColorSensor::TurnOffmodule();//power off
 /*****************小数据远距离判断策略****************/
    if(Redvalue<1200 && Greenvalue<1700 && Bluevalue<1200)
    {
       if(Bluevalue<Redvalue && Redvalue<=Greenvalue)// b<r<g   g最大,b最小，green 和 yellow black,white区分
       {
           if((Greenvalue + Redvalue + Bluevalue >300) && (Greenvalue > 1.5*Redvalue) && (Greenvalue > Bluevalue + Bluevalue))
           {
             result = GREEN;//判定绿色
           }
           else if((Greenvalue > Bluevalue + Bluevalue) && (Redvalue > Bluevalue + Bluevalue) && (Redvalue +  Greenvalue + Bluevalue>120))//芯片对蓝色不太灵敏，所以导致对黄色特别好识别 r>2b g>2b
           {
             result = YELLOW;//判定黄色
           }
           else if(Redvalue>450 && Greenvalue>580 && Bluevalue>320)//调小数据提高白色识别高度
           {
             result = WHITE;//
           }
           else
           {
             result = BLACK;//30mm以外认为是黑色
           }
       }
       else if(Greenvalue<Redvalue && Bluevalue<Greenvalue)//r>g>b  r最大,b最小,red 和 orange black区分
       {
         if(Redvalue + Greenvalue + Bluevalue > 220)
         {
            // if(Redvalue > 2.4*Greenvalue && Greenvalue > 1.4*Bluevalue)//为了满足远距离要求，判定条件由 r>3g g>2b改为r>2.6*g g>1.4*b.
            if(Greenvalue < 2.4*Bluevalue)//用b数据判断效果更加好  将参数2.4改小可以提高橙色识别高度，降低红色识别高度
            {
              result = RED;//red 
            }
            //else if(Redvalue > 1.3*Greenvalue && Greenvalue > 2.3*Bluevalue)//为了满足远距离要求，判定条件由 r>2g g>3b改为r>1.3*g g>2.3*b.
            else  if(Greenvalue >= 2.4*Bluevalue)//用b数据判断效果更加好
            {
               result = ORANGE;//orange r>1.8*g g>3*b
            }
            else
            {
              result = BLACK/*PINKE*/;
            }
         }
         else
         {
           result = BLACK;//否则距离太远判定黑色
         }
       }
      else if(Greenvalue <= Bluevalue && Redvalue<Bluevalue)//b>g b>r//包括b>g>r b>r>g//蓝色，紫色区分
      {
          result = BLUE;//判定蓝色 删除紫色，都判断为蓝色
        // if(Bluevalue>=Redvalue+Redvalue)//b>=2*r
        // {
        //   result = BLUE;//判定蓝色
        // }
        // else
        // {
        //   result = BLUE/*PURPLE*/;//判定紫色//删除紫色
        // }
      }
       else if(Redvalue<=Greenvalue && Bluevalue>=Redvalue)// r<b<g//g max b middle r min 蓝色，绿色，紫色，黑色
       {
         if(Redvalue + Greenvalue + Bluevalue>400)
         {
           if(Greenvalue>2.2*Redvalue && Greenvalue>2.2*Bluevalue)
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
           result = BLACK;//30mm以外认为是黑色
         }
       }
       else if((Greenvalue < Redvalue + Bluevalue) && (Greenvalue + Redvalue + Bluevalue >700))//
       {
           result = WHITE;//判定白色
       }
       else if(Greenvalue + Redvalue + Bluevalue <1000)
       {
           result = BLACK;//判定黑色
       }
      else
      {
        result = BLACK;
      }
    }
    /************** 稍大数据中距离判断策略***************/
    else if(Bluevalue>Greenvalue && Bluevalue>Redvalue)// b>g  b>r//包括b>g>r b>r>g//蓝色，紫色区分
    {
        result = BLUE;//在这一范围内都认为是蓝色
      // if(Bluevalue < Redvalue+Redvalue)//紫色成分中红色所占比例比较大
      //  {
      //    result = BLUE/*PURPLE*/;//删除紫色  只要满足b>g>r都认为是蓝色
      //  }
      //  else                          //蓝色成分中红色所占比例比较小
      //  {
      //    result = BLUE;
      //  }
    }
    else if(Redvalue > Greenvalue && Greenvalue > Bluevalue)// r>g>b r<2000
    {
        if(Greenvalue < 2.5*Bluevalue)//用b数据判断效果更加好
       {
            result = RED;//red 
       }
       else if(Greenvalue >= 2.5*Bluevalue)//用b数据判断效果更加好
       {
            result = ORANGE;//orange r>1.8*g g>3*b
        }
    }
   /************** 超大数据近距离判断策略***************/
    else
    {
      r = Redvalue/Clearvalue;
      g = Greenvalue/Clearvalue;
      b = Bluevalue/Clearvalue;

      if(r>10&&g<=4&&b<=1||r>10&&g<=3&&b<=1)
      {
        result = RED;
      }
      else if(r>10&&g<=4&&b<=4)
      {
        result = RED/*PINKE*/;//删除粉红色
      }
      else if(r<3&&g<=4&&b<3)
      {
          result = BLACK;
      }
      else if(r<=5&&g>10&&b<5)
      {
        result = GREEN;
      }
      else if((r<5&&g<5&&b>10)||r<=3&&g>=8&&b>=8)
      {
        result = BLUE;
      }
      else if((r>=5&&g>6&&b>=7))
      {
        result = BLUE/*PURPLE*/;//删除紫色
      }
      else if((r<=4&&g>=10&&b>=6))
      {
        result = BLUE/*CYAN*/;//删除青色
      }
      else if(r<=8&&r>=6&&g>=6&&b<2)
      {
        result = YELLOW;
      }
      else if(r<=8&&r<=8&&b<2)
      {
        result = /*GOLD*/ORANGE;//删除金色
      }
      else if(r>=10&&g<=7&&b<2)
      {
        result = ORANGE;
      }
      else if(r>=4&&g>=9&&b>=4)
      {
        result = WHITE;
      }
      else
      {
        result = WHITE;
      }
    }
  /***************move filter***************/
    // Temp_[cnt++] = result;
    // if(cnt>=3) cnt = 0;
    // result = (Temp_[0] + Temp_[1] + Temp_[2])/3;
    return result;
 }

  /**
 * \par Function
 *   Returnresult
 * \par Description
 *  when you use just one module, you can use this methods to get data.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   You can check the bh1745 datasheet for the registor address.
 */
 uint8_t MeColorSensor::Returnresult(void)
 {
    static uint8_t cnt_;
    static uint16_t temp[3];
    uint8_t result,r,g,b;

    MeColorSensor::ColorDataRead();

    /*****************小数据远距离判断策略****************/
    if(Redvalue<1200 && Greenvalue<1700 && Bluevalue<1200)
    {
       if(Bluevalue<Redvalue && Redvalue<=Greenvalue)// b<r<g   g最大,b最小，green 和 yellow black,white区分
       {
           if((Greenvalue + Redvalue + Bluevalue >300) && (Greenvalue > 1.5*Redvalue) && (Greenvalue > Bluevalue + Bluevalue))
           {
             result = GREEN;//判定绿色
           }
           else if((Greenvalue > Bluevalue + Bluevalue) && (Redvalue > Bluevalue + Bluevalue) && (Redvalue +  Greenvalue + Bluevalue>120))//芯片对蓝色不太灵敏，所以导致对黄色特别好识别 r>2b g>2b
           {
             result = YELLOW;//判定黄色
           }
           else if(Redvalue>450 && Greenvalue>580 && Bluevalue>320)//调小数据提高白色识别高度
           {
             result = WHITE;//white
           }
           else
           {
             result = BLACK;//30mm以外认为是黑色
           }
       }
       else if(Greenvalue<Redvalue && Bluevalue<Greenvalue)//r>g>b  r最大,b最小,red 和 orange black区分
       {
         if(Redvalue + Greenvalue + Bluevalue > 220)
         {
            // if(Redvalue > 2.4*Greenvalue && Greenvalue > 1.4*Bluevalue)//为了满足远距离要求，判定条件由 r>3g g>2b改为r>2.6*g g>1.4*b.
            if(Greenvalue < 2.4*Bluevalue)//用b数据判断效果更加好  将参数2.4改小可以提高橙色识别高度，降低红色识别高度
            {
              result = RED;//red 
            }
            //else if(Redvalue > 1.3*Greenvalue && Greenvalue > 2.3*Bluevalue)//为了满足远距离要求，判定条件由 r>2g g>3b改为r>1.3*g g>2.3*b.
            else  if(Greenvalue >= 2.4*Bluevalue)//用b数据判断效果更加好
            {
               result = ORANGE;//orange r>1.8*g g>3*b
            }
            else
            {
              result = BLACK/*PINKE*/;
            }
         }
         else
         {
           result = BLACK;//否则距离太远判定黑色
         }
       }
      else if(Greenvalue <= Bluevalue && Redvalue<Bluevalue)//b>g b>r//包括b>g>r b>r>g//蓝色，紫色区分
      {
          result = BLUE;//判定蓝色 删除紫色，都判断为蓝色
        // if(Bluevalue>=Redvalue+Redvalue)//b>=2*r
        // {
        //   result = BLUE;//判定蓝色
        // }
        // else
        // {
        //   result = BLUE/*PURPLE*/;//判定紫色//删除紫色
        // }
      }
       else if(Redvalue<=Greenvalue && Bluevalue>=Redvalue)// r<b<g//g max b middle r min 蓝色，绿色，紫色，黑色
       {
         if(Redvalue + Greenvalue + Bluevalue>400)
         {
           if(Greenvalue>2.2*Redvalue && Greenvalue>2.2*Bluevalue)
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
           result = BLACK;//30mm以外认为是黑色
         }
       }
       else if((Greenvalue < Redvalue + Bluevalue) && (Greenvalue + Redvalue + Bluevalue >700))//
       {
           result = WHITE;//判定白色
       }
       else if(Greenvalue + Redvalue + Bluevalue <1000)
       {
           result = BLACK;//判定黑色
       }
      else
      {
        result = BLACK;
      }
    }
    /************** 稍大数据中距离判断策略***************/
    else if(Bluevalue>Greenvalue && Bluevalue>Redvalue)// b>g  b>r//包括b>g>r b>r>g//蓝色，紫色区分
    {
        result = BLUE;//在这一范围内都认为是蓝色
      // if(Bluevalue < Redvalue+Redvalue)//紫色成分中红色所占比例比较大
      //  {
      //    result = BLUE/*PURPLE*/;//删除紫色  只要满足b>g>r都认为是蓝色
      //  }
      //  else                          //蓝色成分中红色所占比例比较小
      //  {
      //    result = BLUE;
      //  }
    }
    else if(Redvalue > Greenvalue && Greenvalue > Bluevalue)// r>g>b r<2000
    {
        if(Greenvalue < 2.5*Bluevalue)//用b数据判断效果更加好
       {
            result = RED;//red 
       }
       else if(Greenvalue >= 2.5*Bluevalue)//用b数据判断效果更加好
       {
            result = ORANGE;//orange r>1.8*g g>3*b
        }
    }
   /************** 超大数据近距离判断策略***************/
    else
    {
      r = Redvalue/Clearvalue;
      g = Greenvalue/Clearvalue;
      b = Bluevalue/Clearvalue;

      if(r>10&&g<=4&&b<=1||r>10&&g<=3&&b<=1)
      {
        result = RED;
      }
      else if(r>10&&g<=4&&b<=4)
      {
        result = RED/*PINKE*/;//删除粉红色
      }
      else if(r<3&&g<=4&&b<3)
      {
          result = BLACK;
      }
      else if(r<=5&&g>10&&b<5)
      {
        result = GREEN;
      }
      else if((r<5&&g<5&&b>10)||r<=3&&g>=8&&b>=8)
      {
        result = BLUE;
      }
      else if((r>=5&&g>6&&b>=7))
      {
        result = BLUE/*PURPLE*/;//删除紫色
      }
      else if((r<=4&&g>=10&&b>=6))
      {
        result = BLUE/*CYAN*/;//删除青色
      }
      else if(r<=8&&r>=6&&g>=6&&b<2)
      {
        result = YELLOW;
      }
      else if(r<=8&&r<=8&&b<2)
      {
        result = /*GOLD*/ORANGE;//删除金色
      }
      else if(r>=10&&g<=7&&b<2)
      {
        result = ORANGE;
      }
      else if(r>=4&&g>=9&&b>=4)
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
      if(cnt_>=3) cnt_ = 0;
      return (temp[0] + temp[1] + temp[2])/3;
 }
/*
*  Gray = R*0.299 + G*0.587 + B*0.114
*  Gray = (r*38 + g*75 + b*15)>>7;
*/
 uint8_t MeColorSensor::ReturnGrayscale(void)
 {
    uint8_t r,g,b;
    uint16_t gray = 0;
    r = Redvalue>>8;
    g = Greenvalue>>8;
    b = Bluevalue>>8;
    gray = (r*38 + g*75 + b*15)>>4;//470R  >>4  // 100R >>6
    if(gray>255) gray =255;
    return gray;
  }
  /**
 * \par Function
 *   begin
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
 uint16_t MeColorSensor::ReturnColorhue(void)
 {
    uint8_t r,g,b,max,min,c;
    uint16_t h;
    r = Redvalue>>8;
    g = Greenvalue>>8;
    b = Bluevalue>>8;
    if(r==g&&g==b)
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
        h = ((g-b)/c%6)*60; 
      }
      else if(max == g)
      {
        h = ((b-r)/c+2)*60; 
      }
      else if(min == b)
      {
        h = ((r-g)/c+4)*60; 
      }
    }
    return h;
  }
  /**
 * \par Function
 *   begin
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
  uint16_t MeColorSensor::ReturnRedData(void)
  {
    return Redvalue;
  }
     /**
 * \par Function
 *   begin
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
  uint16_t MeColorSensor::ReturnGreenData(void)
  {
    return Greenvalue;
  }
     /**
 * \par Function
 *   begin
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
  uint16_t MeColorSensor::ReturnBlueData(void)
  {
    return Bluevalue;
  }
     /**
 * \par Function
 *   begin
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
  uint16_t MeColorSensor::ReturnClearData(void)
  {
    return Clearvalue;
  }
       /**
 * \par Function
 *   begin
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
  void MeColorSensor::TurnOnLight(void)
  {
    MePort::dWrite1(1);
    // MePort::aWrite1(0x32);
  }
  /**
 * \par Function
 *   begin
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
  void MeColorSensor::TurnOffLight(void)
  {
    MePort::dWrite1(0);
  }
  /**
 * \par Function
 *   begin
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
  void MeColorSensor::TurnOffmodule(void)
  {
     MePort::dWrite2(1);//power off
  }
  /**
 * \par Function
 *   begin
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
  void MeColorSensor::TurnOnmodule(void)
  {
     MePort::dWrite2(0);//power on
  }
/*
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
 *   To set the registor for initializing.
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
  return(0); //return: no error 
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
  return(return_value); //return: no error                     
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
uint8_t MeColorSensor::MAX(uint8_t r,uint8_t g,uint8_t b)
{
  uint8_t max;
  if(r>=g)
  {
    if(r>=b)
    {
      max = r;
    }
    else
    {
      max = b;
    }
  }
  else if(g>=b)
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
uint8_t MeColorSensor::MIN(uint8_t r,uint8_t g,uint8_t b)
{
  uint8_t min;
  if(r<=g)
  {
    if(r<=b)
    {
      min = r;
    }
    else
    {
      min = b;
    }
  }
  else if(g<=b)
  {
      min = g;
  }
  else
  {
    min = b;
  }
  return min;
}




