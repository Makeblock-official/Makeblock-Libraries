/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \brief   Driver for MeAuriga board.
 * @file    MeAuriga.h
 * @author  MakeBlock
 * @version V1.0.4
 * @date    2016/09/23
 * @brief   Driver for MeAuriga board.
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
 * This file is the driver for MeAuriga board by MakeBlock.
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2016/01/27          1.0.0            Build the New.
 * Mark Yan         2016/02/20          1.0.1            Change the port enumeration
 * Scott wang       2016/09/18          1.0.2            Add the PORT[15].
 * Scott            2016/09/20          1.0.3            Add the PORT[16].
 * Scott            2016/09/23          1.0.4            Add the MePS2.h .
 * Zzipeng          2016/12/15          1.0.5            Add the MePm25Sensor.h .
 * </pre>
 */
#ifndef MeAuriga_H
#define MeAuriga_H

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
#include "MeEncoderMotor.h"
#include "MeEncoderNew.h"
#include "MeBuzzer.h"
#include "MeLEDMatrix.h"
#include "MeHumitureSensor.h"
#include "MeFlameSensor.h"
#include "MeGasSensor.h"
#include "MeEncoderOnBoard.h"
#include "MeOnBoardTemp.h"
#include "MeSmartServo.h"
#include "MePS2.h"
#include "MePm25Sensor.h"
#include "MeColorSensor.h"
/*********************  Auriga Board GPIO Map *********************************/
// struct defined in MeAuriga.h
 MePort_Sig mePort[17] =
 {
   { NC, NC }, {   5,   4 }, {   3,   2 }, {   7,   6 }, {   9,   8 }, 
   { 16, 17 }, { A10, A15 }, {  A9, A14 }, {  A8, A13 }, {  A7, A12 }, 
   //             LIGHT2        LIGHT1        TEMP          SOUND
   { A6,A11 }, {  NC,  A2 }, {  NC,  A3 }, {  NC,  A0 }, {  NC,  A1 },
   { NC, NC }, { NC, NC },
 };
 
Encoder_port_type encoder_Port[6] =
{
  { NC,     NC,     NC,     NC,     NC},
  //ENA A   ENA B   PWMA    DIR A2  DIR A1
  { 19,     42,     11,     49,     48},
  //ENB A   ENB B   PWMB    DIR B1  DIR B2
  { 18,     43,     10,     47,     46},
  { NC,     NC,     NC,     NC,     NC},
  { NC,     NC,     NC,     NC,     NC},
};

#define buzzerOn()  pinMode(45,OUTPUT),digitalWrite(45, HIGH)
#define buzzerOff() pinMode(45,OUTPUT),digitalWrite(45, LOW)

#endif // MeAuriga_H
