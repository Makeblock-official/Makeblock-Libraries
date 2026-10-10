/**
 * \par Copyright (C), 2012-2017, MakeBlock
 * \class   MeAudioPlayer
 * \brief   Driver for MeAudioPlayer module.
 * @file    MeAudioPlayer.h
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2017/11/17
 * @brief   Header for MeAudioPlayer.cpp module.
 *
 * \par Copyright
 * This software is Copyright (C), 2012-2016, MakeBlock. Use is subject to license
 * conditions. The main licensing options available are GPL V2 or Commercial:
 *
 * \par Open Source Licensing GPL V2
 * This is the appropriate option if you want to share the source code of your
 * application with everyone you distribute it to, and you also want to give them
 * the right to share who uses it. If you wish to use this software under Open
 * Source Licensing, you must contribute all your source code to the open source
 * community in accordance with the GPL Version 2 when your application is
 * distributed. See http://www.gnu.org/copyleft/gpl.html
 *
 * \par Description
 * This file is a drive for MeAudioPlayer module, It supports MeAudioPlayer V1.0 device provided
 * by MakeBlock.
 *
 * \par Method List:
 *
 *   void MeAudioPlayer::PlayerInit(void)
 *   void MeAudioPlayer::playMusicFileIndex(uint16_t music_index);
 *   void MeAudioPlayer::playMusicFileName(String str);  
 *   void MeAudioPlayer::pauseMusic(void);
 *   void MeAudioPlayer::stopMusic(void);
 *   void MeAudioPlayer::playNextMusic(void);
 *   void MeAudioPlayer::playPrevMusic(void);
 *   void MeAudioPlayer::setMusicVolume(uint8_t vol);
 *   void MeAudioPlayer::setMusicPlayMode(uint8_t mode);
 *   void MeAudioPlayer::startRecordingFileName(String str);
 *   void MeAudioPlayer::stopRecording(void);
 *   void MeAudioPlayer::volumeUp(void);
 *   void MeAudioPlayer::volumeDown(void);
 *   void MeAudioPlayer::deleteFileName(String str);
 *   void MeAudioPlayer::deleteAllMusicFile(void);
 * 
 * \par History:
 * <pre>
 * "<Author>"         "<Time>"        "<Version>"        "<Descr>"
 *  Lanweiting      2017/11/15          1.0.0         Build the files.
 * </pre>
 *
 * @example MeAudioPlayerTest.ino
*/

#include "MeAudioPlayer.h"

/* Private functions ---------------------------------------------------------*/
#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the MeAudioPlayer to arduino port,
 * no pins are used or initialized here
 */
MeAudioPlayer::MeAudioPlayer(void) : MePort(0)
{
  Device_Address = AUDIOPLAYER_DEFAULT_ADDRESS;
}

/**
 * Alternate Constructor which can call your own function to map the MeAudioPlayer to arduino port,
 * \param[in]
 *   port - RJ25 port number
 */
MeAudioPlayer::MeAudioPlayer(uint8_t port) : MePort(port)
{
  Device_Address = AUDIOPLAYER_DEFAULT_ADDRESS;
}

/**
 * Alternate Constructor which can call your own function to map the MeAudioPlayer to arduino port
 * and change the i2c device address
 * \param[in]
 *   port - RJ25 port number
 * \param[in]
 *   address - the i2c address you want to set
 */
MeAudioPlayer::MeAudioPlayer(uint8_t port, uint8_t address) : MePort(port)
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
MeAudioPlayer::MeAudioPlayer(uint8_t _AD0, uint8_t _INT)
{
  Device_Address = AUDIOPLAYER_DEFAULT_ADDRESS;
  _AD0 = _AD0;
  _INT = _INT;
}

#endif /* ME_PORT_DEFINED */


/**
 * \par Function
 *    sendCmd
 * \par Description
 *    send the datas to serial com.
 * \param[in]
 *    data pionter,len
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeAudioPlayer::sendCmd(uint8_t *sendcmd, uint8_t len)
{
  int8_t i;
  
/*  if(millis() - nextCmd_time > MAX_TIMEOUT)  
  {
    Wire.beginTransmission(Device_Address);
    for(i = 0; i < len ; i++)
    {
      Wire.write(sendcmd[i]);
    }
    Wire.endTransmission(true);
    nextCmd_time = millis();
  }*/
  
  Wire.beginTransmission(Device_Address);
  for(i = 0; i < len ; i++)
  {
    Wire.write(sendcmd[i]);
  }
  Wire.endTransmission(true);
  
}

/**
 * \par Function
 *    checkSum
 * \par Description
 *    return check And result
 * \param[in]
 *    data pionter,len
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
uint8_t MeAudioPlayer::checkSum(uint8_t* buffer, uint8_t len)
{
  uint8_t result = 0x00;
  for (int i = 1; i < len; i++)
  {
    result += *(buffer+i);
  }
  return result;
}

/**
 * \par Function
 *   PlayerInit
 * \par Description
 *   Initialize the MeAudioPlayer.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
void MeAudioPlayer::PlayerInit(void)
{
  // readbuffer[15] = {0};
  writebuffer[27] =  {0};
  Wire.begin();
  delay(3000);
  prevCmd_time = millis();
}
/************************************************************

      Playing Music Control Function

*************************************************************/
/**
 * \par Function
 *    playMusicFileIndex
 * \par Description
 *    play a mucsic File with it index on root
 * \param[in]
 *    uint16_t index
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeAudioPlayer::playMusicFileIndex(uint16_t music_index)
{
  uint8_t len = 0x05;
  nextCmd_time = millis();
  if (nextCmd_time - prevCmd_time < MAX_TIMEOUT)
  {
    delay( MAX_TIMEOUT - (nextCmd_time - prevCmd_time));
  }
  if (music_index > 0xFFFF)
  {
    music_index = 0xFFFF;
  }
  writebuffer[0] = MECODE_START;
  writebuffer[1] = len;
  writebuffer[2] = MEPLAY_FILE_INDEX;
  writebuffer[3] = music_index >> 8;
  writebuffer[4] = (uint8_t)music_index;
  writebuffer[5] = checkSum(writebuffer, len);
  writebuffer[6] = MECODE_END;
  
  sendCmd(writebuffer, len+2);
  prevCmd_time = millis();
}

/**
 * \par Function
 *    playMusicFileName
 * \par Description
 *    play a mucsic File with it name
 * \param[in]
 *    String str
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */

void MeAudioPlayer::playMusicFileName(String str)
{
  uint8_t len = 0;
  uint8_t str_len = 0;
  uint8_t i = 0;
  char temp[10];
  char temp_ch;
  str_len = str.length();

  if (str_len > 8)
  {
    len = 8;   
  }
  else
  {
    len = str_len;
  }
  for (int i = 0; i < len; i++)
  {
    temp_ch = str.charAt(i);
    temp[i] = (temp_ch >= 'a' && temp_ch <= 'z') ? (temp_ch - 32) : temp_ch;  
  }
  
  
  nextCmd_time = millis();
  if (nextCmd_time - prevCmd_time < MAX_TIMEOUT)
  {
    delay( MAX_TIMEOUT - (nextCmd_time - prevCmd_time));
  }
  
  if (str_len > 8)
  {
    len = 11;
    writebuffer[0] = MECODE_START;
    writebuffer[1] = len;
    writebuffer[2] = MEPLAY_FILE_NAME;
  
    for (int k = 0; k < 6; k++)
    {
      writebuffer[3 + k] = (uint8_t)temp[k];
    }
    writebuffer[len - 2] = 0x7E;
    writebuffer[len - 1] = 0x31;
    writebuffer[len] = (uint8_t)checkSum(writebuffer, len);
    writebuffer[len + 1] = MECODE_END;
  }
  else
  {
    len = str_len + 3;
    writebuffer[0] = MECODE_START;
    writebuffer[1] = len;
    writebuffer[2] = MEPLAY_FILE_NAME;
  
    for (int k = 0; k < str_len+1; k++)
    {
      writebuffer[3 + k] = (uint8_t)temp[k];
    }
    writebuffer[len] = (uint8_t)checkSum(writebuffer, len);
    writebuffer[len+1] = MECODE_END;
  }

  sendCmd(writebuffer, len + 2);
  prevCmd_time = millis();
}  
/**
 * \par Function
 *    pauseMusic
 * \par Description
 *    pause mucsic File
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeAudioPlayer::pauseMusic(void)
{
  uint8_t len = 0x03;
  nextCmd_time = millis();
  if (nextCmd_time - prevCmd_time < MAX_TIMEOUT)
  {
    delay( MAX_TIMEOUT - (nextCmd_time - prevCmd_time));
  }
  writebuffer[0] = MECODE_START;
  writebuffer[1] = len;
  writebuffer[2] = MEPAUSE_MUSIC;
  writebuffer[3] = checkSum(writebuffer, len);
  writebuffer[4] = MECODE_END;
  sendCmd(writebuffer, 5);
  prevCmd_time = millis();
}
/**
 * \par Function
 *    stopMusic
 * \par Description
 *    stop play mucsic File
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeAudioPlayer::stopMusic(void)
{
  uint8_t len = 0x03;
  nextCmd_time = millis();
  if (nextCmd_time - prevCmd_time < MAX_TIMEOUT)
  {
    delay( MAX_TIMEOUT - (nextCmd_time - prevCmd_time));
  }
  writebuffer[0] = MECODE_START;
  writebuffer[1] = len;
  writebuffer[2] = MESTOP_MUSIC;
  writebuffer[3] = checkSum(writebuffer, len);
  writebuffer[4] = MECODE_END;
  sendCmd(writebuffer, 5);
  prevCmd_time = millis();
}
/**
 * \par Function
 *    playNextMusic
 * \par Description
 *    play next mucsic File
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeAudioPlayer::playNextMusic(void)
{
  uint8_t len = 0x03;
  nextCmd_time = millis();
  if (nextCmd_time - prevCmd_time < MAX_TIMEOUT)
  {
    delay( MAX_TIMEOUT - (nextCmd_time - prevCmd_time));
  }
  writebuffer[0] = MECODE_START;
  writebuffer[1] = len;
  writebuffer[2] = MEPLAY_NEXT;
  writebuffer[3] = checkSum(writebuffer, len);
  writebuffer[4] = MECODE_END;
  sendCmd(writebuffer, 5);
  prevCmd_time = millis();
}
/**
 * \par Function
 *    playPrevMusic
 * \par Description
 *    play prev mucsic File
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeAudioPlayer::playPrevMusic(void)
{
  uint8_t len = 0x03;
  nextCmd_time = millis();
  if (nextCmd_time - prevCmd_time < MAX_TIMEOUT)
  {
    delay( MAX_TIMEOUT - (nextCmd_time - prevCmd_time));
  }
  writebuffer[0] = MECODE_START;
  writebuffer[1] = len;
  writebuffer[2] = MEPLAY_PREV;
  writebuffer[3] = checkSum(writebuffer, len);
  writebuffer[4] = MECODE_END;
  sendCmd(writebuffer, 5);
  prevCmd_time = millis();
}
/**
 * \par Function
 *    setMusicVolume
 * \par Description
 *    set play Music Volume value
 * \param[in]
 *    uint8_t vol(00~100),default:66
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeAudioPlayer::setMusicVolume(uint8_t vol)
{
  uint8_t len = 0x04;
  vol = (uint8_t)(vol * 0.24);
  if (vol > 24)
  {
    vol = 24;
  }
  if (vol < 0)
  {
    vol = 0;
  }
  writebuffer[0] = MECODE_START;
  writebuffer[1] = len;
  writebuffer[2] = MEVOLUME_CTL;
  writebuffer[3] = vol;
  writebuffer[4] = checkSum(writebuffer, len);
  writebuffer[5] = MECODE_END;
  sendCmd(writebuffer, 6);
}
/**
 * \par Function
 *    setMusicPlayMode
 * \par Description
 *    set play Music Play mode
 * \param[in]
 *    uint8_t mode(00~04),
 *    00:Single non-loop playback mode(default)
 *    01:Single cycle play mode
 *    02:Repertoire loop mode
 *    03:Random mode
 *    04:Folder loop mode
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeAudioPlayer::setMusicPlayMode(uint8_t mode)
{
  uint8_t len = 0x04;
  nextCmd_time = millis();
  if (nextCmd_time - prevCmd_time < MAX_TIMEOUT)
  {
    delay( MAX_TIMEOUT - (nextCmd_time - prevCmd_time));
  }
  if (mode > 4)
  {
    mode = 0;
  }
  if (mode < 0)
  {
    mode = 0;
  }
  writebuffer[0] = MECODE_START;
  writebuffer[1] = len;
  writebuffer[2] = MESET_PLAY_MODE;
  writebuffer[3] = mode;
  writebuffer[4] = checkSum(writebuffer, len);
  writebuffer[5] = MECODE_END;
  sendCmd(writebuffer, 6);
  prevCmd_time = millis();
}

/************************************************************

      Recoding Control Function
*************************************************************/
/**
 * \par Function
 *    startRecodingFileName
 * \par Description
 *    start Recoding a mucsic File and Name is
 * \param[in]
 *    String str
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeAudioPlayer::startRecordingFileName(String str)
{
  uint8_t len = 0;
  uint8_t str_len = 0;
  uint8_t i = 0;
  char temp[20];
  char temp_ch;
  str_len = str.length();

  if (str_len > 20)
  {
    str_len = 20;   
  }

  for (int i = 0; i < str_len; i++)
  {
    temp_ch = str.charAt(i);
    temp[i] = (temp_ch >= 'a' && temp_ch <= 'z') ? (temp_ch - 32) : temp_ch;  
  }
    
  nextCmd_time = millis();
  if (nextCmd_time - prevCmd_time < MAX_TIMEOUT)
  {
    delay( MAX_TIMEOUT - (nextCmd_time - prevCmd_time));
  }
  
  len = str_len + 3;
  writebuffer[0] = MECODE_START;
  writebuffer[1] = len;
  writebuffer[2] = MEFILE_NAME_RECORDING;
  for (int k = 0; k < str_len+1; k++)
  {
    writebuffer[3 + k] = (uint8_t)temp[k];
  }
  writebuffer[len] = (uint8_t)checkSum(writebuffer, len);
  writebuffer[len+1] = MECODE_END;
  sendCmd(writebuffer, len + 2);
  prevCmd_time = millis();
}

/**
 * \par Function
 *    stopRecodingFileName
 * \par Description
 *    stop Recoding a mucsic File and Name is
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeAudioPlayer::stopRecording(void)
{
  uint8_t len = 0x03;
  nextCmd_time = millis();
  if (nextCmd_time - prevCmd_time < MAX_TIMEOUT)
  {
    delay( MAX_TIMEOUT - (nextCmd_time - prevCmd_time));
  }
  writebuffer[0] = MECODE_START;
  writebuffer[1] = len;
  writebuffer[2] = MESTOP_RECORDING;
  writebuffer[3] = checkSum(writebuffer, len);
  writebuffer[4] = MECODE_END;
  sendCmd(writebuffer, 5);
  prevCmd_time = millis();
}
/**
 * \par Function
 *    deleteFileName
 * \par Description
 *    delete a Recoding mucsic File and Name is
 * \param[in]
 *    String str
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeAudioPlayer::deleteFileName(String str)
{
  uint8_t len = 0;
  uint8_t str_len = 0;
  uint8_t i = 0;
  char temp[20];
  char temp_ch;
  str_len = str.length();

  for (int i = 0; i < str_len; i++)
  {
    temp_ch = str.charAt(i);
    temp[i] = (temp_ch >= 'a' && temp_ch <= 'z') ? (temp_ch - 32) : temp_ch;  
  }
    
  nextCmd_time = millis();
  if (nextCmd_time - prevCmd_time < MAX_TIMEOUT)
  {
    delay( MAX_TIMEOUT - (nextCmd_time - prevCmd_time));
  }
  
  
  len = str_len + 3;
  writebuffer[0] = MECODE_START;
  writebuffer[1] = len;
  writebuffer[2] = MEDELETE_SPECIFY_FILE_NAME;

  for (int k = 0; k < str_len+1; k++)
  {
    writebuffer[3 + k] = (uint8_t)temp[k];
  }
  writebuffer[len] = (uint8_t)checkSum(writebuffer, len);
  writebuffer[len+1] = MECODE_END;

  sendCmd(writebuffer, len + 2);
  prevCmd_time = millis();
}

/**
 * \par Function
 *    deleteAllMusicFile
 * \par Description
 *    delete all mucsic File
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeAudioPlayer::deleteAllMusicFile(void)
{
  uint8_t len = 0x03;
  nextCmd_time = millis();
  if (nextCmd_time - prevCmd_time < MAX_TIMEOUT)
  {
    delay( MAX_TIMEOUT - (nextCmd_time - prevCmd_time));
  }
  writebuffer[0] = MECODE_START;
  writebuffer[1] = len;
  writebuffer[2] = MEDELETE_ALL;
  writebuffer[3] = checkSum(writebuffer, len);
  writebuffer[4] = MECODE_END;
  sendCmd(writebuffer, 5);
  prevCmd_time = millis();
}

/**
 * \par Function
 *    volumeUp
 * \par Description
 *    volume Up
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeAudioPlayer::volumeUp(void)
{
  uint8_t len = 0x03;
  writebuffer[0] = MECODE_START;
  writebuffer[1] = len;
  writebuffer[2] = MESET_VOLUME_UP;
  writebuffer[3] = checkSum(writebuffer, len);
  writebuffer[4] = MECODE_END;
  sendCmd(writebuffer, 5);
}
/**
 * \par Function
 *    volumeDown
 * \par Description
 *    volume Down
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeAudioPlayer::volumeDown(void)
{
  uint8_t len = 0x03;
  writebuffer[0] = MECODE_START;
  writebuffer[1] = len;
  
writebuffer[2] = MESET_VOLUME_DOWN;
  writebuffer[3] = checkSum(writebuffer, len);
  writebuffer[4] = MECODE_END;
  sendCmd(writebuffer, 5);
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
void MeAudioPlayer::getFirmwareVersion(char *buffer)
{
  int16_t i = 0;
  int8_t return_value = 0;
  uint8_t reg = GET_VERSION_ADDR;

  return_value = readData(reg, &i2cData[reg], 8);
  
  if(return_value == I2C_OK)
  {

    memcpy(buffer, &i2cData[reg], 8);
  }
  delay(2);
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
int8_t MeAudioPlayer::readData(uint8_t start, uint8_t *buffer, uint8_t size)
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

