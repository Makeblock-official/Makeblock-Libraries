#include "MeEncoderNew.h"
#define HOLD 0x40
#define SYNC 0x80
// move state and function
#define CMD_RESET 0x00
#define CMD_MOVE_TO 0x01
#define CMD_BREAK 0x02
#define CMD_MOVE_SPD 0x03
#define CMD_MOVE_TO_SPD 0x04
// config function
#define CMD_SET_PID 0x10
#define CMD_SET_HOLD 0x11
#define CMD_SET_POWER 0x12
#define CMD_SET_MODE 0x13
#define CMD_SET_PWM 0x14
#define CMD_SET_RATIO 0x15
#define CMD_SET_PULSE 0x16
#define CMD_SET_DEVID 0x17
// get motor status
#define CMD_GET_PID 0x20
#define CMD_GET_POWER 0x21
#define CMD_GET_POS 0x22
#define CMD_GET_SPEED 0x23
#define CMD_GET_RATIO 0x24
#define CMD_GET_PULSE 0x25

MeEncoderNew::MeEncoderNew(uint8_t addr,uint8_t slot)
{
  _slot = slot - 1;
  address = addr;
}
MeEncoderNew::MeEncoderNew(uint8_t slot)
{
  _slot = slot - 1;
  address = 0x9;
}
MeEncoderNew::MeEncoderNew()
{
  address = 0x09;
}

void MeEncoderNew::begin()
{
  Wire.begin();
}

void MeEncoderNew::move(long angle, int speed)
{
  cmdBuf[0] = _slot;
  cmdBuf[1] = CMD_MOVE_TO;
  memcpy(cmdBuf + 2, &angle, 4);
  memcpy(cmdBuf + 6,&speed,2);
  sendCmd();
}

void MeEncoderNew::moveTo(long angle, int speed)
{
  cmdBuf[0] = _slot;
  cmdBuf[1] = CMD_MOVE_TO;
  memcpy(cmdBuf + 2, &angle, 4);
  memcpy(cmdBuf + 6,&speed,2);
  sendCmd();
}

void MeEncoderNew::moveTo(long angle)
{
  cmdBuf[0] = _slot;
  cmdBuf[1] = CMD_MOVE_TO;
  memcpy(cmdBuf + 2, &angle, 4);
  sendCmd();
}

void MeEncoderNew::runSpeed(int speed)
{
  cmdBuf[0] = _slot;
  cmdBuf[1] = CMD_MOVE_SPD;
  memcpy(cmdBuf+2,&speed,2);
  sendCmd();
}

void MeEncoderNew::runSpeedAndTime(int speed, float time)
{
  if(_lastTime == 0)
  {
     _lastTime = millis();
	 runSpeed(speed);
  }

  if(millis() - _lastTime > (1000*time))
  {
    _lastTime = 0;
    runSpeed(0);
  }
}

void MeEncoderNew::runTurns(int turns, int speed)
{
    return move(turns * 360, speed);
}

void MeEncoderNew::movetoSpeed(long angle,int speed)
{
  cmdBuf[0] = _slot;
  cmdBuf[1] = CMD_MOVE_TO_SPD;
  memcpy(cmdBuf + 2, &angle, 4);
  memcpy(cmdBuf + 6,&speed,2);
  sendCmd();
}

void MeEncoderNew::setHold(uint8_t hold)
{
  cmdBuf[0] = _slot;
  cmdBuf[1] = CMD_SET_HOLD;
  cmdBuf[2] = hold;
  sendCmd();
}

void MeEncoderNew::reset()
{
  cmdBuf[0] = _slot;
  cmdBuf[1] = CMD_RESET;
  sendCmd();
}

void MeEncoderNew::setPID(float p,float i,float d,float s)
{
  cmdBuf[0] = _slot;
  cmdBuf[1] = CMD_SET_PID;
  memcpy(&cmdBuf[2],&p,4);
  memcpy(&cmdBuf[6],&i,4);
  memcpy(&cmdBuf[10],&d,4);
  memcpy(&cmdBuf[14],&s,4);
  sendCmd();
}

void MeEncoderNew::setMaxPower(int8_t maxPower)
{
  cmdBuf[0] = _slot;
  cmdBuf[1] = CMD_SET_POWER;
  cmdBuf[2] = maxPower;
  sendCmd();
}

void MeEncoderNew::setMode(uint8_t mode)
{
  cmdBuf[0] = _slot;
  cmdBuf[1] = CMD_SET_MODE;
  cmdBuf[2] = mode;
  sendCmd();
}
void MeEncoderNew::setPWM(int pwm)
{
  cmdBuf[0] = _slot;
  cmdBuf[1] = CMD_SET_PWM;
  memcpy(cmdBuf+2,&pwm,2);
  sendCmd();
}
long MeEncoderNew::getCurrentPosition()
{
  long pos;
  char buf[8];
  Wire.beginTransmission(address); 
  Wire.write(_slot);      
  Wire.write(CMD_GET_POS);            
  Wire.endTransmission(0);
  Wire.requestFrom(address,(uint8_t)4);   
  for(int i=0;i<4;i++)   
  { 
    buf[i] = Wire.read();
  }
  pos = *(long*)buf;
  return pos;
}

void MeEncoderNew::getPID(float * p,float * i,float * d,float * s)
{
  char buf[8];

  Wire.beginTransmission(address); 
  Wire.write(_slot);       
  Wire.write(CMD_GET_PID);             
  Wire.endTransmission(0);
  Wire.requestFrom(address,(uint8_t)16);  

  for(int i=0;i<4;i++)   
  { 
    buf[i] = Wire.read(); 
  }
  *p = *(float*)buf;
  for(int i=0;i<4;i++)   
  { 
    buf[i] = Wire.read(); 
  }
  *i = *(float*)buf;
    for(int i=0;i<4;i++)   
  { 
    buf[i] = Wire.read(); 
  }
  *d = *(float*)buf;
    for(int i=0;i<4;i++)   
  { 
    buf[i] = Wire.read(); 
  }
  *s = *(float*)buf;
}

int MeEncoderNew::getCurrentSpeed()
{
  char buf[8];
  int speed;
  Wire.beginTransmission(address); 
  Wire.write(_slot);      
  Wire.write(CMD_GET_SPEED);             
  Wire.endTransmission(0);
  Wire.requestFrom(address,(uint8_t)2);   
  for(int i=0;i<2;i++)   
  { 
    buf[i] = Wire.read(); 
  }
  speed = *(int*)buf;
  return speed;
}

int8_t MeEncoderNew::getPower()
{
  Wire.beginTransmission(address); 
  Wire.write(_slot);       
  Wire.write(CMD_GET_POWER);              
  Wire.endTransmission(0);
  Wire.requestFrom(address,(uint8_t)1);   
  return (int8_t)Wire.read();
}

void MeEncoderNew::sendCmd()
{
  Wire.beginTransmission(address); 
  for(int i=0;i<18;i++)
    Wire.write(cmdBuf[i]);       
  Wire.endTransmission();
}

float MeEncoderNew::getRatio()
{ 
  char buf[8];
  float ratio;
  Wire.beginTransmission(address); 
  Wire.write(_slot);      
  Wire.write(CMD_GET_RATIO);             
  Wire.endTransmission(0);
  Wire.requestFrom(address,(uint8_t)4);   
  for(int i=0;i<4;i++)   
  { 
    buf[i] = Wire.read(); 
  }
  ratio = *(float*)buf;
  return ratio;
}

void MeEncoderNew::setRatio(float ratio)
{
  cmdBuf[0] = _slot;
  cmdBuf[1] = CMD_SET_RATIO;
  memcpy(cmdBuf+2,&ratio,4);
  sendCmd();
}

int MeEncoderNew::getPulse()
{ 
  char buf[8];
  int pulse;
  Wire.beginTransmission(address); 
  Wire.write(_slot);      
  Wire.write(CMD_GET_PULSE);             
  Wire.endTransmission(0);
  Wire.requestFrom(address,(uint8_t)2);   
  for(int i=0;i<2;i++)   
  { 
    buf[i] = Wire.read(); 
  }
  pulse = *(int*)buf;
  return pulse;
}

void MeEncoderNew::setPulse(int pulse)
{
  cmdBuf[0] = _slot;
  cmdBuf[1] = CMD_SET_PULSE;
  memcpy(cmdBuf+2,&pulse,2);
  sendCmd();
}

void MeEncoderNew::setDevid(int devid)
{
  cmdBuf[0] = _slot;
  cmdBuf[1] = CMD_SET_DEVID;
  memcpy(cmdBuf+2,&devid,2);
  sendCmd();
}
