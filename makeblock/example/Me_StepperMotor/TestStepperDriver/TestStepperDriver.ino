#include "MeOrion.h"

int dirPin = mePort[PORT_1].s1;//the direction pin connect to Base Board PORT1 SLOT1
int stpPin = mePort[PORT_1].s2;//the Step pin connect to Base Board PORT1 SLOT2

void setup()
{
  pinMode(dirPin, OUTPUT);
  pinMode(stpPin, OUTPUT);
}

void step(boolean dir,int steps)
{
  digitalWrite(dirPin,dir);
  delay(50);
  for(int i=0;i<steps;i++)
  {
    digitalWrite(stpPin, HIGH);
    delayMicroseconds(800);
    digitalWrite(stpPin, LOW);
    delayMicroseconds(800); 
  }
}

void loop()
{
  step(1,200);//run 200 step
  delay(1000);
  step(0,200);
  delay(1000);
}
