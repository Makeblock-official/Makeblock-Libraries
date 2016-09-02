/*************************************************************************
* File Name          : mblock_Firmware.ino
* Author             : myan
* Updated            : myan
* Version            : V3.2
* Date               : 12/05/2015
* Description        : Firmware for Makeblock Electronic modules with Scratch.  
* License            : CC-BY-SA 3.0
* Copyright (C) 2013 - 2016 Maker Works Technology Co., Ltd. All right reserved.
* http://www.makeblock.cc/
**************************************************************************/

/*************************************************************************
 * Product and definitions
**************************************************************************/
//1-baseboard,  2-Orion, 3-mCore, 4-shield
#define PRO_SELECT   0x02

#if (PRO_SELECT == 0x01)
  #define PRO_ME_BASEBOARD
#elif (PRO_SELECT == 0x02)
  #define PRO_ME_ORION
#elif (PRO_SELECT == 0x03)
  #define PRO_ME_CORE
#elif (PRO_SELECT == 0x04)
  #define PRO_ME_SHIELD
#else
  #define PRO_ME_ORION
#endif
/*************************************************************************
 * Function Module Definition
**************************************************************************/
//#define DEV_TEMPERATURE_SENSOR
//#define DEV_GYRO
//#define DEV_RGBLED
//#define DEV_SEVSEG
//#define DEV_ENCODER
//#define DEV_IR
//#define DEV_INFRARED
//#define DEV_HUMITURE
//#define DEV_COMPASS
//#define DEV_LEDMATRIX
//#define DEV_STEPPER

#include <Servo.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#if defined(PRO_ME_BASEBOARD)
#include <MeBaseBoard.h>
#elif defined(PRO_ME_ORION)
#include <MeOrion.h>
#elif defined(PRO_ME_CORE)
#include <MeMCore.h>
#elif defined(PRO_ME_SHIELD)
#include <MeShield.h>
#endif

MeDCMotor dc;
MeUltrasonicSensor us;
MeJoystick joystick;
MePort generalDevice;
#if (defined PRO_ME_CORE)
MeBuzzer buzzer;
#endif // PRO_ME_CORE
MeFlameSensor FlameSensor;
MeGasSensor GasSensor;
Servo servos[8];

#if defined(DEV_TEMPERATURE_SENSOR)
MeTemperature ts;
#endif // DEV_TEMPERATURE_SENSOR

#if defined(DEV_RGBLED)
MeRGBLed led(0,30);
#endif // DEV_RGBLED

#if (defined DEV_SEVSEG)
Me7SegmentDisplay seg;
#endif // DEV_SEVSEG

#if (defined DEV_LEDMATRIX)
MeLEDMatrix ledMx;
#endif // DEV_LEDMATRIX

#if ((defined DEV_IR) && (defined PRO_ME_CORE))
MeIR ir;
#endif // DEV_IR && PRO_ME_CORE

#if (defined DEV_GYRO)
MeGyro gyro;
#endif // DEV_GYRO

#if (defined DEV_COMPASS)
MeCompass Compass;
#endif // DEV_COMPASS

#if (defined DEV_HUMITURE)
MeHumiture humiture;
#endif // DEV_HUMITURE

#if (defined DEV_INFRARED)
MeInfraredReceiver *ir_read = NULL;
#endif // DEV_INFRARED

#if (defined DEV_STEPPER)
  #if defined(PRO_ME_SHIELD)
  MeStepper steppers[4];
  #elif ((defined PRO_ME_ORION) || (defined PRO_ME_BASEBOARD))
  MeStepper steppers[2];
  #endif
#endif // DEV_STEPPER

#if (defined DEV_ENCODER)
MeEncoderMotor *encoders[4] = {NULL,NULL,NULL,NULL};
#endif // DEV_ENCODER

union
{
  uint8_t byteVal[4];
  float floatVal;
  long longVal;
}val;

union
{
  uint8_t byteVal[4];
  double doubleVal;
}valDouble;

union
{
  uint8_t byteVal[2];
  short shortVal;
}valShort;

#if defined(__AVR_ATmega32U4__) 
const uint8_t analogs[12] PROGMEM = {A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11};
#elif defined(__AVR_ATmega328P__)
const uint8_t analogs[8] PROGMEM = {A0,A1,A2,A3,A4,A5,A6,A7};
#elif defined(__AVR_ATmega2560__)
const uint8_t analogs[16] PROGMEM = {A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15};
#endif

boolean isAvailable = false;
boolean isStart = false;
#if defined(PRO_ME_BASEBOARD) 
boolean isBluetooth = false;
#endif

#if ((defined DEV_IR) && (defined PRO_ME_CORE))
boolean irReady = false;
boolean irPressed = false;
#endif

#if (defined PRO_ME_CORE)
boolean buttonPressed = false;
#endif //PRO_ME_CORE

uint8_t len = 52;
uint8_t servo_pins[8]={0,0,0,0,0,0,0,0};
uint8_t buffer[52];
#if defined(PRO_ME_BASEBOARD) 
uint8_t bufferBt[52];
#endif // PRO_ME_BASEBOARD
uint8_t serialRead;
uint8_t prevc=0;
uint8_t index = 0;
uint8_t dataLen;
uint8_t modulesLen=0;
#if (defined DEV_INFRARED)
uint8_t irRead;
#endif // DEV_INFRARED

#if (defined DEV_ENCODER)
uint8_t i2c_id[2] = {0,0};
#endif // DEV_ENCODER

#if (defined DEV_IR) && (defined PRO_ME_CORE)
uint8_t irRead = 0;
int16_t irDelay = 0;
int16_t irIndex = 0;
#endif // DEV_IR && PRO_ME_CORE

float angleServo = 90.0;
double lastTime = 0.0;
double currentTime = 0.0;
double lastIRTime = 0.0;

#if (defined DEV_IR) && (defined PRO_ME_CORE)
String irBuffer = "";
#endif // DEV_IR && PRO_ME_CORE

String mVersion = "10.01.102";

#define VERSION              0
#define ULTRASONIC_SENSOR    1
#define TEMPERATURE_SENSOR   2
#define LIGHT_SENSOR         3
#define POTENTIONMETER       4
#define JOYSTICK             5
#define GYRO                 6
#define SOUND_SENSOR         7
#define RGBLED               8
#define SEVSEG               9
#define MOTOR                10
#define SERVO                11
#define ENCODER              12
#define IR                   13
#define IRREMOTE             14
#define PIRMOTION            15
#define INFRARED             16
#define LINEFOLLOWER         17
#define IRREMOTECODE         18
#define SHUTTER              20
#define LIMITSWITCH          21
#define BUTTON               22
#define HUMITURE             23
#define FLAMESENSOR          24
#define GASSENSOR            25
#define COMPASS              26
#define DIGITAL              30
#define ANALOG               31
#define PWM                  32
#define SERVO_PIN            33
#define TONE                 34
#define BUTTON_INNER         35
#define ULTRASONIC_ARDUINO   36
#define PULSEIN              37
#define STEPPER              40
#define LEDMATRIX            41
#define TIMER                50

#define GET 1
#define RUN 2
#define RESET 4
#define START 5

uint8_t readBuffer(uint8_t index)
{
#if defined(PRO_ME_BASEBOARD)  
  if(isBluetooth)
  {
    return bufferBt[index];
  }
  else
  {
    return buffer[index];
  }
#else //PRO_ME_BASEBOARD
  return buffer[index];
#endif //PRO_ME_BASEBOARD
}

void writeBuffer(int16_t index,uint8_t c)
{
#if defined(PRO_ME_BASEBOARD)  
  if(isBluetooth)
  {
    bufferBt[index]=c;
  }
  else
  {
    buffer[index]=c;
  }
#else //PRO_ME_BASEBOARD
  buffer[index]=c;
#endif //PRO_ME_BASEBOARD
}

void writeHead(void)
{
  writeSerial(0xff);
  writeSerial(0x55);
}

void writeEnd(void)
{
#if defined(PRO_ME_BASEBOARD)  
  if(isBluetooth)
  {
    Serial1.println();
  }
  else
  {
    Serial.println();
  }
#else //PRO_ME_BASEBOARD
  Serial.println();
#endif //PRO_ME_BASEBOARD 
}

void writeSerial(uint8_t c)
{
#if defined(PRO_ME_BASEBOARD)  
  if(isBluetooth)
  {
    Serial1.write(c);
  }
  else
  {
    Serial.write(c);
  }
#else //PRO_ME_BASEBOARD
  Serial.write(c);
#endif //PRO_ME_BASEBOARD 
}

void readSerial(void)
{
#if defined(PRO_ME_BASEBOARD)  
  if(Serial1.available() > 0)
  {
    isAvailable = true;
    isBluetooth = true;
    serialRead = Serial1.read();
  }
#else
  isAvailable = false;
  if(Serial.available() > 0)
  {
    isAvailable = true;
    serialRead = Serial.read();
  }
#endif
}

/*
ff 55 len idx action device port slot data a
0  1  2   3   4      5      6    7    8
*/
void parseData()
{
  isStart = false;
  uint8_t idx = readBuffer(3);
  uint8_t action = readBuffer(4);
  uint8_t device = readBuffer(5);
  switch(action)
  {
    case GET:
    {
      writeHead();
      writeSerial(idx);
      readSensor(device);
      writeEnd();
    }
    break;
    case RUN:
    {
      runModule(device);
      callOK();
    }
    break;
    case RESET:{
      //reset
      dc.reset(M1);
      dc.run(0);
      dc.reset(M2);
      dc.run(0);
#if (defined PRO_ME_CORE)
      buzzer.noTone();
#elif ((defined PRO_ME_CORE) || (defined PRO_ME_BASEBOARD))
      buzzerOff();
#endif // PRO_ME_CORE
#if (defined DEV_ENCODER)
    if(encoders[0] == NULL)
    {
      encoders[0]->runSpeed(0);
    }
    else if(encoders[1] == NULL)
    {
      encoders[1]->runSpeed(0);
    }
    else if(encoders[2] == NULL)
    {
      encoders[2]->runSpeed(0);
    }
    else if(encoders[3] == NULL)
    {
      encoders[3]->runSpeed(0);
    }
#endif // DEV_ENCODER
      callOK();
    }
    break;
    case START:
    {
      //start
      callOK();
    }
    break;
  }
}

void callOK(void)
{
  writeSerial(0xff);
  writeSerial(0x55);
  writeEnd();
}

void sendByte(uint8_t c)
{
  writeSerial(1);
  writeSerial(c);
}

void sendString(String s)
{
  int16_t l = s.length();
  writeSerial(4);
  writeSerial(l);
  for(int16_t i=0;i<l;i++)
  {
    writeSerial(s.charAt(i));
  }
}

void sendFloat(float value)
{ 
  writeSerial(2);
  val.floatVal = value;
  writeSerial(val.byteVal[0]);
  writeSerial(val.byteVal[1]);
  writeSerial(val.byteVal[2]);
  writeSerial(val.byteVal[3]);
}

void sendShort(short value)
{
  writeSerial(3);
  valShort.shortVal = value;
  writeSerial(valShort.byteVal[0]);
  writeSerial(valShort.byteVal[1]);
}

void sendDouble(double value)
{
  writeSerial(5);
  valDouble.doubleVal = value;
  writeSerial(valDouble.byteVal[0]);
  writeSerial(valDouble.byteVal[1]);
  writeSerial(valDouble.byteVal[2]);
  writeSerial(valDouble.byteVal[3]);
}

void sendLong(long value)
{
  writeSerial(6);
  writeSerial(2);
  val.longVal = value;
  writeSerial(val.byteVal[0]);
  writeSerial(val.byteVal[1]);
  writeSerial(val.byteVal[2]);
  writeSerial(val.byteVal[3]);
}

short readShort(int16_t idx)
{
  valShort.byteVal[0] = readBuffer(idx);
  valShort.byteVal[1] = readBuffer(idx+1);
  return valShort.shortVal; 
}

float readFloat(int16_t idx)
{
  val.byteVal[0] = readBuffer(idx);
  val.byteVal[1] = readBuffer(idx+1);
  val.byteVal[2] = readBuffer(idx+2);
  val.byteVal[3] = readBuffer(idx+3);
  return val.floatVal;
}

float readLong(int16_t idx)
{
  val.byteVal[0] = readBuffer(idx);
  val.byteVal[1] = readBuffer(idx+1);
  val.byteVal[2] = readBuffer(idx+2);
  val.byteVal[3] = readBuffer(idx+3);
  return val.longVal;
}

char _receiveStr[20] = {};
uint8_t _receiveUint8[16] = {};

char* readString(int16_t idx,int16_t len)
{
  for(int16_t i=0;i<len;i++)
  {
    _receiveStr[i]=readBuffer(idx+i);
  }
  _receiveStr[len] = '\0';
  return _receiveStr;
}

uint8_t* readUint8(int16_t idx,int16_t len)
{
  for(int16_t i=0;i<len;i++)
  {
    if(i>15)
    {
      break;
    }
    _receiveUint8[i] = readBuffer(idx+i);
  }
  return _receiveUint8;
}

#if (defined DEV_ENCODER)
uint8_t SearchI2CID(uint8_t i2cid)
{
  for(uint8_t i=0; i<2; i++)
  {
    if(i2c_id[i] == i2cid)
    {
      return i;
    }
    if(i2c_id[i] == 0)
    {
      i2c_id[i] = i2cid;
      return i;
    }
  }
  i2c_id[0] = i2cid;
  return 0;
}
#endif // DEV_ENCODER

uint8_t searchServoPin(uint8_t pin)
{
  for(uint8_t i=0; i<8; i++)
  {
    if(servo_pins[i] == pin)
    {
      return i;
    }
    if(servo_pins[i] == 0)
    {
      servo_pins[i] = pin;
      return i;
    }
  }
  return 0;
}

void runModule(uint8_t device)
{
  //0xff 0x55 0x6 0x0 0x2 0x22 0x9 0x0 0x0 0xa 
  uint8_t port = readBuffer(6);
  uint8_t pin = port;
  switch(device)
  {
    case MOTOR:
    {
      int16_t speed = readShort(7);
      if(dc.getPort() != port)
      {
        dc.reset(port);
      }
      dc.run(speed);
    } 
    break;
    case JOYSTICK:
    {
      int16_t leftSpeed = readShort(6);
      dc.reset(M1);
      dc.run(leftSpeed);
      int16_t rightSpeed = readShort(8);
      dc.reset(M2);
      dc.run(rightSpeed);
    }
    break;
#if (defined DEV_STEPPER)
    case STEPPER:
    {
      int16_t maxSpeed = readShort(7);
      long distance = readLong(9);
#if ((defined PRO_ME_ORION) || (defined PRO_ME_BASEBOARD) || (defined PRO_ME_SHIELD))
      if(port == PORT_1)
      {
        steppers[0] = MeStepper(PORT_1);
        steppers[0].moveTo(distance);
        steppers[0].setMaxSpeed(maxSpeed);
        steppers[0].setSpeed(maxSpeed);
      }
      else if(port == PORT_2)
      {
        steppers[1] = MeStepper(PORT_2);
        steppers[1].moveTo(distance);
        steppers[1].setMaxSpeed(maxSpeed);
        steppers[1].setSpeed(maxSpeed);
      }
#endif
#if defined(PRO_ME_SHIELD)
      else if(port == M1)
      {
        steppers[2] = MeStepper(M1);
        steppers[2].moveTo(distance);
        steppers[2].setMaxSpeed(maxSpeed);
        steppers[2].setSpeed(maxSpeed);
      }
      else if(port == M2)
      {
        steppers[3] = MeStepper(M2);
        steppers[3].moveTo(distance);
        steppers[3].setMaxSpeed(maxSpeed);
        steppers[3].setSpeed(maxSpeed);
      }
#endif
    }
    break;
#endif // DEV_STEPPER
#if (defined DEV_ENCODER)
    case ENCODER:
    {
      uint8_t i2cId = port;
      uint8_t i2c_id_index;
      uint8_t i2c_index;
      int8_t slot = readBuffer(7);
      int16_t maxSpeed = readShort(8);
      long distance = readLong(10);
      i2c_id_index = SearchI2CID(i2cId);
      i2c_index = (i2c_id_index * 2 + slot) - 1;
      if(encoders[i2c_index] == NULL)
      {
        encoders[i2c_index] = new MeEncoderMotor(i2cId,slot);
        encoders[i2c_index]->begin();
        delay(50);
        encoders[i2c_index]->runSpeed(0);
      }
      else if(i2c_id[i2c_id_index] != i2cId)
      {
        delete encoders[i2c_index];
        encoders[i2c_index] = new MeEncoderMotor(i2cId,slot);
        encoders[i2c_index]->begin();
        delay(50);
        encoders[i2c_index]->runSpeed(0);
      }
      encoders[i2c_index]->move(distance,maxSpeed);
    }
    break;
#endif // DEV_ENCODER
#if (defined DEV_RGBLED)
    case RGBLED:
    {
      uint8_t slot = readBuffer(7);
      uint8_t idx = readBuffer(8);
      uint8_t r = readBuffer(9);
      uint8_t g = readBuffer(10);
      uint8_t b = readBuffer(11);
      led.reset(port,slot);
      if(idx > 0)
      {
       led.setColorAt(idx-1,r,g,b); 
      }
	  else
      {
        led.setColor(r,g,b); 
      }
      led.show();
    }
    break;
#endif // DEV_RGBLED
    case SERVO:
    {
      uint8_t slot = readBuffer(7);
      pin = slot==1?mePort[port].s1:mePort[port].s2;
      uint8_t v = readBuffer(8);
      Servo sv = servos[searchServoPin(pin)];
      if(v >= 0 && v <= 180)
      {
        if(port > 0)
        {
          sv.attach(pin);
          sv.write(v);
        }
      }
    }
    break;
#if (defined DEV_SEVSEG)
    case SEVSEG:
    {
      if(seg.getPort() != port)
      {
        seg.reset(port);
      }
      float v = readFloat(7);
      seg.display(v);
    }
    break;
#endif // DEV_SEVSEG
#if (defined DEV_LEDMATRIX)
    case LEDMATRIX:
    {
      if(ledMx.getPort() != port)
      {
        ledMx.reset(port);
      }
      uint8_t action = readBuffer(7);
      if(action == 1)
      {
        uint8_t brightness = readBuffer(8);
        uint8_t color = readBuffer(9);
        int16_t px = readShort(10);
        int16_t py = readShort(12);
        uint8_t len = readBuffer(14);
        char *s = readString(15,len);
        ledMx.clearScreen();
        ledMx.setColorIndex(color);
        ledMx.setBrightness(brightness);
        ledMx.drawStr(px,py,s);
      }
      else if(action == 2)
      {
        uint8_t brightness = readBuffer(8);
        uint8_t dw = readBuffer(9);
        int16_t px = readShort(10);
        int16_t py = readShort(12);
        uint8_t len = readBuffer(14);
        uint8_t *ss = readUint8(15,len);
        ledMx.clearScreen();
        ledMx.setColorIndex(1);
        ledMx.setBrightness(brightness);
        ledMx.drawBitmap(px,py,dw,ss);
      }
      else if(action == 3)
      {
        uint8_t brightness = readBuffer(8);
        uint8_t point = readBuffer(9);
        int16_t hours = readShort(10);
        int16_t minutes = readShort(12);
        ledMx.clearScreen();
        ledMx.setColorIndex(1);
        ledMx.setBrightness(brightness);
        ledMx.showClock(hours,minutes,point);
      }
    }
    break;
#endif // DEV_LEDMATRIX
    case LIGHT_SENSOR:
    {
      if(generalDevice.getPort() != port)
      {
        generalDevice.reset(port);
      }
      uint8_t v = readBuffer(7);
      generalDevice.dWrite1(v);
    }
    break;
#if (defined DEV_IR) && (defined PRO_ME_CORE)
    case IR:
    {
      String Str_data;
      int16_t len = readBuffer(2)-3;
      for(int16_t i=0;i<len;i++)
      {
        Str_data+=(char)readBuffer(6+i);
      }
      ir.sendString(Str_data);
      Str_data = "";
    }
    break;
#endif // DEV_IR && PRO_ME_CORE
    case SHUTTER:
    {
      if(generalDevice.getPort() != port)
	  {
       generalDevice.reset(port);
      }
      uint8_t v = readBuffer(7);
      if(v < 2)
	  {
        generalDevice.dWrite1(v);
      }
	  else
	  {
        generalDevice.dWrite2(v-2);
      }
    }
    break;
    case DIGITAL:
    {
      pinMode(pin,OUTPUT);
      uint8_t v = readBuffer(7);
      digitalWrite(pin,v);
    }
    break;
    case PWM:
    {
      pinMode(pin,OUTPUT);
      uint8_t v = readBuffer(7);
      analogWrite(pin,v);
    }
    break;
#if (defined PRO_ME_CORE)
    case TONE:
    {
      int16_t hz = readShort(6);
      int16_t tone_time = readShort(8);
      if(hz > 0)
      {
        buzzer.tone(hz,tone_time);
      }
	  else
	  {
        buzzer.noTone(); 
      }
    }
    break;
#endif // PRO_ME_CORE
    case SERVO_PIN:
    {
      uint8_t v = readBuffer(7);
      if(v >= 0 && v <= 180)
      {
        Servo sv = servos[searchServoPin(pin)];
        sv.attach(pin);
        sv.write(v);
      }
    }
    break;
    case TIMER:
    {
      lastTime = millis()/1000.0; 
    }
    break;
	default:
    break;
  }
}

void readSensor(uint8_t device)
{
  /**************************************************
      ff    55      len idx action device port slot data a
      0     1       2   3   4      5      6    7    8
      0xff  0x55   0x4 0x3 0x1    0x1    0x1  0xa 
  ***************************************************/
  uint8_t port,slot,pin;
  float value = 0.0;
  port = readBuffer(6);
  pin = port;
  switch(device)
  {
    case  ULTRASONIC_SENSOR:
    {
      if(us.getPort() != port)
      {
        us.reset(port);
      }
      value = (float)us.distanceCm(50000);
      sendFloat(value);
    }
    break;
#if defined(DEV_TEMPERATURE_SENSOR)
    case  TEMPERATURE_SENSOR:
    {
      slot = readBuffer(7);
      if((ts.getPort() != port) || (ts.getSlot() != slot))
      {
        ts.reset(port,slot);
      }
      value = ts.temperature();
      sendFloat(value);
    }
    break;
#endif // DEV_TEMPERATURE_SENSOR
    case  LIGHT_SENSOR:
    case  SOUND_SENSOR:
    case  POTENTIONMETER:
    {
      if(generalDevice.getPort() != port)
      {
        generalDevice.reset(port);
        pinMode(generalDevice.pin2(),INPUT);
      }
      value = (float)generalDevice.aRead2();
      sendFloat(value);
    }
    break;
    case  JOYSTICK:
    {
      slot = readBuffer(7);
      if(joystick.getPort() != port)
      {
        joystick.reset(port);
      }
      value = joystick.read(slot);
      sendFloat(value);
    }
    break;
#if (defined DEV_IR) && (defined PRO_ME_CORE)
    case  IR:
   	{
      if(irReady)
      {
        sendString(irBuffer);
        irReady = false;
        irBuffer = "";
      }
    }
    break;
    case IRREMOTE:
    {
      uint8_t r = readBuffer(7);
      if((millis()/1000.0-lastIRTime) > 0.2)
      {
        sendByte(0);
      }
      else
      {
        sendByte(irRead==r);
      }
      //irRead = 0;
      irIndex = 0;
    }
    break;
    case IRREMOTECODE:
    {
      if(irRead<0xff)
      {
        sendByte(irRead);
      }
      irRead = 0;
      irIndex = 0;
    }
    break;
#endif // DEV_IR && PRO_ME_CORE
#if (defined DEV_INFRARED)
    case  INFRARED:
    {
      if(ir_read == NULL)
      {
        ir_read = new MeInfraredReceiver(port);
        ir_read->begin();
      }
      else if(ir_read->getPort() != port)
      {
        delete ir_read;
        ir_read = new MeInfraredReceiver(port);
        ir_read->begin();
      }
      irRead = ir_read->getCode();
      if((irRead < 255) && (irRead > 0))
      {
        sendFloat((float)irRead);
      }
      else
      {
        sendFloat(0);
      }
    }
    break;
#endif // DEV_INFRARED
    case PIRMOTION:
    {
      if(generalDevice.getPort() != port)
      {
        generalDevice.reset(port);
        pinMode(generalDevice.pin2(),INPUT);
      }
      value = generalDevice.dRead2();
      sendFloat(value);
    }
    break;
    case  LINEFOLLOWER:
    {
      if(generalDevice.getPort()!=port)
      {
        generalDevice.reset(port);
        pinMode(generalDevice.pin1(),INPUT);
        pinMode(generalDevice.pin2(),INPUT);
      }
      value = generalDevice.dRead1()*2+generalDevice.dRead2();
      sendFloat(value);
    }
    break;
    case LIMITSWITCH:
    {
      slot = readBuffer(7);
      if(generalDevice.getPort() != port ||
	  	 generalDevice.getSlot() != slot)
	  {
        generalDevice.reset(port,slot);
      }
      if(slot==1)
      {
        pinMode(generalDevice.pin1(),INPUT_PULLUP);
        value = generalDevice.dRead1();
      }
      else
      {
        pinMode(generalDevice.pin2(),INPUT_PULLUP);
        value = generalDevice.dRead2();
      }
      sendFloat(value);  
    }
    break;
#if (defined PRO_ME_CORE)
    case BUTTON_INNER:
    {
      //pin = analogs[pin];
      pin = pgm_read_byte(&analogs[pin]);
      int8_t s = readBuffer(7);
      pinMode(pin,INPUT);
      boolean currentPressed = !(analogRead(pin)>10);
      sendByte(s^(currentPressed?1:0));
      buttonPressed = currentPressed;
    }
    break;
#endif

#if (defined DEV_COMPASS)
    case COMPASS:
    {
      if(Compass.getPort() != port)
      {
        Compass.reset(port);
        Compass.setpin(Compass.pin1(),Compass.pin2());
      }
      double CompassAngle;
      CompassAngle = Compass.getAngle();
      sendDouble(CompassAngle);
    }
    break;
#endif // DEV_COMPASS
#if (defined DEV_HUMITURE)
    case HUMITURE:
    {
      uint8_t index = readBuffer(7);
      if(humiture.getPort() != port)
      {
        humiture.reset(port);
      }
      uint8_t HumitureData;
      humiture.update();
      HumitureData = humiture.getValue(index);
      sendByte(HumitureData);
    }
    break;
#endif // DEV_HUMITURE
    case FLAMESENSOR:
    {
      if(FlameSensor.getPort() != port)
      {
        FlameSensor.reset(port);
        FlameSensor.setpin(FlameSensor.pin2(),FlameSensor.pin1());
      }
      int16_t FlameData; 
      FlameData = FlameSensor.readAnalog();
      sendShort(FlameData);
    }
    break;
    case GASSENSOR:{
      if(GasSensor.getPort() != port)
      {
        GasSensor.reset(port);
        GasSensor.setpin(GasSensor.pin2(),GasSensor.pin1());
      }
      int16_t GasData; 
      GasData = GasSensor.readAnalog();
      sendShort(GasData);
    }
    break;
#if (defined DEV_GYRO)
    case  GYRO:
    {
      int8_t axis = readBuffer(7);
      gyro.update();
      value = gyro.getAngle(axis);
      sendFloat(value);
    }
    break;
#endif // DEV_GYRO
    case  VERSION:
    {
      sendString(mVersion);
    }
    break;
    case  DIGITAL:
    {
      pinMode(pin,INPUT);
      sendFloat(digitalRead(pin));
    }
    break;
    case  ANALOG:
    {
      //pin = analogs[pin];
      pin = pgm_read_byte(&analogs[pin]);
      pinMode(pin,INPUT);
      sendFloat(analogRead(pin));
    }
    break;
    case TIMER:
    {
      sendFloat(currentTime);
    }
    break;
	default:
	break;
  }
}

void setup()
{
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  delay(300);
  digitalWrite(13,LOW);
  Serial.begin(115200);
  delay(500);
#if (defined PRO_ME_CORE)
  buzzer.tone(500,50); 
  delay(100);
  buzzer.noTone();
#elif ((defined PRO_ME_CORE) || (defined PRO_ME_BASEBOARD))
  buzzerOn();
  delay(100);
  buzzerOff();
#endif // PRO_ME_CORE

#if (defined DEV_IR) && (defined PRO_ME_CORE)
  ir.begin();
#endif // DEV_IR && PRO_ME_CORE

#if ((defined DEV_RGBLED) && (defined PRO_ME_CORE))
  led.setpin(13);
  led.setColor(0,0,0);
  led.show();
#endif // DEV_RGBLED && PRO_ME_CORE

#if (defined DEV_GYRO)
  gyro.begin();
#endif // DEV_GYRO
  Serial.print("Version: ");
  Serial.println(mVersion);
}

void loop()
{
  currentTime = millis()/1000.0-lastTime;
#if (defined DEV_INFRARED)
  if(ir_read != NULL)
  {
    ir_read->loop();
  }
#endif // DEV_INFRARED
#if ((defined DEV_IR) && (defined PRO_ME_CORE))
  if(ir.decode())
  {
    irRead = ((ir.value>>8)>>8)&0xff;
    lastIRTime = millis()/1000.0;
    irPressed = true;
    if(irRead == 0xa || irRead == 0xd)
    {
      irIndex = 0;
      irReady = true;
    }
	else
	{
      irBuffer+=irRead; 
      irIndex++;
      if(irIndex>64)
	  {
        irIndex = 0;
        irBuffer = "";
      }
    }
    irDelay = 0;
  }
  else
  {
    irDelay++;
    if(irRead > 0)
    {
      if(irDelay > 5000)
      {
        irRead = 0;
        irDelay = 0;
      }
    }
  }
#endif // DEV_IR && PRO_ME_CORE
  readSerial();
#if (defined DEV_STEPPER)
  #if ((defined PRO_ME_ORION) || (defined PRO_ME_BASEBOARD) || (defined PRO_ME_SHIELD))
  steppers[0].runSpeedToPosition();
  steppers[1].runSpeedToPosition();
  #endif
  #if defined(PRO_ME_SHIELD)
  steppers[2].runSpeedToPosition();
  steppers[3].runSpeedToPosition();
  #endif
#endif // DEV_STEPPER
  if(isAvailable)
  {
    unsigned char c = serialRead&0xff;
    if((c == 0x55) && (isStart == false))
    {
      if(prevc == 0xff)
      {
        index=1;
        isStart = true;
      }
    }
    else
    {
      prevc = c;
      if(isStart)
      {
        if(index == 2)
        {
          dataLen = c; 
        }
        else if(index>2)
        {
          dataLen--;
        }
        writeBuffer(index,c);
      }
    }
    index++;
    if(index > 51)
    {
      index=0; 
      isStart=false;
    }
    if(isStart && (dataLen == 0) && (index > 3))
    { 
      isStart = false;
      parseData(); 
      index=0;
    }
  }
}
