#ifndef ME_LEGOENCODER_H
#define ME_LEGOENCODER_H
#include <Arduino.h>
#include <stdbool.h>
#include "MeConfig.h"
#include "MePort.h"

#define LEONARDO_PORT1

#if defined(UNO_PORT1) //10:PB3 11:PB2
#define SDA_PORT PORTB
#define SDA_PIN 3
#define SCL_PORT PORTB
#define SCL_PIN 2
#define I2C_SLOWMODE 1
#elif defined(UNO_PORT2) //3:PD3 9:PB1
#define SDA_PORT PORTD
#define SDA_PIN 3
#define SCL_PORT PORTB
#define SCL_PIN 1
#define I2C_SLOWMODE 1
#elif defined(LEONARDO_PORT1) //11:PB7 8:PB4
#define SDA_PORT PORTB
#define SDA_PIN 7
#define SCL_PORT PORTB
#define SCL_PIN 4
#define I2C_SLOWMODE 1
#elif defined(LEONARDO_PORT2) //13:PC7 12:PD6
#define SDA_PORT PORTC
#define SDA_PIN 7
#define SCL_PORT PORTD
#define SCL_PIN 6
#define I2C_SLOWMODE 1
#else
// change sda scl if you use a different pin mapping
#endif

#define PULSE_PER_C 8 

class MeEncoderNew{
public:
  MeEncoderNew(uint8_t addr,uint8_t slot);
  MeEncoderNew(uint8_t slot);
  MeEncoderNew();
  void begin();
  void reset();
  void move(long angle, int speed);
  void moveTo(long angle, int speed);
  void runTurns(int turns, int speed);
  void runSpeed(int speed);
  void runSpeedAndTime(int speed, float time);
  int getCurrentSpeed();
  long getCurrentPosition();

  void moveTo(long angle);
  void movetoSpeed(long angle,int speed);
  void setHold(uint8_t hold);
  void setPID(float p,float i,float d,float s);
  void setMaxPower(int8_t maxPower);
  int8_t getPower();
  void getPID(float * p,float * i,float * d,float * s);
  float getRatio();
  void setRatio(float r);
  int getPulse();
  void setPulse(int p);
  void setDevid(int devid);
  void setMode(uint8_t mode);
  void setPWM(int pwm);
private:
  void sendCmd();
  uint8_t _slot; 
  uint8_t address;
  unsigned long _lastTime;
  char cmdBuf[18];
};

#endif



