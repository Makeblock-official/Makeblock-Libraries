/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    MeSerialReceiveTest.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/09/09
 * @brief   Description: this file is sample code for hardware/software Serial.
 *
 * Function List:
 *    1. void MeSerial::begin(long baudrate)
 *    2. size_t MeSerial::write(uint8_t byte)
 *    3. int16_t MeSerial::read(void)
 *    4. int16_t MeSerial::available(void)
 *    5. int16_t MeSerial::poll(void)
 *    6. void MeSerial::print(char *fmt,...)
 *    7. void MeSerial::println(char *fmt,...)
 *
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Mark Yan     2015/09/09    1.0.0          rebuild the old lib
 * forfish      2015/11/19    1.0.0          add some descriptions
 * </pre>
 */

#include "MeOrion.h"
#include <SoftwareSerial.h>

unsigned char table[128] = {0};
SoftwareSerial softuart(13,12);

void setup() 
{
   Serial.begin(57600);
   softuart.begin(57600); 
}

void loop()
{
    int readdata = 0,i = 0,count = 0;
    if (softuart.available())
    {
        Serial.print("   readdata:");
        while((readdata = softuart.read()) != (int)-1)
        {
            table[count] = readdata;
            count++;
            delay(1);
        }
        for(i = 0;i<count;i++)
        {
            Serial.print(" 0x");
            Serial.print( table[i],HEX);
            softuart.write(table[i]);
        }
        Serial.println("   stop rev");
    }
}

