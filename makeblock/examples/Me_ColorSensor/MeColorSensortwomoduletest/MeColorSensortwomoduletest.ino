//#include "MeOrion.h"
//#include "MeUnoShield.h"
#include "MeMCore.h"
#include "Wire.h"
/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MeGyro
 * \brief   Driver for MeColorSensor module.
 * @file    MeColorSensor.h
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2017/01/17
 * @brief   Header for MeColorSensor.cpp module.
 * \par Description
 * This file is a drive for MeColorSensor module, It supports MeColorSensor V1.0 device provided
 * by MakeBlock.
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 *  zzipeng         2017/04/12          1.0.0         complete the driver code.
 * </pre>
 *                 Attention please!
 *                 8mm~24mm detection distance.
 *              Unoshield board only support PORT 1 2 3 4,as for this module!!
 */
//本例程示例两个颜色传感器模块分时轮流工作，获取颜色识别值。数据返回率每个每秒3~4次。

MeColorSensor colorsensor1(PORT_3);
MeColorSensor colorsensor2(PORT_4);

uint8_t colorresult1,colorresult2;
uint16_t redvalue1=0,greenvalue1=0,bluevalue1=0,colorvalue1=0;
uint16_t redvalue2=0,greenvalue2=0,bluevalue2=0,colorvalue2=0;
long systime = 0,colorcode1=0,colorcode2=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  colorsensor1.SensorInit();//
  colorsensor2.SensorInit();//
  systime = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(millis()-systime>200)
  {
    systime = millis();
    colorresult1 = colorsensor1.ColorIdentify();
    redvalue1   = colorsensor1.ReturnRedData();
    greenvalue1 = colorsensor1.ReturnGreenData();
    bluevalue1  = colorsensor1.ReturnBlueData();
    colorvalue1 = colorsensor1.ReturnColorData();
    colorcode1 = colorsensor1.ReturnColorCode();//RGB24code

    colorresult2 = colorsensor2.ColorIdentify();
    redvalue2   = colorsensor2.ReturnRedData();
    greenvalue2 = colorsensor2.ReturnGreenData();
    bluevalue2  = colorsensor2.ReturnBlueData();
    colorvalue2 = colorsensor2.ReturnColorData();
    colorcode2 = colorsensor2.ReturnColorCode();//RGB24code
    
    Serial.print("R1:");
    Serial.print(redvalue1);
    Serial.print("\t");
    Serial.print("G:");
    Serial.print(greenvalue1);
    Serial.print("\t");
    Serial.print("B:");
    Serial.print(bluevalue1);
    Serial.print("\t");
    Serial.print("C:");
    Serial.print(colorvalue1);
    Serial.print("\t");
    Serial.print("color:");
    switch(colorresult1)
    {
       case BLACK:
      Serial.print("BLACK");
      break;
      case BLUE:
      Serial.print("BLUE");
      break;
      case YELLOW:
      Serial.print("YELLOW");
      break;
      case GREEN:
      Serial.print("GREEN");
      break;
      case RED:
      Serial.print("RED");
      break;
      case WHITE:
      Serial.print("WHITE");
      break;
      default:
      Serial.print("i don't know");
      break;    
     }
    Serial.print("\t");
    Serial.print("code:");
    Serial.print(colorcode1,HEX); 

    Serial.print("\t\t");
    Serial.print("R2:");
    Serial.print(redvalue2);
    Serial.print("\t");
    Serial.print("G:");
    Serial.print(greenvalue2);
    Serial.print("\t");
    Serial.print("B:");
    Serial.print(bluevalue2);
    Serial.print("\t");
    Serial.print("C:");
    Serial.print(colorvalue2);
    Serial.print("\t");
    Serial.print("color:");
    switch(colorresult2)
    {
      case BLACK:
      Serial.print("BLACK");
      break;
      case BLUE:
      Serial.print("BLUE");
      break;
      case YELLOW:
      Serial.print("YELLOW");
      break;
      case GREEN:
      Serial.print("GREEN");
      break;
      case RED:
      Serial.print("RED");
      break;
      case WHITE:
      Serial.print("WHITE");
      break;
      default:
      Serial.print("i don't know");
      break;    
     }
    Serial.print("\t");
    Serial.print("code:");
    Serial.println(colorcode2,HEX); 
  }
}