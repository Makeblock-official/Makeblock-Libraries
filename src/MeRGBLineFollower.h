/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MeRGBLineFollower
 * \brief   Driver for MeRGBLineFollower module.
 * @file   MeRGBLineFollower.h
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2017/09/26
 * @brief   Header for MeRGBLineFollower.cpp module.
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
 * This file is a drive for MeRGBLineFollower module, It supports MeRGBLineFollower V1.0 device provided
 * by MakeBlock.
 *
 * \par Method List:
 *
 *    1.void MeRGBLineFollower::setpin(uint8_t AD0, uint8_t INT)
 *	 	2.void MeRGBLineFollower::begin(void)
 *    3.uint8_t MeRGBLineFollower::getDevAddr(void)
 *    4.uint8_t MeRGBLineFollower::getADCValueRGB1(void)
 *    5.uint8_t MeRGBLineFollower::getADCValueRGB2(void)
 *    6.uint8_t MeRGBLineFollower::getADCValueRGB3(void)
 *    7.uint8_t MeRGBLineFollower::getADCValueRGB4(void)
 *    8.void MeRGBLineFollower::setKp(float value)
 *    9.int16_t MeRGBLineFollower::getPositionOffset(void)
 *    10.uint8_t MeRGBLineFollower::getPositionState(void)
 *    11.uint8_t MeRGBLineFollower::getIicErrorCnt(void)
 *    12.uint8_t MeRGBLineFollower::getStudyTypes(void)
 *    13.void MeRGBLineFollower::updataAllSensorValue(void)
 *    14.int8_t MeRGBLineFollower::readData(uint8_t start, uint8_t *buffer, uint8_t size)
 *    15.int8_t MeRGBLineFollower::writeReg(uint8_t reg, uint8_t data)
 *    16.int8_t MeRGBLineFollower::writeData(uint8_t start, const uint8_t *pData, uint8_t size)
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 *  Payton            2017/09/26          1.0.0         rebuild the old lib.
 * </pre>
 *
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MeRGBLineFollower_H
#define MeRGBLineFollower_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>
#include "MeConfig.h"
#ifdef ME_PORT_DEFINED
#include "MePort.h"
#endif // ME_PORT_DEFINED

/* Exported macro ------------------------------------------------------------*/
#define I2C_ERROR                  						(-1)
#define I2C_OK                    						(0)
#define RGBLINEFOLLOWER_DEFAULT_ADDRESS      (0x20)
#define RGBLINEFOLLOWER_DEVICE_ID							(0xFA)

//RGBLineFollower IIC Register Address
#define RGBLINEFOLLOWER_DEVICE_ID_ADDR                  (0x00)
#define RGBLINEFOLLOWER_RGB1_ADDR                       (0x01)
#define RGBLINEFOLLOWER_RGB2_ADDR                       (0x02)
#define RGBLINEFOLLOWER_RGB3_ADDR                       (0x03)
#define RGBLINEFOLLOWER_RGB4_ADDR                       (0x04)
#define RGBLINEFOLLOWER_TURNOFFSET_L_ADDR               (0x05)
#define RGBLINEFOLLOWER_TURNOFFSET_H_ADDR               (0x06)
#define RGBLINEFOLLOWER_STATE_ADDR       	              (0x07)
#define RGBLINEFOLLOWER_RGB1_BACKGROUND_THRESHOLD_ADDR  (0x08)
#define RGBLINEFOLLOWER_RGB2_BACKGROUND_THRESHOLD_ADDR  (0x09)
#define RGBLINEFOLLOWER_RGB3_BACKGROUND_THRESHOLD_ADDR  (0x0A)
#define RGBLINEFOLLOWER_RGB4_BACKGROUND_THRESHOLD_ADDR  (0x0B)
#define RGBLINEFOLLOWER_RGB1_TRACK_THRESHOLD_ADDR       (0x0C)
#define RGBLINEFOLLOWER_RGB2_TRACK_THRESHOLD_ADDR       (0x0D)
#define RGBLINEFOLLOWER_RGB3_TRACK_THRESHOLD_ADDR       (0x0E)
#define RGBLINEFOLLOWER_RGB4_TRACK_THRESHOLD_ADDR       (0x0F)
#define RGBLINEFOLLOWER_GET_VERSION_ADDR                (0x10)
#define RGBLINEFOLLOWER_STUDY_ADDR                      (0x31)
#define RGBLINEFOLLOWER_SET_RGB_ADDR                    (0x32)


//RGBLineFollower index
#define RGB1_INDEX		0
#define RGB2_INDEX		1
#define RGB3_INDEX		2
#define RGB4_INDEX		3

//RGBLineFollower number
#define RGBLINEFOLLOWER_NUM		         (0x04)

//address
#define ADDRESS1		0
#define ADDRESS2		1
#define ADDRESS3		2
#define ADDRESS4		3

//study types
#define STUDY_IDLE          0
#define STUDY_BACKGROUND    1
#define STUDY_TRACK         2


//RGB culour
#define RGB_COLOUR_RED      1
#define RGB_COLOUR_GREEN    2
#define RGB_COLOUR_BLUE     3

/**
 * Class: MeRGBLineFollower
 * \par Description
 * Declaration of Class MeRGBLineFollower
 */
#ifndef ME_PORT_DEFINED
class MeRGBLineFollower
#else // !ME_PORT_DEFINED
class MeRGBLineFollower : public MePort
#endif // !ME_PORT_DEFINED
{
public:
#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the MeCompass to arduino port,
 * no pins are used or initialized here
 */
  MeRGBLineFollower(void);

/**
 * Alternate Constructor which can call your own function to map the MeCompass to arduino port,
 * no pins are used or initialized here, but PWM frequency set to 976 Hz
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
  MeRGBLineFollower(uint8_t port);

/**
 * Alternate Constructor which can call your own function to map the MeCompass to arduino port
 * and change the i2c device address
 * no pins are used or initialized here, but PWM frequency set to 976 Hz
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 * \param[in]
 *   address - the i2c address you want to set
 */
  MeRGBLineFollower(uint8_t port, uint8_t address);
#else
/**
 * Alternate Constructor which can call your own function to map the AD0 and INT to arduino port,
 * no pins are used or initialized here
 * \param[in]
 *   AD0 - arduino gpio number
 * \param[in]
 *   INT - arduino gpio number
  * \param[in]
 *   address - the i2c address you want to set(0~3), If it's another value,it will be set to 0
 */
 MeRGBLineFollower(uint8_t AD0, uint8_t INT, uint8_t address);

#endif  //  ME_PORT_DEFINED
/**
 * \par Function
 *   setpin
 * \par Description
 *   Set the PIN of the button module.
 * \param[in]
 *   AD0 - pin mapping for arduino
 * \param[in]
 *   INT - pin mapping for arduino
 * \par Output
 *   None
 * \return
 *   None.
 * \par Others
 *   Set global variable _AD0, _INT, s1 and s2
 */
  void setpin(uint8_t AD0, uint8_t INT);

/**
 * \par Function
 *   begin
 * \par Description
 *   Initialize the MeRGBLineFollower.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void begin();

/**
 * \par Function
 *   getDevAddr
 * \par Description
 *   Get the device address of RGBLineFollower.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   The device address of RGBLineFollower
 * \par Others
 *   None
 */
  uint8_t getDevAddr(void);

/**
 * \par Function
 *   getADCValueRGB1
 * \par Description
 *   Get the RGB1 sensor ADC value.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   The ADC value of RGB1:0~255
 * \par Others
 *   
 */
	uint8_t getADCValueRGB1(void);

/**
 * \par Function
 *   getADCValueRGB2
 * \par Description
 *   Get the RGB2 sensor ADC value.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   The ADC value of RGB2:0~255
 * \par Others
 *   
 */
	uint8_t getADCValueRGB2(void);

/**
 * \par Function
 *   getADCValueRGB3
 * \par Description
 *   Get the RGB3 sensor ADC value.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   The ADC value of RGB3:0~255
 * \par Others
 *   
 */
	uint8_t getADCValueRGB3(void);

/**
 * \par Function
 *   getADCValueRGB4
 * \par Description
 *   Get the RGB4 sensor ADC value.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   The ADC value of RGB4:0~255
 * \par Others
 *   
 */
	uint8_t getADCValueRGB4(void);

/**
 * \par Function
 *   setKp
 * \par Description
 *   set Kp.
 * \param[in]
 *   value: 0 ~ 1，turn sensitivity
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *  
 */
	void setKp(float value);

/**
 * \par Function
 *   getPositionOffset
 * \par Description
 *   get turn Value.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   Position offset Value,-512 ~ 512
      <0,in right
      >0,in left
 * \par Others
 *   |---------------------------|
     |      |      |      |      |
     |RGB1  |RGB2  |RGB3  |RGB4  |
     |      |      |      |      |
     |---------------------------|
 */
	int16_t getPositionOffset(void);


/**
 * \par Function
 *   getPositionState
 * \par Description
 *   get turn Value.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   Position StateValue,0x00~0x0F,bit0-RGB1,bit1-RGB2 ,bit2-RGB3  ,bit3-RGB4 
 * \par Others
 *   |---------------------------|
     |      |      |      |      |
     |RGB1  |RGB2  |RGB3  |RGB4  |
     |      |      |      |      |
     |---------------------------|
 */
	uint8_t getPositionState(void);
	
/**
   * \par Function
   *   getStudyTypes
   * \par Description
   *   get turn Value.
   * \param[in]
   *   None
   * \par Output
   *   None
   * \return
   *   Study types
  *    0. STUDY_IDLE
  *    1. STUDY_BACKGROUND
  *    2. STUDY_TRACK
   * \par Others
   */
  uint8_t getStudyTypes(void);

/**
 * \par Function
 *   updataAllSensorValue
 * \par Description
 *   updata All Sensor Value.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 */
	void updataAllSensorValue(void);
	
/**
   * \par Function
   *   studyBackground
   * \par Description
   *   study background.
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
   */
  int8_t studyBackground(void);

/**
 * \par Function
 *   studyTrack
 * \par Description
 *   study track.
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
 */
  int8_t studyTrack(void);
  
/**
 * \par Function
 *   getThreshold
 * \par Description
 *   Get the threshold between background and track.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   Return the threshold. 0~255
 * \par Others
 */
int8_t getThreshold(void);

/**
 * \par Function
 *   setRGBColour
 * \par Description
 *   Set the RGB colour.
 * \param[in]
 *   types:RGB_COLOUR_RED, RGB_COLOUR_GREEN, RGB_COLOUR_BLUE
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
 */
 int8_t setRGBColour(uint8_t colour);

 /**
  * \par Function
  *   setBackgroundThreshold
  * \par Description
  *   set Background Threshold.
  * \param[in]
  *   ch:RGB index
  *      0.RGB1_INDEX
  *      1.RGB2_INDEX
  *      2.RGB3_INDEX
  *      3.RGB4_INDEX
  *   threshold:0~255
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
  */
 int8_t setBackgroundThreshold(uint8_t ch, uint8_t threshold);
 
 /**
  * \par Function
  *   setTrackThreshold
  * \par Description
  *   set track Threshold.
  * \param[in]
  *   ch:RGB index
  *      0.RGB1_INDEX
  *      1.RGB2_INDEX
  *      2.RGB3_INDEX
  *      3.RGB4_INDEX
  *   threshold:0~255
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
  */
 int8_t setTrackThreshold(uint8_t ch, uint8_t threshold);

/**
  * \par Function
  *   getBackgroundThreshold
  * \par Description
  *   Get the background threshold .
  * \param[in]
  *   ch:RGB index
  *      0.RGB1_INDEX
  *      1.RGB2_INDEX
  *      2.RGB3_INDEX
  *      3.RGB4_INDEX
  * \par Output
  *   None
  * \return
  *   Return the background threshold. 0~255
  * \par Others
  */
 uint8_t getBackgroundThreshold(uint8_t ch);
 
/**
 * \par Function
 *   getTrackThreshold
 * \par Description
 *   Get the track threshold .
 * \param[in]
 *   ch:RGB index
 *      0.RGB1_INDEX
 *      1.RGB2_INDEX
 *      2.RGB3_INDEX
 *      3.RGB4_INDEX
 * \par Output
 *   None
 * \return
 *   Return the track threshold. 0~255
 * \par Others
 */
uint8_t getTrackThreshold(uint8_t ch);

/**
 * \par Function
 *   getFirmwareVersion
 * \par Description
 *   Get Firmware Version.
 * \param[in]
 *   None
 * \par Output
 *   buffer: for storage version, length greater than 8
 * \return
 *   None
 * \par Others
 */
void getFirmwareVersion(char *buffer);

/**
	 * \par Function
	 *	 loop
	 * \par Description
	 *	RGBLineFollower loop.
	 * \param[in]
	 *	 None
	 * \par Output
	 *	 None
	 * \return
	 *	 
	 * \par Others
	 */
  void loop(void);

private:
  uint8_t i2cData[25];
  uint8_t adcOutput[RGBLINEFOLLOWER_NUM];
  uint8_t Device_Address;
  float Kp;
  int16_t positionOffset;
  uint8_t positionState;
  uint8_t study_types;
  uint32_t iic_error_count;
  unsigned long updata_time;
  
/**
 * \par Function
 *   writeReg
 * \par Description
 *   Write the registor of i2c device.
 * \param[in]
 *   reg - the address of registor.
 * \param[in]
 *   data - the data that will be written to the registor.
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
 *   To set the registor for initializing.
 */
  int8_t writeReg(uint8_t reg, uint8_t data);

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
};
#endif //  MeRGBLineFollower_H
