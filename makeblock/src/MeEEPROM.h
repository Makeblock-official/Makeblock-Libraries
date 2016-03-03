/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \brief   Location map for EEPROM.
 * @file    MeEEPROM.h
 * @author  MakeBlock
 * @version V1.0.
 * @date    2016/01/21
 * @brief   EEPROM location map.
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
 * This file is a location map for EEPROM that used by makeblock libraries.
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 *  Mark Yan         2015/09/02          1.0.0         build the new
 * </pre>
 *
 */
#ifndef ME_EEPROM_H
#define ME_EEPROM_H
#include <Arduino.h>
#include <stdbool.h>
#include "MeConfig.h"
#include "MePort.h"

#define EEPROM_START_POS         0

//0x00 - 0x34   (26*2 = 52)
#define EEPROM_IF_HAVEPID_CHECK1 0xAB
#define EEPROM_IF_HAVEPID_CHECK2 0xCD
#define EEPROM_CHECK_START 0xAB
#define EEPROM_CHECK_MID   0xCD
#define EEPROM_CHECK_END   0xEF

#define ON_BOARD_ENCODER_SLOT0_PARTITION_CHECK   EEPROM_START_POS                             //
#define ON_BOARD_ENCODER_SLOT0_START_ADDR        ON_BOARD_ENCODER_SLOT0_PARTITION_CHECK  + 2  //Partition checksum
#define ON_BOARD_ENCODER_SLOT0_PID_ADDR          ON_BOARD_ENCODER_SLOT0_START_ADDR + 1        //start data
#define ON_BOARD_ENCODER_SLOT0_RATIO_ADDR        ON_BOARD_ENCODER_SLOT0_PID_ADDR + 16         //pids data, 4 float
#define ON_BOARD_ENCODER_SLOT0_PLUS_ADDR         ON_BOARD_ENCODER_SLOT0_RATIO_ADDR + 4        //ratio0 data
#define ON_BOARD_ENCODER_SLOT0_END_ADDR          ON_BOARD_ENCODER_SLOT0_PLUS_ADDR + 2         //plus data

#define ON_BOARD_ENCODER_SLOT1_PARTITION_CHECK   ON_BOARD_ENCODER_SLOT0_END_ADDR + 1          //end data
#define ON_BOARD_ENCODER_SLOT1_START_ADDR        ON_BOARD_ENCODER_SLOT1_PARTITION_CHECK  + 2  //Partition checksum
#define ON_BOARD_ENCODER_SLOT1_PID_ADDR          ON_BOARD_ENCODER_SLOT1_START_ADDR + 1        //start data
#define ON_BOARD_ENCODER_SLOT1_RATIO_ADDR        ON_BOARD_ENCODER_SLOT1_PID_ADDR + 16         //pids data, 4 float
#define ON_BOARD_ENCODER_SLOT1_PLUS_ADDR         ON_BOARD_ENCODER_SLOT1_RATIO_ADDR + 4        //ratio0 data
#define ON_BOARD_ENCODER_SLOT0_END_ADDR          ON_BOARD_ENCODER_SLOT0_PLUS_ADDR + 2         //plus data

//0x50 - 0x70
#define BALANCED_CAR_PARTITION_CHECK    0x50
#define BALANCED_CAR_START_ADDR         BALANCED_CAR_PARTITION_CHECK + 2                      //Partition checksum
#define BALANCED_CAR_NATURAL_BALANCE    BALANCED_CAR_START_ADDR + 1                           //start data
#define BALANCED_CAR_ANGLE_PID_ADDR     BALANCED_CAR_NATURAL_BALANCE + 4                      //Natural balance angle 4
#define BALANCED_CAR_SPEED_PID_ADDR     BALANCED_CAR_ANGLE_PID_ADDR + 12                      //PID 4*3
#define BALANCED_CAR_DIR_PID_ADDR       BALANCED_CAR_SPEED_PID_ADDR + 12                      //PID 4*3
#define BALANCED_CAR_END_ADDR           BALANCED_CAR_DIR_PID_ADDR + 4                         //only P

//0x80 - 0x90
#define AURIGA_MODE_PARTITION_CHECK    0x80
#define AURIGA_MODE_START_ADDR         AURIGA_MODE_PARTITION_CHECK + 2                        //Partition checksum
#define AURIGA_MODE_CONFIGURE          AURIGA_MODE_START_ADDR + 1                             //start data
#define AURIGA_MODE_END_ADDR           AURIGA_MODE_CONFIGURE + 1                              //auriga mode

//0x90 - 0xa0
#define MEGAPI_MODE_PARTITION_CHECK    0x90
#define MEGAPI_MODE_START_ADDR         MEGAPI_MODE_PARTITION_CHECK + 2                        //Partition checksum
#define MEGAPI_MODE_CONFIGURE          MEGAPI_MODE_START_ADDR + 1                             //start data
#define MEGAPI_MODE_END_ADDR           MEGAPI_MODE_CONFIGURE + 1

#endif