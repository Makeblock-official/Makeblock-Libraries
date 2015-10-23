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

