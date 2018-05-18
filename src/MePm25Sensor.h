/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class MePm25Sensor
 * \brief   Driver for Me PM2.5 sensor device.
 * @file    MePm25Sensor.h
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2016/12/12
 * @brief   Header for for MePm25Sensor.cpp module
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
 * Zzipeng          2016/12/13     1.0.0            Build the new.
 * </pre>
 *
 */

#ifndef MePm25Sensor_H
#define MePm25Sensor_H

#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>
#include "MeConfig.h"
#include "MeSerial.h"

#define START_SYSEX1 0x32
#define START_SYSEX2 0x3d
#define LENGTH       28//帧长度

//#define DEFAULT_UART_BUF_SIZE      32

typedef struct{
//  uint16_t head;//0x323d
  uint16_t len;
  uint16_t data1;
  uint16_t data2;
  uint16_t data3;
  uint16_t data4;
  uint16_t data5;
  uint16_t data6;
  uint16_t data7;
  uint16_t data8;
  uint16_t data9;
  uint16_t data10;//default
  uint16_t data11;//default
  uint16_t data12;//default
  uint16_t data13;//default
  uint16_t checksum;
}PM25DATASTRUCT;


union PM25DATAUINO{
  uint8_t storedInputData[31];
  PM25DATASTRUCT val;
};

#ifdef ME_PORT_DEFINED
#include "MePort.h"
#endif /* ME_PORT_DEFINED */

/**
 * Class: MePm25Sensor
 * \par Description
 * Declaration of Class MePm25Sensor.
 */
#ifndef ME_PORT_DEFINED
class MePm25Sensor
#else /* !ME_PORT_DEFINED */
class MePm25Sensor : public MeSerial
#endif 
/* !ME_PORT_DEFINED */
{
public:
#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the Me Smart Servo to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
  MePm25Sensor();

/**
 * Alternate Constructor which can call your own function to map the Me Smart Servo to arduino port,
 * If the hardware serial was selected, it will used the hardware serial.
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
  MePm25Sensor(uint8_t port);
#else // ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the Me Smart Servo to arduino port,
 * If the hardware serial was selected, it will used the hardware serial.
 * \param[in]
 *   receivePin - the rx pin of serial(arduino port)
 * \param[in]
 *   transmitPin - the tx pin of serial(arduino port)
 * \param[in]
 *   inverse_logic - Whether the Serial level need inv.
 */
  MePm25Sensor(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic);
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
  uint16_t readPm1_0Concentration(void);//PM1.0 ug/m^3
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
  uint16_t readPm2_5Concentration(void);//PM2.5 ug/m^3
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
  uint16_t readPm10Concentration(void);//PM10 ug/m^3
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
  uint16_t read0_3NumIn100ml(void);//0.1L空气中直径在0.3um的颗粒物个数
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
  uint16_t read0_5NumIn100ml(void);//0.1L空气中直径在0.5um的颗粒物个数
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
  uint16_t read1_0NumIn100ml(void);//0.1L空气中直径在1.0um的颗粒物个数
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
  uint16_t read2_5NumIn100ml(void);//0.1L空气中直径在2.5um的颗粒物个数
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
  uint16_t read5_0NumIn100ml(void);//0.1L空气中直径在5.0um的颗粒物个数
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
  uint16_t read10NumIn100ml(void);//0.1L空气中直径在10um的颗粒物个数
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
  uint16_t setOuputCompatibility(void);//设置输出兼容
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
  uint16_t turnOnFanLaser(void);//打开风扇激光
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
  uint16_t turnOffFanLaser(void);//关闭风扇激光
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
  uint16_t OutputIntimeOn(void);//定时输出开
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
  uint16_t OutputIntimeOff(void);//定时输出关
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
  uint16_t setOutputIntimePeriod(uint16_t settime);//设定定时输出间隔
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
  uint16_t askForData(void);//请求输出数据
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
  uint16_t coreSelfTest(void);//滤芯检测
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
  uint16_t returnCoreSelfTest(void);//返回自检结果
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
  uint16_t setCoreSelfTestTime(uint8_t settime);//设定检测时间 0~255s
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
  uint16_t setCoreEfficient(uint8_t efficent);//设定滤芯使用效率
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
  void rxloop(void);
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
  uint16_t returnlen(void);//返回有效数据长度
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
  uint16_t returnchecksum(void);//返回校验值
 private:
    union PM25DATAUINO sysex,rxbuf;
   // uint8_t buffer_flag;
    volatile int16_t sysexBytesRead;
    // volatile uint16_t resFlag;
    // volatile servo_device_type servo_dev_list[8];
    // volatile long cmdTimeOutValue;
    volatile boolean parsingSysex;
};

#endif
