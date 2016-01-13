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
#endif