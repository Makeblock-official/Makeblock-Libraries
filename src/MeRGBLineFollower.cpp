/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MeRGBLineFollower
 * \brief   Driver for MeRGBLineFollower module.
 * @file    MeRGBLineFollower.cpp
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2017/09/26
 * @brief   Driver for MeRGBLineFollower module.
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
 *  Payton            2017/09/26        1.0.0         rebuild the old lib.
 * </pre>
 *
 * @example MeRGBLineFollowerTest.ino
 */

/* Includes ------------------------------------------------------------------*/
#include "MeRGBLineFollower.h"

#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

/* Private functions ---------------------------------------------------------*/
#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the MeRGBLineFollower to arduino port,
 * no pins are used or initialized here
 */
MeRGBLineFollower::MeRGBLineFollower(void) : MePort(0)
{
  Device_Address = RGBLINEFOLLOWER_DEFAULT_ADDRESS;
}

/**
 * Alternate Constructor which can call your own function to map the MeRGBLineFollower to arduino port,
 * no pins are used or initialized here, but PWM frequency set to 976 Hz
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
MeRGBLineFollower::MeRGBLineFollower(uint8_t port) : MePort(port)
{
  Device_Address = RGBLINEFOLLOWER_DEFAULT_ADDRESS;
}

/**
 * Alternate Constructor which can call your own function to map the MeRGBLineFollower to arduino port
 * and change the i2c device address
 * no pins are used or initialized here, but PWM frequency set to 976 Hz
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 * \param[in]
 *   address - the i2c address you want to set(0~3), If it's another value,it will be set to 0
 */
MeRGBLineFollower::MeRGBLineFollower(uint8_t port, uint8_t address) : MePort(port)
{
  //address0-11, address1-10, address2-01, address3-00
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  if(address == ADDRESS1)
  {
    digitalWrite(s1,HIGH);
    digitalWrite(s2,HIGH);
  }
  else if(address == ADDRESS2)
  {
    digitalWrite(s1,LOW);
    digitalWrite(s2,HIGH);
  }
  else if(address == ADDRESS3)
  {
    digitalWrite(s1,HIGH);
    digitalWrite(s2,LOW);
  }
  else if(address == ADDRESS4)
  {
    digitalWrite(s1,LOW);
    digitalWrite(s2,LOW);
  }
  else
  { 
    digitalWrite(s1,HIGH);
    digitalWrite(s2,HIGH);
    address = ADDRESS1;
  }

  Device_Address = RGBLINEFOLLOWER_DEFAULT_ADDRESS + address;
}
#else  // ME_PORT_DEFINED
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
MeRGBLineFollower::MeRGBLineFollower(uint8_t AD0, uint8_t INT, uint8_t address)
{
  pinMode(AD0,OUTPUT);
  pinMode(INT,OUTPUT);

  //address0-11, address1-10, address2-01, address3-00
  if(address == ADDRESS1)
  {
    digitalWrite(AD0,HIGH);
    digitalWrite(INT,HIGH);
  }
  else if(address == ADDRESS2)
  {
    digitalWrite(AD0,LOW);
    digitalWrite(INT,HIGH);
  }
  else if(address == ADDRESS3)
  {
    digitalWrite(AD0,HIGH);
    digitalWrite(INT,LOW);
  }
  else if(address == ADDRESS4)
  {
    digitalWrite(AD0,LOW);
    digitalWrite(INT,LOW);
  }
  else
  { 
    digitalWrite(AD0,HIGH);
    digitalWrite(INT,HIGH);
    address = ADDRESS1;
  }

  Device_Address = RGBLINEFOLLOWER_DEFAULT_ADDRESS + address;
}

#endif // ME_PORT_DEFINED

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
 *   Set global variable AD0, INT, s1 and s2
 */
void MeRGBLineFollower::setpin(uint8_t AD0, uint8_t INT)
{
  pinMode(AD0,OUTPUT);
  pinMode(INT,OUTPUT);

#ifdef ME_PORT_DEFINED
  s1 = AD0;
  s2 = INT;
#endif // ME_PORT_DEFINED
}

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
 *   
 */
void MeRGBLineFollower::begin(void)
{
  uint8_t i;
  
  Kp = 0.3;
  study_types = STUDY_IDLE;
  iic_error_count = 0;
  
  Wire.begin();
  delay(10);
}

/**
 * \par Function
 *   getDevAddr
 * \par Description
 *   Get the device address of MeRGBLineFollower.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   The device address of MeRGBLineFollower
 * \par Others
 *   None
 */
uint8_t MeRGBLineFollower::getDevAddr(void)
{
  return Device_Address;
}

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
 *   The ADC value of RGB1
 * \par Others
 *   
 */
uint8_t MeRGBLineFollower::getADCValueRGB1(void)
{
  return adcOutput[RGB1_INDEX];
}

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
 *   The ADC value of RGB2
 * \par Others
 *   
 */
uint8_t MeRGBLineFollower::getADCValueRGB2(void)
{
  return adcOutput[RGB2_INDEX];
}

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
 *   The ADC value of RGB3
 * \par Others
 *   
 */
uint8_t MeRGBLineFollower::getADCValueRGB3(void)
{
  return adcOutput[RGB3_INDEX];
}

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
 *   The ADC value of RGB4
 * \par Others
 *   
 */
uint8_t MeRGBLineFollower::getADCValueRGB4(void)
{
  return adcOutput[RGB4_INDEX];
}

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
void MeRGBLineFollower::setKp(float value)
{
  if((value >= 0) && (value <= 1))
  {
    Kp = value;
  }
}

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
int16_t MeRGBLineFollower::getPositionOffset(void)
{
  return positionOffset;
}

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
uint8_t MeRGBLineFollower::getPositionState(void)
{
  return positionState;
}

/**
 * \par Function
 *   getPositionState
 * \par Description
 *   get turn Value.
 * \param[in]
 *   Speed:set forward speed
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 */
//uint8_t MeRGBLineFollower::getIICErrorCnt(void)
//{
//  return iic_error_count;
//}

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
uint8_t MeRGBLineFollower::getStudyTypes(void)
{
  return study_types;
}

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
 *   
 * \par Others
 */
void MeRGBLineFollower::updataAllSensorValue(void)
{
  int8_t return_value;
  int16_t temp_positionOffset;
  
  /* read data */
  return_value = readData(RGBLINEFOLLOWER_DEVICE_ID_ADDR, &i2cData[0], 8);
  if(return_value == I2C_OK)
  {
    if(i2cData[RGBLINEFOLLOWER_DEVICE_ID_ADDR] == RGBLINEFOLLOWER_DEVICE_ID)
    {
      adcOutput[RGB1_INDEX] = i2cData[RGBLINEFOLLOWER_RGB1_ADDR];
      adcOutput[RGB2_INDEX] = i2cData[RGBLINEFOLLOWER_RGB2_ADDR];
      adcOutput[RGB3_INDEX] = i2cData[RGBLINEFOLLOWER_RGB3_ADDR];
      adcOutput[RGB4_INDEX] = i2cData[RGBLINEFOLLOWER_RGB4_ADDR];
      temp_positionOffset = (int16_t)(i2cData[RGBLINEFOLLOWER_TURNOFFSET_L_ADDR] | ((uint16_t)i2cData[RGBLINEFOLLOWER_TURNOFFSET_H_ADDR]<<8));
      temp_positionOffset = (int16_t)(Kp * temp_positionOffset);
      positionOffset= constrain(temp_positionOffset, -512, 512);
      positionState = i2cData[RGBLINEFOLLOWER_STATE_ADDR] & 0x0F;
      study_types = (i2cData[RGBLINEFOLLOWER_STATE_ADDR]>>4) & 0x03;
    }
    else
    {
      iic_error_count++;  
      //Serial.print("iic error:");
      //Serial.println(iic_error_count);
    }
  }

//  Serial.print("i2cData[0]:");
//  Serial.print(i2cData[0]);
//  Serial.print("  i2cData[1]:");
//  Serial.print(i2cData[1]);
//  Serial.print("  i2cData[2]:");
//  Serial.print(i2cData[2]);
//  Serial.print("  i2cData[3]:");
//  Serial.print(i2cData[3]);
//  Serial.print("  i2cData[4]:");
//  Serial.print(i2cData[4]);
//  Serial.print("  i2cData[5]:");
//  Serial.print(i2cData[5]);
//  Serial.print("  i2cData[6]:");
//  Serial.print(i2cData[6]);
//  Serial.print("  i2cData[7]:");
//  Serial.print(i2cData[7]);
//  Serial.println("  ");
}

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
int8_t MeRGBLineFollower::studyBackground(void)
{
  int8_t return_value = 0;
  uint8_t data = STUDY_BACKGROUND;
  return_value = writeReg(RGBLINEFOLLOWER_STUDY_ADDR, data);
  return(return_value);
}

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
int8_t MeRGBLineFollower::studyTrack(void)
{
  int8_t return_value = 0;
  uint8_t data = STUDY_TRACK;
  return_value = writeReg(RGBLINEFOLLOWER_STUDY_ADDR, data);
  return(return_value);
}

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
int8_t MeRGBLineFollower::setRGBColour(uint8_t colour)
{
  int8_t return_value = 0;
  uint8_t data = colour;
  return_value = writeReg(RGBLINEFOLLOWER_SET_RGB_ADDR, data);
  return(return_value);
}

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
int8_t MeRGBLineFollower::setBackgroundThreshold(uint8_t ch, uint8_t threshold)
{
  int8_t return_value = 0;
  uint8_t data = threshold;

  if(ch > RGB4_INDEX)
  {
    return I2C_ERROR;
  }
  
  return_value = writeReg(RGBLINEFOLLOWER_RGB1_BACKGROUND_THRESHOLD_ADDR + ch, data);
  return(return_value);
}

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
int8_t MeRGBLineFollower::setTrackThreshold(uint8_t ch, uint8_t threshold)
{
  int8_t return_value = 0;
  uint8_t data = threshold;

  if(ch > RGB4_INDEX)
  {
    return I2C_ERROR;
  }
  
  return_value = writeReg(RGBLINEFOLLOWER_RGB1_TRACK_THRESHOLD_ADDR + ch, data);
  return(return_value);
}

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
uint8_t MeRGBLineFollower::getBackgroundThreshold(uint8_t ch)
{
  int8_t return_value = 0;
  uint8_t reg = RGBLINEFOLLOWER_RGB1_BACKGROUND_THRESHOLD_ADDR + ch;
  
  if(ch <= RGB4_INDEX)
  {
    return_value = readData(reg, &i2cData[reg], 1);
    delay(50);
    if(return_value == I2C_OK)
    {
      return i2cData[reg];
    }
    return i2cData[reg];
  }
  
  return 0;
}

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
uint8_t MeRGBLineFollower::getTrackThreshold(uint8_t ch)
{
  int8_t return_value = 0;
  uint8_t reg = RGBLINEFOLLOWER_RGB1_TRACK_THRESHOLD_ADDR + ch;
  
  if(ch <= RGB4_INDEX)
  {
    return_value = readData(reg, &i2cData[reg], 1);
    if(return_value == I2C_OK)
    {
      return i2cData[reg];
    }
    return i2cData[reg];
  }
  
  return 0;
}

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
void MeRGBLineFollower::getFirmwareVersion(char *buffer)
{
  int8_t return_value = 0;
  uint8_t reg = RGBLINEFOLLOWER_GET_VERSION_ADDR;

  return_value = readData(reg, &i2cData[reg], 8);
  if(return_value == I2C_OK)
  {
    memcpy(buffer, &i2cData[reg], 8);
  }
  memcpy(buffer, &i2cData[reg], 8);
}

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
int8_t MeRGBLineFollower::writeReg(uint8_t reg, uint8_t data)
{
  int8_t return_value = 0;
  return_value = writeData(reg, &data, 1);
  delay(5);
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
int8_t MeRGBLineFollower::readData(uint8_t start, uint8_t *buffer, uint8_t size)
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
int8_t MeRGBLineFollower::writeData(uint8_t start, const uint8_t *pData, uint8_t size)
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
void MeRGBLineFollower::loop(void)
{
  if(millis() - updata_time > 8)  
  {
    updata_time = millis();
    updataAllSensorValue();
  }
}
