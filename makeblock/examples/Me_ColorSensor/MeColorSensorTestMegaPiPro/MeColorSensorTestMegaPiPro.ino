#include "MeMegaPiPro.h"
#include "Wire.h"
/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MeColorsensor
 * \brief   Driver for MeColorSensor module.
 * @file    MeColorSensor.h
 * @author  MakeBlock
 * @version V1.0.1
 * @date    2017/05/23
 * @brief   Header for MeColorSensor.cpp module.
 * \par Description
 * This file is a drive for MeColorSensor module, It supports MeColorSensor V1.0 device provided
 * by MakeBlock.
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 *  zzipeng         2017/05/23          1.0.1         complete the driver code.
 * </pre>
 *  megapipro board only support PORT 6 7 8 9 10 11 12,as for this module!!
 */

MeColorSensor colorsensor1(PORT_6);
MeColorSensor colorsensor2(PORT_7);

uint16_t r1=0,g1=0,b1=0,c1=0;
uint8_t colorresult1 = 0;
uint16_t r2=0,g2=0,b2=0,c2=0;
uint8_t colorresult2 = 0;
long colorcode1=0,colorcode2=0;

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  colorsensor1.SensorInit();
  colorsensor2.SensorInit();
}

void loop() 
{
  // put your main code here, to run repeatedly:
    colorresult1 = colorsensor1.ColorIdentify();
    colorcode1 = colorsensor1.ReturnColorCode();
    
    r1 = (uint8_t)(colorcode1>>16);
    g1 = (uint8_t)(colorcode1>>8);
    b1 = (uint8_t)(colorcode1);
    
    Serial.print("R:");
    Serial.print(r1);
    Serial.print("\t");
    Serial.print("G:");
    Serial.print(g1);
    Serial.print("\t");
    Serial.print("B:");
    Serial.print(b1);
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
      break;    
    }
    Serial.print("\t");
    Serial.print("code:");
    Serial.print(colorcode1,HEX);
    Serial.print("\t");
   
    colorresult2 = colorsensor2.ColorIdentify();
    colorcode2 = colorsensor2.ReturnColorCode();
    r2 = (uint8_t)(colorcode2>>16);
    g2 = (uint8_t)(colorcode2>>8);
    b2 = (uint8_t)(colorcode2);
    
    Serial.print("R:");
    Serial.print(r2);
    Serial.print("\t");
    Serial.print("G:");
    Serial.print(g2);
    Serial.print("\t");
    Serial.print("B:");
    Serial.print(b2);
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
      case ORANGE:
      Serial.print("ORANGE");
      break;
      case GREEN:
      Serial.print("GREEN");
      break;
      case RED:
      Serial.print("RED");
      break;
      case PINKE:
      Serial.print("PINKE");
      break;
      case WHITE:
      Serial.print("WHITE");
      break;
      default:
      break;    
    }

    Serial.print("\t");
    Serial.print("code:");
    Serial.println(colorcode2,HEX);
}
