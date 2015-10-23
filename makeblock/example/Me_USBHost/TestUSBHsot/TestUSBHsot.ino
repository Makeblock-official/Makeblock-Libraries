#include "MeOrion.h"
#include "SoftwareSerial.h"
//MeUSBHost usbhost(8,13);
MeUSBHost usbhost(PORT_3);

void parseJoystick(unsigned char *buf)   //Analytic function, print 8 bytes from USB Host
{
    int i = 0;
    for(i = 0; i < 7; i++)
    {
        Serial.print(buf[i]);  //It won't work if you connect to the Makeblock Orion.
        Serial.print('-');
    }
    Serial.println(buf[7]);
    delay(10);
}

void setup()
{
    Serial.begin(57600);
    usbhost.init(USB1_0);  //USB1_0 or USB2_0
}

void loop()
{
    if(!usbhost.device_online)
    {
        usbhost.probeDevice();
        delay(1000);
    }
    else
    {
        int len = usbhost.host_recv();
        parseJoystick(usbhost.RECV_BUFFER);
        delay(5);
    }
}

