/**
 * \par Copyright (C), 2012-2015, MakeBlock
 * \brief   Driver for MeZero board.
 * @file    MeZero_H.h
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/01
 * @brief   Driver for MeZero board.
 *
 * \par Copyright
 * This software is Copyright (C), 2012-2015, MakeBlock. Use is subject to license \n
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
 * This file is the driver for MeOrion hoard by MakeBlock.
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/09/01     1.0.0            Rebuild the old lib.
 * Rafael Lee       2015/09/02     1.0.0            Added some comments and macros.
 * </pre>
 */
#ifndef MeZero_H
#define MeZero_H

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

/*********************  Orion Board GPIO Map *********************************/
// struct defined in MePort.h
 MePort_Sig mePort[15] =
 {
   { NC, NC }, {   5,   4 }, {   3,   2 }, {   7,   6 }, {   9,   8 }, 
   { 16, 17 }, {  A8,  A9 }, {  A6,  A7 }, {  A4,  A5 }, {  A2,  A3 }, 
   { A0, A1 }, {  NC, A13 }, {  NC, A14 }, {  NC, A11 }, {  NC, A12 },
 };

#define buzzerOn()  pinMode(10,OUTPUT),digitalWrite(10, HIGH)
#define buzzerOff() pinMode(10,OUTPUT),digitalWrite(10, LOW)

#endif // MeOrion_H
