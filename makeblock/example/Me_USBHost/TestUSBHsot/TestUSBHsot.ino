/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    MeUSBHost.ino
 * @author  MakeBlock
 * @version V1.0.2
 * @date    2016/01/20
 * @brief   Description: this file is sample code for Me USB Host module.
 *          The USB Host inherited the MeSerial class from SoftwareSerial.
 *          Software Serial will disable/enable interrupt when read data,
 *          so it will affect servos pwm output if the device frequent input 
 *          data. if you need use servos, It is best to use the hardware serial
 *          on the board.(UNO is PORT_5). In this case, Program file(*.ino)
 *          Can not use the debug information. if you need debug information
 *          Software Serial and a USB-Serial device is needed.
 *
 * \par Method List:
 *
 *    1. void MeUSBHost::init(int8_t type);
 *    2. int16_t MeUSBHost::probeDevice();
 *    3. uint8_t MeUSBHost::host_recv();
 *
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Mark Yan     2015/09/09    1.0.0          rebuild the old lib.
 * forfish      2015/11/19    1.0.1          add some descriptions.
 * Mark Yan     2016/01/20    1.0.2          Add some important descriptive.
 * </pre>
 */

#include "MeOrion.h"
#include "SoftwareSerial.h"
//MeUSBHost usbhost(13,12);
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

