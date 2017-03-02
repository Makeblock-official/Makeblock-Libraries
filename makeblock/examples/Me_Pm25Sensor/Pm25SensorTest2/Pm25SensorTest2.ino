/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    MeCompass.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2016/12/23
 * @brief   Description: this file is sample code for Me MePm25Sensor device.
 *
 * Function List:
 *
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Zzipeng     2016/12/13      1.0.0          rebuild the old lib
 * </pre>
 *
 */
#include "MeAuriga.h"
//#include "MePm25Sensor.h"
#include <SoftwareSerial.h>
MePm25Sensor myMePm25Sensor(PORT3);
uint16_t pm1_0=0,pm2_5=0,pm10=0,len=0,checksum=0;
int cmdTimeOutValue = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myMePm25Sensor.begin(9600);
  delay(50);
  Serial.println("setup...!");
 cmdTimeOutValue = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  myMePm25Sensor.rxloop();//loop
  pm1_0 = myMePm25Sensor.readPm1_0Concentration();
  pm2_5 = myMePm25Sensor.readPm2_5Concentration();
  pm10 = myMePm25Sensor.readPm10Concentration();
  len = myMePm25Sensor.returnlen();
  checksum = myMePm25Sensor.returnchecksum();

 if(millis() - cmdTimeOutValue > 500)
 {
    cmdTimeOutValue = millis();
    Serial.print("len=");
    Serial.print(len);
    Serial.print("\t");
    Serial.print("Pm1.0=");
    Serial.print(pm1_0);
    Serial.print("ug/m^3\t");
    Serial.print("Pm2.5=");
    Serial.print(pm2_5);
    Serial.print("ug/m^3\t");
    Serial.print("Pm10=");
    Serial.print(pm10);
    Serial.print("ug/m^3");
    Serial.print("\t");
    Serial.print("check=");
    Serial.print(checksum);
    Serial.println();  
 }

}
