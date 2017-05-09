/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \brief   Driver for mCore Board.
 * \file    MeMCore.h
 * @author  MakeBlock
 * @version V1.0.4
 * @date    2016/09/23
 * @brief   Driver for mCore Board.
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
 * This file is Hardware adaptation layer between Mbot board and all
 * MakeBlock drives
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/09/01         1.0.0            Rebuild the old lib.
 * Mark Yan         2015/11/09         1.0.1            fix some comments error.
 * Scott wang       2016/09/18         1.0.2            Add the PORT[15].
 * Scott            2016/09/20         1.0.3            Add the PORT[16].
 * Scott            2016/09/23         1.0.4            Add the MePS2.h .
 * </pre>
 */
#ifndef MeMCore_H
#define MeMCore_H

#include <Arduino.h>
#include "MeConfig.h"

/* Supported Modules drive needs to be added here */
#include "Me7SegmentDisplay.h"
#include "MeUltrasonicSensor.h"
#include "MeMbotDCMotor.h"
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
#include "MeIR.h"
#include "MeLEDMatrix.h"
#include "MeBuzzer.h"
#include "MeHumitureSensor.h"
#include "MeFlameSensor.h"
#include "MeGasSensor.h"
#include "MePS2.h"

/*********************  Mbot Board GPIO Map *********************************/
MePort_Sig mePort[17] =
{
  { NC, NC }, { 11, 12 }, {  9, 10 }, { A2, A3 }, { A0, A1 },
  { NC, NC }, {  8, A6 }, { A7, 13 }, {  8, A6 }, {  6,  7 },
  {  5,  4 }, { NC, NC }, { NC, NC }, { NC, NC }, { NC, NC },
  { NC, NC },{ NC, NC },
};
#endif // MeMCore_H

