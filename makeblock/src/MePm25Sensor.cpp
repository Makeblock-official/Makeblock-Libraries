/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class MePm25Sensor
 * \brief   Driver for Me PM2.5 sensor device.
 * @file    MePm25Sensor.cpp
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2016/12/13
 * @brief   Driver for Me PM2.5 sensor device.
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
 * This file is a drive for Smart Servo device, The Smart Servo inherited the 
 * MeSerial class from SoftwareSerial.
 *
 *
 * \par Method List:
 *    1. uint16_t MePm25Sensor::readPm1_0Concentration(void);
 *    2. uint16_t MePm25Sensor::readPm2_5Concentration(void);
 *    3. uint16_t MePm25Sensor::readPm10Concentration(void);
 *    4. uint16_t MePm25Sensor::read0_3NumIn100ml(void);
 *    5. uint16_t MePm25Sensor::read0_5NumIn100ml(void);
 *    6. uint16_t MePm25Sensor::read1_0NumIn100ml(void);
 *    7. uint16_t MePm25Sensor::read2_5NumIn100ml(void);
 *    8. uint16_t MePm25Sensor::read5_0NumIn100ml(void);
 *    9. uint16_t MePm25Sensor::read10NumIn100ml(void);
 *    10. uint16_t MePm25Sensor::setOuputCompatibility(void);
 *    11. uint16_t MePm25Sensor::turnOnFanLaser(void);
 *    12. uint16_t MePm25Sensor::turnOffFanLaser(void);
 *    13. uint16_t MePm25Sensor::OutputIntimeOn(void);
 *    14. uint16_t MePm25Sensor::OutputIntimeOff(void);
 *    15. uint16_t MePm25Sensor::setOutputIntimePeriod(uint16_t settime);
 *    16. uint16_t MePm25Sensor::askForData(void);
 *    17. uint16_t MePm25Sensor::coreSelfTest(void);
 *    18. uint16_t MePm25Sensor::setCoreSelfTestTime(uint8_t settime);
 *    19. uint16_t MePm25Sensor::setCoreEfficient(uint8_t efficent);
 *    20. uint16_t MePm25Sensor::rxloop(void);
 *    21. uint16_t MePm25Sensor::returnlen(void);
 *    22. uint16_t MePm25Sensor::returnchecksum(void);
 *    
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Zzipeng         2016/12/13     1.0.0            Build the new.
 * </pre>
 *
 * @example Pm25Sensor.ino
 * @Orion PORT3 PORT4 PORT5
 */
#include "MePm25Sensor.h"
#include "MeSerial.h"

#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the Me Smart Servo to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
MePm25Sensor::MePm25Sensor() : MeSerial(0)
{
  parsingSysex = false;
  sysex = {0};
  rxbuf = {0};
  sysexBytesRead = 0;
}

/**
 * Alternate Constructor which can call your own function to map the Me Smart Servo to arduino port,
 * If the hardware serial was selected, it will used the hardware serial.
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2.
 */
MePm25Sensor::MePm25Sensor(uint8_t port) : MeSerial(port)
{
  parsingSysex = false;
  sysex = {0};
  rxbuf = {0};
  sysexBytesRead = 0;
}
#else // ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the Me Smart Servo to arduino port,
 * If the hardware serial was selected, it will used the hardware serial.
 * \param[in]
 *   receivePin - the rx pin of serial(arduino port).
 * \param[in]
 *   transmitPin - the tx pin of serial(arduino port).
 * \param[in]
 *   inverse_logic - Whether the Serial level need inv.
 */
MePm25Sensor::MePm25Sensor(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic)\
                        : MeSerial(receivePin, transmitPin, inverse_logic)
{
  parsingSysex = false;
  sysex = {0};
  rxbuf = {0};
  sysexBytesRead = 0;
}
#endif // ME_PORT_DEFINED
/**
 * \par Function
 *   readByte
 * \par Description
 *   change 2byte 7bit read date to 1byte byte data.
 * \param[in]
 *   *argv - the buffer for input data.\n
 * \param[in]
 *   idx - the index used to identify the first address of read data.\n
 * \par Output
 *   return the converted data.
 * \return
 *   None
 * \par Others
 *   None
 */
  uint16_t MePm25Sensor::readPm1_0Concentration(void)//PM1.0 ug/m^3
  {
      //return sysex.val.data1;
     return (uint16_t)sysex.storedInputData[3]<<8|sysex.storedInputData[4];
  }
/**
 * \par Function
 *   readByte
 * \par Description
 *   change 2byte 7bit read date to 1byte byte data.
 * \param[in]
 *   *argv - the buffer for input data.\n
 * \param[in]
 *   idx - the index used to identify the first address of read data.\n
 * \par Output
 *   return the converted data.
 * \return
 *   None
 * \par Others
 *   None
 */
  uint16_t MePm25Sensor::readPm2_5Concentration(void)//PM2.5 ug/m^3
  {
     // return sysex.val.data2;
      return (uint16_t)sysex.storedInputData[5]<<8|sysex.storedInputData[6];
  }
/**
 * \par Function
 *   readByte
 * \par Description
 *   change 2byte 7bit read date to 1byte byte data.
 * \param[in]
 *   *argv - the buffer for input data.\n
 * \param[in]
 *   idx - the index used to identify the first address of read data.\n
 * \par Output
 *   return the converted data.
 * \return
 *   None
 * \par Others
 *   None
 */
  uint16_t MePm25Sensor::readPm10Concentration(void)//PM10 ug/m^3
  {
      //return sysex.val.data3;
       return (uint16_t)sysex.storedInputData[7]<<8|sysex.storedInputData[8];
  }
/**
 * \par Function
 *   readByte
 * \par Description
 *   change 2byte 7bit read date to 1byte byte data.
 * \param[in]
 *   *argv - the buffer for input data.\n
 * \param[in]
 *   idx - the index used to identify the first address of read data.\n
 * \par Output
 *   return the converted data.
 * \return
 *   None
 * \par Others
 *   None
 */
  uint16_t MePm25Sensor::read0_3NumIn100ml(void)//0.1L空气中直径在0.3um的颗粒物个数
  {
     // return sysex.val.data4;
      return (uint16_t)sysex.storedInputData[9]<<8|sysex.storedInputData[10];
  }
/**
 * \par Function
 *   readByte
 * \par Description
 *   change 2byte 7bit read date to 1byte byte data.
 * \param[in]
 *   *argv - the buffer for input data.\n
 * \param[in]
 *   idx - the index used to identify the first address of read data.\n
 * \par Output
 *   return the converted data.
 * \return
 *   None
 * \par Others
 *   None
 */
  uint16_t MePm25Sensor::read0_5NumIn100ml(void)//0.1L空气中直径在0.5um的颗粒物个数
  {
      //return sysex.val.data5;
       return (uint16_t)sysex.storedInputData[11]<<8|sysex.storedInputData[12];
  }
/**
 * \par Function
 *   readByte
 * \par Description
 *   change 2byte 7bit read date to 1byte byte data.
 * \param[in]
 *   *argv - the buffer for input data.\n
 * \param[in]
 *   idx - the index used to identify the first address of read data.\n
 * \par Output
 *   return the converted data.
 * \return
 *   None
 * \par Others
 *   None
 */
  uint16_t MePm25Sensor::read1_0NumIn100ml(void)//0.1L空气中直径在1.0um的颗粒物个数
  {
     // return sysex.val.data6;
      return (uint16_t)sysex.storedInputData[13]<<8|sysex.storedInputData[14];
  }
  /**
 * \par Function
 *   readByte
 * \par Description
 *   change 2byte 7bit read date to 1byte byte data.
 * \param[in]
 *   *argv - the buffer for input data.\n
 * \param[in]
 *   idx - the index used to identify the first address of read data.\n
 * \par Output
 *   return the converted data.
 * \return
 *   None
 * \par Others
 *   None
 */
  uint16_t MePm25Sensor::read2_5NumIn100ml(void)//0.1L空气中直径在2.5um的颗粒物个数
  {
      //return sysex.val.data7;
       return (uint16_t)sysex.storedInputData[15]<<8|sysex.storedInputData[16];
  }
/**
 * \par Function
 *   readByte
 * \par Description
 *   change 2byte 7bit read date to 1byte byte data.
 * \param[in]
 *   *argv - the buffer for input data.\n
 * \param[in]
 *   idx - the index used to identify the first address of read data.\n
 * \par Output
 *   return the converted data.
 * \return
 *   None
 * \par Others
 *   None
 */
  uint16_t MePm25Sensor::read5_0NumIn100ml(void)//0.1L空气中直径在5.0um的颗粒物个数
  {
      //return sysex.val.data8;
       return (uint16_t)sysex.storedInputData[17]<<8|sysex.storedInputData[18];
  }
  /**
 * \par Function
 *   readByte
 * \par Description
 *   change 2byte 7bit read date to 1byte byte data.
 * \param[in]
 *   *argv - the buffer for input data.\n
 * \param[in]
 *   idx - the index used to identify the first address of read data.\n
 * \par Output
 *   return the converted data.
 * \return
 *   None
 * \par Others
 *   None
 */
  uint16_t MePm25Sensor::read10NumIn100ml(void)//0.1L空气中直径在10um的颗粒物个数
  {
     // return sysex.val.data9;
      return (uint16_t)sysex.storedInputData[19]<<8|sysex.storedInputData[20];
  }
 /**
 * \par Function
 *   readByte
 * \par Description
 *   change 2byte 7bit read date to 1byte byte data.
 * \param[in]
 *   *argv - the buffer for input data.\n
 * \param[in]
 *   idx - the index used to identify the first address of read data.\n
 * \par Output
 *   return the converted data.
 * \return
 *   None
 * \par Others
 *   None
 */
  uint16_t MePm25Sensor::setOuputCompatibility(void)//设置输出兼容
  {
      uint8_t temp[16]={0X33,0X3E,0X00,0X0C,0XA0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X1D};//默认益杉
      uint8_t i=0;
      for(i=0;i<16;i++)
      {
          write(temp[i]);
      }
      return 0;
  }
 /**
 * \par Function
 *   readByte
 * \par Description
 *   change 2byte 7bit read date to 1byte byte data.
 * \param[in]
 *   *argv - the buffer for input data.\n
 * \param[in]
 *   idx - the index used to identify the first address of read data.\n
 * \par Output
 *   return the converted data.
 * \return
 *   None
 * \par Others
 *   None
 */
  uint16_t MePm25Sensor::turnOnFanLaser(void)//打开风扇激光
  {
      uint8_t temp[16]={0X33,0X3E,0X00,0X0C,0XA1,0X00,0X00,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X1F};
      uint8_t i=0;
      for(i=0;i<16;i++)
      {
          write(temp[i]);
      }
      return 0;
  }
 /**
 * \par Function
 *   readByte
 * \par Description
 *   change 2byte 7bit read date to 1byte byte data.
 * \param[in]
 *   *argv - the buffer for input data.\n
 * \param[in]
 *   idx - the index used to identify the first address of read data.\n
 * \par Output
 *   return the converted data.
 * \return
 *   None
 * \par Others
 *   None
 */
  uint16_t MePm25Sensor::turnOffFanLaser(void)//关闭风扇激光
  {
      uint8_t temp[16]={0X33,0X3E,0X00,0X0C,0XA1,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X1E};
      uint8_t i=0;
      for(i=0;i<16;i++)
      {
          write(temp[i]);
      }
      return 0;
  }
   /**
 * \par Function
 *   readByte
 * \par Description
 *   change 2byte 7bit read date to 1byte byte data.
 * \param[in]
 *   *argv - the buffer for input data.\n
 * \param[in]
 *   idx - the index used to identify the first address of read data.\n
 * \par Output
 *   return the converted data.
 * \return
 *   None
 * \par Others
 *   None
 */
  uint16_t MePm25Sensor::OutputIntimeOn(void)//定时输出开
  {
      uint8_t temp[16]={0X33,0X3E,0X00,0X0C,0XA2,0X00,0X00,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X20};
      uint8_t i=0;
      for(i=0;i<16;i++)
      {
          write(temp[i]);
      }
      return 0;      
  }
     /**
 * \par Function
 *   readByte
 * \par Description
 *   change 2byte 7bit read date to 1byte byte data.
 * \param[in]
 *   *argv - the buffer for input data.\n
 * \param[in]
 *   idx - the index used to identify the first address of read data.\n
 * \par Output
 *   return the converted data.
 * \return
 *   None
 * \par Others
 *   None
 */
  uint16_t MePm25Sensor::OutputIntimeOff(void)//定时输出关
  {
      uint8_t temp[16]={0X33,0X3E,0X00,0X0C,0XA2,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X1F};
      uint8_t i=0;
      for(i=0;i<16;i++)
      {
          write(temp[i]);
      }
      return 0;      
  }
     /**
 * \par Function
 *   readByte
 * \par Description
 *   change 2byte 7bit read date to 1byte byte data.
 * \param[in]
 *   *argv - the buffer for input data.\n
 * \param[in]
 *   idx - the index used to identify the first address of read data.\n
 * \par Output
 *   return the converted data.
 * \return
 *   None
 * \par Others
 *   None
 */
  uint16_t MePm25Sensor::setOutputIntimePeriod(uint16_t settime)//设定定时输出间隔 0~65535s
  {
      uint8_t d1 = settime>>8;//msb
      uint8_t d2 = settime;//lsb
      uint8_t temp[16]={0X33,0X3E,0X00,0X0C,0XA3,0X00,d1,d2,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X25};
      uint8_t i=0;
      for(i=0;i<16;i++)
      {
          write(temp[i]);
      }
      return 0;      
  }
     /**
 * \par Function
 *   readByte
 * \par Description
 *   change 2byte 7bit read date to 1byte byte data.
 * \param[in]
 *   *argv - the buffer for input data.\n
 * \param[in]
 *   idx - the index used to identify the first address of read data.\n
 * \par Output
 *   return the converted data.
 * \return
 *   None
 * \par Others
 *   None
 */
  uint16_t MePm25Sensor::askForData(void)//请求输出数据
  {
      uint8_t temp[16]={0X33,0X3E,0X00,0X0C,0XA4,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X21};
      uint8_t i=0;
      for(i=0;i<16;i++)
      {
          write(temp[i]);
      }
      return 0;           
  }
     /**
 * \par Function
 *   readByte
 * \par Description
 *   change 2byte 7bit read date to 1byte byte data.
 * \param[in]
 *   *argv - the buffer for input data.\n
 * \param[in]
 *   idx - the index used to identify the first address of read data.\n
 * \par Output
 *   return the converted data.
 * \return
 *   None
 * \par Others
 *   None
 */
  uint16_t MePm25Sensor::coreSelfTest(void)//滤芯检测
  {
      uint8_t temp[16]={0X33,0X3E,0X00,0X0C,0XA5,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X22};
      uint8_t i=0;
      for(i=0;i<16;i++)
      {
          write(temp[i]);
      }
      return 0;      
  }
 /**
 * \par Function
 *   readByte
 * \par Description
 *   change 2byte 7bit read date to 1byte byte data.
 * \param[in]
 *   *argv - the buffer for input data.\n
 * \param[in]
 *   idx - the index used to identify the first address of read data.\n
 * \par Output
 *   return the converted data.
 * \return
 *   None
 * \par Others
 *   None
 */
  uint16_t MePm25Sensor::returnCoreSelfTest(void)//返回自检结果 0要更换 1 正常
  {
     uint8_t rxflag=0,cnt=0,inputData=0;
     while (available())
     {
        // get the new byte:
         inputData = read();
        if(rxflag & 0x80 == 0x80) 
        {
             //normal data byte - add to buffer
            cnt++;
            if(cnt==6 && inputData==0x02)
            {
                rxflag=0;
                cnt=0;
                return 0;
            }
            else if(cnt==6 && inputData==0x01)
            {
                rxflag=0;
                cnt=0;
                return 1;
            }
            else
            {
                rxflag=0;
                cnt=0;
            }
            
        }
        else if(inputData == 0X4E)
        {
            if(rxflag & 0x01)
           {
             rxflag |= 0x80;//successful
             cnt = 0;    
           }
           else
           {
             rxflag = 0;//failed
             cnt = 0;                
           }   
        }
        else if(inputData == 0X42)
        {
            rxflag |= 0x01;
        }
    }           
  }
 /**
 * \par Function
 *   readByte
 * \par Description
 *   change 2byte 7bit read date to 1byte byte data.
 * \param[in]
 *   *argv - the buffer for input data.\n
 * \param[in]
 *   idx - the index used to identify the first address of read data.\n
 * \par Output
 *   return the converted data.
 * \return
 *   None
 * \par Others
 *   None
 */
  uint16_t MePm25Sensor::setCoreSelfTestTime(uint8_t settime)//设定检测时间 0~255s
  {
      uint8_t temp[16]={0X33,0X3E,0X00,0X0C,0XA6,0X00,0X00,settime,0X00,0X00,0X00,0X00,0X00,0X00,0X02,0X22};
      uint8_t i=0;
      for(i=0;i<16;i++)
      {
          write(temp[i]);
      }
      return 0;      
  }
     /**
 * \par Function
 *   readByte
 * \par Description
 *   change 2byte 7bit read date to 1byte byte data.
 * \param[in]
 *   *argv - the buffer for input data.\n
 * \param[in]
 *   idx - the index used to identify the first address of read data.\n
 * \par Output
 *   return the converted data.
 * \return
 *   None
 * \par Others
 *   None
 */
  uint16_t MePm25Sensor::setCoreEfficient(uint8_t efficent)//设定滤芯使用效率 0~100%
  {
      uint8_t temp[16]={0X33,0X3E,0X00,0X0C,0XA7,0X00,0X00,efficent,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X74};
      uint8_t i=0;
      for(i=0;i<16;i++)
      {
          write(temp[i]);
      }
      return 0;      
  }

  /**
 * \par Function
 *   smartServoEventHandle
 * \par Description
 *   This function is used to process protocol messages of smart servo.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void MePm25Sensor::rxloop(void)
  {
      uint8_t inputData=0,i=0;
      uint16_t checksum=0x006F;//0x3d+0x32
     while (available())
    {
        // get the new byte:
         inputData = read();
        if(parsingSysex & 0x81 == 0x81) 
        {
             //normal data byte - add to buffer
            rxbuf.storedInputData[sysexBytesRead] = inputData;
            // Serial.println(sysex.storedInputData[0]);
            sysexBytesRead++;
           if(sysexBytesRead >= 31)
           {
              for(i=0;i<28;i++)
             {
                checksum +=rxbuf.storedInputData[i+1]; //cal checksum
             }
            // Serial.println(checksum);
             
              if(checksum == ((uint16_t)rxbuf.storedInputData[29]<<8|rxbuf.storedInputData[30]))
              {
                for(i=0;i<31;i++)
                {
                    sysex.storedInputData[i] = rxbuf.storedInputData[i]; //data swap
                }
              }

             parsingSysex = false;
             sysexBytesRead = 0;
           }
            
        }
        else if(inputData == START_SYSEX2)
        {
            if(parsingSysex == 0x01)
           {
             parsingSysex |= 0x80;//successful
             sysexBytesRead = 0;
             inputData=0;  
           }
           else
           {
             parsingSysex = 0;//failed
             sysexBytesRead = 0;
             inputData=0;      
           }   
        }
        else if(inputData == START_SYSEX1)
        {
            parsingSysex |= 0x01;
            inputData=0;
        }
    }
  }
  /**
 * \par Function
 *   smartServoEventHandle
 * \par Description
 *   This function is used to process protocol messages of smart servo.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
uint16_t MePm25Sensor::returnlen(void)//返回有效数据长度
{
    //return sysex.val.len;
     return (uint16_t)sysex.storedInputData[1]<<8|sysex.storedInputData[2];
}
  /**
 * \par Function
 *   smartServoEventHandle
 * \par Description
 *   This function is used to process protocol messages of smart servo.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
uint16_t MePm25Sensor::returnchecksum(void)//返回校验
{   //return sysex.val.checksum;
    return (uint16_t)sysex.storedInputData[29]<<8|sysex.storedInputData[30];
}
