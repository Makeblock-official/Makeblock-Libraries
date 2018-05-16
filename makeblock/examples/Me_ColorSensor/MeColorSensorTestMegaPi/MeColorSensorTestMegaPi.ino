#include "MeMegaPi.h"
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
 *              megapi board only support PORT 5 6 7 8,as for this module!!
 */
//本例程示例单个颜色传感器模块工作，获取颜色识别值。单个模块工作数据返回率每秒6次。

#define NUM 1
MeColorSensor colorsensor0(PORT_6);
uint8_t colorresult[NUM]={0};
long systime = 0,colorcode=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  colorsensor0.SensorInit();
}

void loop() {
  // put your main code here, to run repeatedly:
    colorresult[0] = colorsensor0.ColorIdentify();
    
    for(uint8_t i = 0;i<NUM;i++)
    {
      switch(colorresult[i])
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
    }
   Serial.print("\r\n");
}
