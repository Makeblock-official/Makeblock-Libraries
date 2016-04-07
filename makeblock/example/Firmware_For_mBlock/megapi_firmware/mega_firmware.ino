/*************************************************************************
* File Name          : Firmware_for_MegaPi.ino
* Author             : myan
* Updated            : myan
* Version            : V0e.01.101
* Date               : 02/20/2016
* Description        : Firmware for Makeblock Electronic modules with Scratch.  
* License            : CC-BY-SA 3.0
* Copyright (C) 2013 - 2016 Maker Works Technology Co., Ltd. All right reserved.
* http://www.makeblock.cc/
**************************************************************************/
#include <Arduino.h>
#include <MeMegaPi.h>
#include "MeEEPROM.h"
#include <Wire.h>
#include <SoftwareSerial.h>
#include "init.h"
//#define DEBUG_INFO
String mVersion = "0e.01.101";
void setup()
{
  delay(5);
  Serial.begin(115200);
  Serial2.begin(115200);
  Serial3.begin(115200);
  delay(100);
  Serial.print("Version: ");
  Serial.println(mVersion);
  encodersInit();
  steppersInit();
  delay(100);
}
void loop()
{
  parserRead();
  encodersUpdate();
  steppersUpdate();
}


