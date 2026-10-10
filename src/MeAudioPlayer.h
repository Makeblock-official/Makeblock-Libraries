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
 * 
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

*/
/*
 * 
 * \par History:
 * <pre>
 * "<Author>"         "<Time>"        "<Version>"        "<Descr>"
 *  Lanweiting      2017/11/15          1.0.0         Build the files.
 * </pre>
 *
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MeAudioPlayer_H_
#define _MeAudioPlayer_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>
#include <Wire.h>

#include "MeConfig.h"

#ifdef ME_PORT_DEFINED
  #include "MePort.h"
#endif // ME_PORT_DEFINED


/* Exported macro ------------------------------------------------------------*/
#define I2C_OK                          (0)
#define I2C_ERROR                       (-1)
#define AUDIOPLAYER_DEFAULT_ADDRESS     (0x2B)//

/* Communication Control CMD------------------------------------------------------------*/
#define MEPLAY_FILE_INDEX               0xA2
#define MEPLAY_FILE_NAME                0xA3
#define MEPAUSE_MUSIC                   0xAA
#define MESTOP_MUSIC                    0xAB
#define MEPLAY_NEXT                     0xAC
#define MEPLAY_PREV                     0xAD
#define MEVOLUME_CTL                    0xAE
#define MESET_PLAY_MODE                 0xAF
#define MEMEMORY_SELECT                 0xD2

#define MEFILE_NAME_RECORDING           0xD6
#define MESTOP_RECORDING                0xD9
#define MEDELETE_SPECIFY_FILE_NAME      0xDB
#define MEDELETE_ALL                    0xDE

#define MECODE_START                    0x7E
#define MECODE_END                      0xEF

/* Music Mode Control CMD------------------------------------------------------------*/
#define SINGLE_NON_CYCLE                0x00
#define SINGLE_CYCLE                    0x01
#define ALL_CYCLE                       0x02
#define RANDOM_PLAY                     0x03
#define FOLDER_CYCLE                    0x04
/* Memory Mode Control CMD------------------------------------------------------------*/
#define DEFAULT_MEMORY                  0x00
#define EXTERNAL_MEMORY                 0x01

#define MESET_VOLUME_UP                 0xEA
#define MESET_VOLUME_DOWN               0xEB
#define MEGET_VERSION                   0xEC
//IIC Register Address
#define GET_VERSION_ADDR                0x01

#define MAX_TIMEOUT                     100L

/*
 * Class: MeAudioPlayer
 * \par Description
 * Declaration of Class MeAudioPlayer
 */

#ifndef ME_PORT_DEFINED
  class MeAudioPlayer
#else // !ME_PORT_DEFINED
  class MeAudioPlayer : public MePort
#endif // !ME_PORT_DEFINED
{
public:
#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the MeAudioPlayer to arduino port,
 * no pins are used or initialized here
 */
  MeAudioPlayer(void);

/**
 * Alternate Constructor which can call your own function to map the MeAudioPlayer to arduino port.
 * \param[in]
 *   port - RJ25 port number
 */
  MeAudioPlayer(uint8_t port);
  /**
 * Alternate Constructor which can call your own function to map the MeAudioPlayer to arduino port
 * and change the i2c device address
 * \param[in]
 *   port - RJ25 port number
 * \param[in]
 *   address - the i2c address you want to set
 */ 
  MeAudioPlayer(uint8_t port, uint8_t address);
#else // ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the _AD0 and _INT to arduino port,
 * \param[in]
 *   _AD0 - arduino gpio number
 * \param[in]
 *   _INT - arduino gpio number
 */
  MeAudioPlayer(uint8_t _AD0, uint8_t _INT);

#endif  /* ME_PORT_DEFINED */
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
void PlayerInit(void);

/**
 * \par Function
 *    sendData
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
void sendCmd(uint8_t *sendcmd, uint8_t len);
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
uint8_t checkSum(uint8_t* buffer, uint8_t len);
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
void playMusicFileIndex(uint16_t music_index);

/**
 * \par Function
 *    playMusicFileName
 * \par Description
 *    play a mucsic File with it name
 * \param[in]
 *    char str[]
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void playMusicFileName(String str);
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
void pauseMusic(void);
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
void stopMusic(void);
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
void playNextMusic(void);
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
void playPrevMusic(void);
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
void setMusicVolume(uint8_t vol);
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
void setMusicPlayMode(uint8_t mode);

/************************************************************

      Recoding Control Function

*************************************************************/
/**
 * \par Function
 *    startRecodingFileName
 * \par Description
 *    start Recoding a mucsic File and Name is
 * \param[in]
 *    char str[]
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void startRecordingFileName(String str);
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
void stopRecording(void);
/**
 * \par Function
 *    deleteFileName
 * \par Description
 *    delete a Recoding mucsic File and Name is
 * \param[in]
 *    char str[]
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void deleteFileName(String str);
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
void deleteAllMusicFile(void);
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
void volumeUp(void);
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
void volumeDown(void);
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
uint8_t Device_Address;

private:
  volatile uint8_t  _S1;
  volatile uint8_t  _S2;
  volatile uint8_t  _AD0;
  volatile uint8_t  _INT;
  uint8_t i2cData[10];
  // uint8_t readbuffer[27];
  uint8_t writebuffer[27];
  uint32_t nextCmd_time;
  uint32_t prevCmd_time;
};
#endif
