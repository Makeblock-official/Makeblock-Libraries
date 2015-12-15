/*************************************************************************
* File Name          : orion_firmware.ino
* Author             : Ander
* Updated            : Ander
* Version            : V1.10101
* Date               : 03/06/2014
* Description        : Firmware for Makeblock Electronic modules with Scratch.  
* License            : CC-BY-SA 3.0
* Copyright (C) 2013 - 2014 Maker Works Technology Co., Ltd. All right reserved.
* http://www.makeblock.cc/
**************************************************************************/
#include <Servo.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Arduino.h>
#include <MeOrion.h>

Servo servos[8];  
MeDCMotor dc;
MeTemperature ts;
MeRGBLed led;
MeUltrasonicSensor us(PORT_3);
Me7SegmentDisplay seg;
MePort generalDevice;
MeInfraredReceiver infraredReceiverDecode(PORT_6);
MeJoystick joystick;
MeStepper steppers[2];
MeBuzzer buzzer;
MeHumiture humiture;
MeFlameSensor FlameSensor;
MeGasSensor GasSensor;

typedef struct MeModule
{
    int device;
    int port;
    int slot;
    int pin;
    int index;
    float values[3];
} MeModule;

union{
    byte byteVal[4];
    float floatVal;
    long longVal;
}val;

union{
  byte byteVal[8];
  double doubleVal;
}valDouble;

union{
  byte byteVal[2];
  short shortVal;
}valShort;
MeModule modules[12];
#if defined(__AVR_ATmega32U4__) 
  int analogs[12]={A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11};
#endif
#if defined(__AVR_ATmega328P__) or defined(__AVR_ATmega168__)
  int analogs[8]={A0,A1,A2,A3,A4,A5,A6,A7};
#endif
#if defined(__AVR_ATmega1280__)|| defined(__AVR_ATmega2560__)
  int analogs[16]={A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15};
#endif
String mVersion = "10.01.001";
boolean isAvailable = false;
boolean isBluetooth = false;

int len = 52;
char buffer[52];
char bufferBt[52];
byte index = 0;
byte dataLen;
byte modulesLen=0;
boolean isStart = false;
unsigned char irRead = 0;
char serialRead;
#define VERSION 0
#define ULTRASONIC_SENSOR 1
#define TEMPERATURE_SENSOR 2
#define LIGHT_SENSOR 3
#define POTENTIONMETER 4
#define JOYSTICK 5
#define GYRO 6
#define SOUND_SENSOR 7
#define RGBLED 8
#define SEVSEG 9
#define MOTOR 10
#define SERVO 11
#define ENCODER 12
#define IR 13
#define PIRMOTION 15
#define INFRARED 16
#define LINEFOLLOWER 17
#define SHUTTER 20
#define LIMITSWITCH 21
#define BUTTON 22
#define HUMITURE 23
#define FLAMESENSOR 24
#define GASSENSOR 25
#define COMPASS 26
#define DIGITAL 30
#define ANALOG 31
#define PWM 32
#define SERVO_PIN 33
#define TONE 34
#define PULSEIN 35
#define ULTRASONIC_ARDUINO 36
#define STEPPER 40
#define LEDMATRIX 41
#define TIMER 50
#define COMMONCMD 60
  //Secondary command
  #define SET_STARTER_MODE 0x10

#define GET 1
#define RUN 2
#define RESET 4
#define START 5
float angleServo = 90.0;
int servo_pins[8]={0,0,0,0,0,0,0,0};
unsigned char prevc=0;
double lastTime = 0.0;
double currentTime = 0.0;

//Just for Start
int moveSpeed = 190;
int turnSpeed = 200;
int minSpeed = 45;
int factor = 23;
int distance=0;
int randnum = 0;
boolean leftflag;
boolean rightflag;
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

void BackwardAndTurnLeft()
{
  dc.reset(M1);
  dc.run(-moveSpeed/2);
  dc.reset(M2);
  dc.run(-moveSpeed);
}

void BackwardAndTurnRight()
{
  dc.reset(M1);
  dc.run(-moveSpeed);
  dc.reset(M2);
  dc.run(-moveSpeed/2);
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
    if((randnum > 190) && (!rightflag))
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
    case IR_BUTTON_TEST:
      Stop();
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
      Stop();
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
  randomSeed(analogRead(0));
}
void loop(){
  IrProcess();
  if(starter_mode == 1)
  {
    ultrCarProcess();    
  }
}


