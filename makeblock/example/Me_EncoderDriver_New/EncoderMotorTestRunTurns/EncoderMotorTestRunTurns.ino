#include "MeOrion.h"
#include <Wire.h>
#include <SoftwareSerial.h>

MeEncoderNew motor2(0x09, SLOT2);   //  motor at slot2

void setup()
{
  motor2.begin();
  Serial.begin(9600);
}

void loop()
{
  motor2.runTurns(3,100);
  delay(9000);
}
