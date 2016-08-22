/*************************************************************************
* File Name          : IR_Ultrasonic.ino
* Author             : Ander, Mark Yan
* Updated            : Ander, Mark Yan
* Version            : V01.01.003
* Date               : 01/19/2016
* Description        : Firmware for Makeblock Electronic modules with Scratch.  
* License            : CC-BY-SA 3.0
* Copyright (C) 2013 - 2016 Maker Works Technology Co., Ltd. All right reserved.
* http://www.makeblock.cc/
**************************************************************************/
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Arduino.h>
#include <MeOrion.h>
#include<math.h>

MeDCMotor dc;
MeUltrasonicSensor us(PORT_3);
MeInfraredReceiver infraredReceiverDecode(PORT_6);
MeBuzzer buzzer;

int analogs[8]={A0,A1,A2,A3,A4,A5,A6,A7};
String mVersion = "01.01.003";
unsigned char irRead = 0;

//Just for Start
int moveSpeed = 190;
int turnSpeed = 200;
int minSpeed = 45;
int factor = 23;
int distance=0;
int randnum = 0;
boolean leftflag;
boolean rightflag;
boolean maintainMode;
int starter_mode = 0;

void Forward()
{
  dc.reset(M1);
  dc.run(moveSpeed);
  dc.reset(M2);
  dc.run(moveSpeed);
}

void Backward()
{
  dc.reset(M1);
  dc.run(-moveSpeed);
  dc.reset(M2);
  dc.run(-moveSpeed);
}

int backTurnTime(int speed)
{
  return (int)(5.9074e-3*speed*speed - 4.6078*speed + 1286);
}

void BackwardAndTurnLeft()
{
  dc.reset(M1);
  dc.run(-moveSpeed/2);
  dc.reset(M2);
  dc.run(-moveSpeed);
  delay( backTurnTime(moveSpeed) );
}

void BackwardAndTurnRight()
{
  dc.reset(M1);
  dc.run(-moveSpeed);
  dc.reset(M2);
  dc.run(-moveSpeed/2);
  delay( backTurnTime(moveSpeed) );
}

void TurnLeft()
{
  dc.reset(M1);
  dc.run(-moveSpeed);
  dc.reset(M2);
  dc.run(moveSpeed);
}

void TurnRight()
{
  dc.reset(M1);
  dc.run(moveSpeed);
  dc.reset(M2);
  dc.run(-moveSpeed);
}

void Stop()
{
  dc.reset(M1);
  dc.run(0);
  dc.reset(M2);
  dc.run(0);
}
void ChangeSpeed(int spd)
{
  moveSpeed = spd;
}

void ultrCarProcess()
{
  distance = us.distanceCm();
  Serial.println(distance);
  randomSeed(analogRead(A4));
  if((distance > 10) && (distance < 40))
  {
    randnum=random(300);
    if(leftflag || (randnum > 150) && (!rightflag))
    {
      leftflag=true;
      TurnLeft();   
    }
    else
    {
      rightflag=true;
      TurnRight();  
    }
  }
  else if(distance < 10)
  {
    randnum=random(300);
    if(randnum > 190)
    {
      BackwardAndTurnLeft();
    }
    else
    {
      BackwardAndTurnRight();
    }
  }
  else
  {
    leftflag=false;
    rightflag=false;
    Forward();
  }
}

void IrProcess()
{
  infraredReceiverDecode.loop();
  irRead = infraredReceiverDecode.getCode();
  if((irRead != IR_BUTTON_TEST) && (starter_mode != 0))
  {
    return;
  }
      switch(irRead)
  {
    case IR_BUTTON_PLUS: 
      Forward();
      break;
    case IR_BUTTON_MINUS:
      Backward();
      maintainMode = false;
      break;
    case IR_BUTTON_NEXT:
      TurnRight();
      break;
    case IR_BUTTON_PREVIOUS:
      TurnLeft();
      break;
    case IR_BUTTON_9:
      ChangeSpeed(factor*9+minSpeed);
      break;
    case IR_BUTTON_8:
      ChangeSpeed(factor*8+minSpeed);
      break;
    case IR_BUTTON_7:
      ChangeSpeed(factor*7+minSpeed);
      break;
    case IR_BUTTON_6:
      ChangeSpeed(factor*6+minSpeed);
      break;
    case IR_BUTTON_5:
      ChangeSpeed(factor*5+minSpeed);
      break;
    case IR_BUTTON_4:
      ChangeSpeed(factor*4+minSpeed);
      break;
    case IR_BUTTON_3:
      ChangeSpeed(factor*3+minSpeed);
      break;
    case IR_BUTTON_2:
      ChangeSpeed(factor*2+minSpeed);
      break;
    case IR_BUTTON_1:
      ChangeSpeed(factor*1+minSpeed);
      break;
    case IR_BUTTON_E:
      maintainMode = !maintainMode;
      
      //eat long press input
      while(infraredReceiverDecode.buttonState() != 0)
      {
        infraredReceiverDecode.loop();
      }
      starter_mode = 0;
      break;  
    case IR_BUTTON_TEST:
      Stop();
      
      //eat long press input
      while(infraredReceiverDecode.buttonState() != 0)
      {
        infraredReceiverDecode.loop();
      }
      starter_mode = starter_mode + 1;
      if(starter_mode == 2)
      { 
        starter_mode = 0;
      }
      break;
    default:
      if(!maintainMode)
        {
          Stop();
        }
        else
        {
          Forward();
        }
      break;
  }
}
void setup(){
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  delay(300);
  digitalWrite(13,LOW);
  Serial.begin(115200);
  delay(500);
  buzzerOn();
  delay(100);
  buzzerOff();
  delay(500);
  infraredReceiverDecode.begin();
  leftflag=false;
  rightflag=false;
  maintainMode=false;
  randomSeed(analogRead(0));
  Stop();
  Serial.print("Version: ");
  Serial.println(mVersion);
}
void loop(){
  IrProcess();
  if(starter_mode == 1)
  {
    maintainMode = false;
    ultrCarProcess();    
  }
}


