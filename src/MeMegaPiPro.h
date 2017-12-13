/**
 * \par Copyright (C), 2012-2017, MakeBlock
 * \brief   Driver for MegaPiPro board.
 * @file    MeMegaPiPro.h
 * @author  MakeBlock
 * @version V1.0.3
 * @date    2017/05/22
 * @brief   Driver for MegaPiPro board.
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
 * This file is the driver for MegaPiPro board by MakeBlock.
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Zzipeng          2017/02/27          1.0.0            build the new
 * Zzipeng          2017/03/14          1.0.1            add MeSmartServo.h.
 * Zzipeng          2017/02/20          1.0.2            put the array megaPi_slots[4] to MegaPipro.h
 * Zzipeng          2017/05/20          1.0.3            add MeMegaPiProESCMotor.h
 * Zzipeng          2017/05/22          1.0.4            add MeColorSensor.h
 * </pre>
 */
#ifndef MeMegaPiPro_H
#define MeMegaPiPro_H

#include <Arduino.h>
#include "MeConfig.h"

// Supported Modules drive needs to be added here
#include "Me7SegmentDisplay.h"
#include "MeUltrasonicSensor.h"
#include "MeDCMotor.h"
#include "MeRGBLed.h"
#include "Me4Button.h"
#include "MePotentiometer.h"
#include "MeJoystick.h"
#include "MePIRMotionSensor.h"
#include "MeShutter.h"
#include "MeLineFollower.h"
#include "MeSoundSensor.h"
#include "MeLimitSwitch.h"
#include "MeLightSensor.h"
#include "MeSerial.h"
#include "MeBluetooth.h"
#include "MeWifi.h"
#include "MeTemperature.h"
#include "MeGyro.h"
#include "MeInfraredReceiver.h"
#include "MeCompass.h"
#include "MeUSBHost.h"
#include "MeTouchSensor.h"
#include "MeStepper.h"
#include "MeStepperOnBoard.h"
#include "MeEncoderMotor.h"
#include "MeEncoderNew.h"
#include "MeBuzzer.h"
#include "MeLEDMatrix.h"
#include "MeHumitureSensor.h"
#include "MeFlameSensor.h"
#include "MeGasSensor.h"
#include "MeEncoderOnBoard.h"
#include "MeMegaPiDCMotor.h"
#include "MePressureSensor.h"
#include "MePS2.h"
#include "MeSmartServo.h"
#include "MeDCMotor.h"
#include "MeMegaPiProESCMotor.h"
#include "MeColorSensor.h"

/*********************  MegaPiPro Board GPIO Map *********************************/
// struct defined in MegaPiPro.h
#define PORT1A  PORT_1
#define PORT1B  PORT_9
#define PORT2A  PORT_2
#define PORT2B  PORT_10
#define PORT3A  PORT_3
#define PORT3B  PORT_11
#define PORT4A  PORT_4
#define PORT4B  PORT_12
/*4 Dc Motor gpio Map*/
#define M9   (0X01)
#define M10  (0X02)
#define M11  (0X03)
#define M12  (0X04)

 MePort_Sig mePort[17] =
 {
   { NC, NC },{ 46, 23 }, { 45,  22 }, { 44, 25 },  { 10, 24 },
   { 16, 17 },{ A9, A4 }, { A10, A11}, { A15, A14}, { A13, A12 }, 
   { A7, A8 },{ A6, A5 }, { A1,  A2 }, { NC, NC }, { NC, NC }, 
   { NC, NC }, { NC, NC },
 };//PORT 5~ PORT 12 is for the RJ25 Shield
  //PORT1~PORT4 is for the 4dcmotor shield

megapi_dc_type megapi_dc_Port[14] =
{
  { NC, NC }, {33,32,11}, {40,41, 7}, {47,48, 6}, {28,27, 4},
  { NC, NC }, { NC, NC }, { NC, NC }, { NC, NC }, {35,34,12},
  {36,37, 8}, {42,43, 9}, {39,29, 5},
};

Encoder_port_type encoder_Port[6] =
{
  { NC,     NC,     NC,     NC,     NC},
  //NET1    NET2    PWM     DIR1    DIR2
  { 18,     31,     12,     34,     35},
  //ENB A   ENB B   PWMB    DIR B1  DIR B2
  { 19,     38,     8,      37,     36},

  { 3,      49,     9,      43,     42},

  { 2,      26,     5,      29,     39},// for MegaPi Pro
  { NC,     NC,     NC,     NC,     NC},
};

megaPi_slot_type megaPi_slots[4] =
{
  {35,   34,   33,   32,   31,  18,  12,  11},
  {36,   37,   40,   41,   38,  19,   8,   7},
  {42,   43,   47,   48,   49,   3,   9,   6},
  /* {A5,   A4,   A3,   A2,   A1,   2,   5,   4}*/// for megapi
  {39,   29,   28,   27,   26,   2,   5,   4}// for megapipro
};

megapipro_esc_type megapi_esc_Port[5] =
{
  { NC}, {12}, {8}, {9}, {5},
};
//for step motor on board
#endif // MeMegaPiPro_H
