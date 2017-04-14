/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   Mecolor
 * \brief   Driver for MeColorSensor module.
 * @file    MeColorSensor.h
 * @author  MakeBlock
 * @version V1.0.2
 * @date    2017/01/17
 * @brief   Header for MeColorSensor.cpp module.
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
 *    10.uint8_t  MeColorSensor::ColorIdentify(void)
 *    11.long MeColorSensor::ReturnColorCode(void)
 *    12.uint16_t MeColorSensor::calculateColorTemperature(void)
 *    13.uint16_t MeColorSensor::calculateLux(void)
 *    14.int8_t MeColorSensor::writeReg(int16_t reg, uint8_t data)
 *    15.int8_t MeColorSensor::readData(uint8_t start, uint8_t *buffer, uint8_t size)
 *    16.int8_t MeColorSensor::writeData(uint8_t start, const uint8_t *pData, uint8_t size)
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
 */
#ifndef _MeColorSensor_H_
#define _MeColorSensor_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>
#include "MeConfig.h"
#include "Wire.h"
 #ifdef ME_PORT_DEFINED
#include "MePort.h"
 #endif // ME_PORT_DEFINED

/* Exported macro ------------------------------------------------------------*/
#define I2C_ERROR                       (-1)
#define COLORSENSOR_DEFAULT_ADDRESS     (0x38)//011 1000
/* register List  ------------------------------------------------------------*/
#define SYSTEM_CONTROL    (0X40)
#define MODE_CONTROL1     (0X41)
#define MODE_CONTROL2     (0X42)
#define MODE_CONTROL3     (0X44)
#define RED_DATA_LSBs     (0X50)
#define RED_DATA_MSBs     (0X51)
#define GREEN_DATA_LSBs   (0X52)
#define GREEN_DATA_MSBs   (0X53)
#define BLUE_DATA_LSBs    (0X54)
#define BLUE_DATA_MSBs    (0X55)
#define CLEAR_DATA_LSBs   (0X56)
#define CLEAR_DATA_MSBs   (0X57)
#define DINT_DATA_LSBs    (0X58)
#define DINT_DATA_MSBs    (0X59)
#define INTERRUPT         (0X60)
#define PERSISTENCE       (0X61)
#define TH_LSBs           (0X62)
#define TH_MSBs           (0X63)
#define TL_LSBs           (0X64)
#define TL_MSBs           (0X65)
#define MANUFACTURER_ID   (0X92)
/************ define parameter for register ************/
#define SW_RESET               (1 << 7)
#define INT_RESET              (1 << 6)

/* REG_MODECONTROL1(0x41) */
#define MEASURE_160MS          (0x00)
#define MEASURE_320MS          (0x01)
#define MEASURE_640MS          (0x02)
#define MEASURE_1280MS         (0x03)
#define MEASURE_2560MS         (0x04)
#define MEASUREMENT_MAX        (0x05)
/*RGB list*/
typedef enum
{
  WHITE=0,
  PINKE,
  RED,
  ORANGE,
  YELLOW,
  GREEN,
  CYAN,
  BLUE,
  PURPLE,
  BLACK,
  GOLD,
}COLORTYPES;
/*
 * Class: MeColorSensor
 * \par Description
 * Declaration of Class MeColorSensor
 */

#ifndef ME_PORT_DEFINED
class MeColorSensor
#else // !ME_PORT_DEFINED
class MeColorSensor : public MePort
#endif // !ME_PORT_DEFINED
{
public:
#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the MeCompass to arduino port,
 * no pins are used or initialized here
 */
  MeColorSensor(void);

/**
 * Alternate Constructor which can call your own function to map the MeCompass to arduino port,
 * no pins are used or initialized here, but PWM frequency set to 976 Hz
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
  MeColorSensor(uint8_t port);

/**
 * Alternate Constructor which can call your own function to map the MeCompass to arduino port
 * and change the i2c device address
 * no pins are used or initialized here, but PWM frequency set to 976 Hz
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 * \param[in]
 *   address - the i2c address you want to set
 */
  MeColorSensor(uint8_t port, uint8_t address);
#else
/**
 * Alternate Constructor which can call your own function to map the _AD0 and _INT to arduino port,
 * no pins are used or initialized here
 * \param[in]
 *   _AD0 - arduino gpio number
 * \param[in]
 *   _INT - arduino gpio number
 */
  MeColorSensor(uint8_t AD0, uint8_t INT);

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
  MeColorSensor(uint8_t AD0, uint8_t INT, uint8_t address);
#endif  //  ME_PORT_DEFINED
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
  void SensorInit(void);
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
  uint8_t ReportId(void);
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
  void ColorDataRead(void);
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
 long ReturnColorCode(void);
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
 uint8_t ColorIdentify(void);
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
 uint8_t Returnresult(void);
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
 uint8_t ReturnGrayscale(void);
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
 uint16_t ReturnColorhue(void);
/*!
    @brief  Converts the raw R/G/B values to color temperature in degrees
            Kelvin
*/
/**************************************************************************/
// uint16_t calculateColorTemperature(void);
/**************************************************************************/
/*!
    @brief  Converts the raw R/G/B values to lux
*/
/**************************************************************************/
// uint16_t calculateLux(void);
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
 /**************************************************************************/
  uint16_t ReturnRedData(void);
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
  uint16_t ReturnGreenData(void);
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
  uint16_t ReturnBlueData(void);
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
  uint16_t ReturnClearData(void);
/*
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
  void TurnOnLight(void);
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
  void TurnOffLight(void);
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
  void TurnOffmodule(void);
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
  void TurnOnmodule(void);
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
  int8_t writeReg(int16_t reg, uint8_t data);
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
  int8_t readData(uint8_t start, uint8_t *buffer, uint8_t size);

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
  int8_t writeData(uint8_t start, const uint8_t *pData, uint8_t size);
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
  uint8_t MAX(uint8_t r,uint8_t g,uint8_t b);
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
  uint8_t MIN(uint8_t r,uint8_t g,uint8_t b);
/***********************************/
  uint8_t Device_Address;
  uint16_t Redvalue;
  uint16_t Greenvalue;
  uint16_t Bluevalue;
  uint16_t Clearvalue;
private:
  volatile uint8_t  _AD0;
  volatile uint8_t  _INT;
  volatile uint8_t  _S1;
  volatile uint8_t  _S2;
};
#endif

