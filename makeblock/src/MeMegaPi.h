/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \brief   Driver for MegaPi board.
 * @file    MeMegaPi.h
 * @author  MakeBlock
 * @version V1.0.4
 * @date    2016/09/23
 * @brief   Driver for MegaPi board.
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
 * This file is the driver for MegaPi board by MakeBlock.
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2016/02/20     1.0.0            Build the New.
 * Mark Yan         2016/03/10     1.0.1            Port mapping changes.
 * Scott wang       2016/09/18     1.0.2            Add the PORT[15].
 * Scott            2016/09/20     1.0.3            Add the PORT[16].
 * Scott            2016/09/23     1.0.4            Add the MePS2.h .
 * </pre>
 */
#ifndef MeMegaPi_H
#define MeMegaPi_H

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

/*********************  MegaPi Board GPIO Map *********************************/
// struct defined in MeMegaPi.h
#define PORT1A  PORT_1
#define PORT1B  PORT_9
#define PORT2A  PORT_2
#define PORT2B  PORT_10
#define PORT3A  PORT_3
#define PORT3B  PORT_11
#define PORT4A  PORT_4
#define PORT4B  PORT_12

 MePort_Sig mePort[17] =
 {
   { NC, NC }, {  NC,  NC }, {  NC,  NC }, {  NC,  NC }, {  NC,  NC }, 
   { 16, 17 }, {  A8,  A9 }, { A10, A11 }, { A13, A12 }, {  NC,  NC }, 
   { NC, NC }, {  NC,  NC }, {  NC,  NC }, {  NC,  NC }, {  NC,  NC },
   { NC, NC },{ NC, NC },
 };

Encoder_port_type encoder_Port[6] =
{
  { NC,     NC,     NC,     NC,     NC},
  //NET2    NET1    PWM     DIR1    DIR2
  { 18,     31,     12,     34,     35},
  //ENB A   ENB B   PWMB    DIR B1  DIR B2
  { 19,     38,     8,      37,     36},
  { 3,      49,     9,      43,     42},
  { 2,      A1,     5,      A4,     A5},
  { NC,     NC,     NC,     NC,     NC},
};

megapi_dc_type megapi_dc_Port[14] =
{
  { NC, NC }, {33,32,11}, {40,41, 7}, {47,48, 6}, {A3,A2, 4},
  { NC, NC }, { NC, NC }, { NC, NC }, { NC, NC }, {35,34,12},
  {36,37, 8}, {42,43, 9}, {A5,A4, 5},
};

#endif // MeMegaPi_H
