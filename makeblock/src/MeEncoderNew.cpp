/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MeEncoderNew
 * \brief   Driver for Me Encoder New module.
 * @file    MeEncoderNew.cpp
 * @author  MakeBlock
 * @version V1.0.2
 * @date    2015/11/10
 * @brief   Driver for Me Encoder New module.
 *
 * \par Copyright
 * This software is Copyright (C), 2012-2016, MakeBlock. Use is subject to license \n
 * conditions. The main licensing options available are GPL V2 or Commercial: \n
 *
 * \par Open Source Licensing GPL V2
 * This is the appropriate option if you want to share the source code of your \n
 * application with everyone you distribute it to, and you also want to give them \n
 * the right to share who uses it. If you wish to use this software under Open \n
 * Source Licensing, you must contribute all your source code to the open source \n
 * community in accordance with the GPL Version 2 when your application is \n
 * distributed. See http://www.gnu.org/copyleft/gpl.html
 *
 * \par Description
 * This file is a drive for Me EncoderNew device, The Me EncoderNew inherited the 
 * MeSerial class from SoftwareSerial.
 *
 * \par Method List:
 *
 *    1. void MeEncoderNew::begin();
 *    2. void MeEncoderNew::reset();
 *    3. void MeEncoderNew::move(long angle, int speed);
 *    4. void MeEncoderNew::moveTo(long angle, int speed);
 *    5. void MeEncoderNew::runTurns(int turns, int speed);
 *    6. void MeEncoderNew::runSpeed(int speed);
 *    7. void MeEncoderNew::runSpeedAndTime(int speed, float time);
 *    8. int MeEncoderNew::getCurrentSpeed();
 *    9. long MeEncoderNew::getCurrentPosition();
 *    10. void MeEncoderNew::moveTo(long angle);
 *    11. void MeEncoderNew::movetoSpeed(long angle,int speed);
 *    12. void MeEncoderNew::setHold(uint8_t hold);
 *    13. void MeEncoderNew::setPID(float p,float i,float d,float s);
 *    14. void MeEncoderNew::setMaxPower(int8_t maxPower);
 *    15. int8_t MeEncoderNew::getPower();
 *    16. void MeEncoderNew::getPID(float * p,float * i,float * d,float * s);
 *    17. float MeEncoderNew::getRatio();
 *    18. void MeEncoderNew::setRatio(float r);
 *    19. int MeEncoderNew::getPulse();
 *    20. void MeEncoderNew::setPulse(int p);
 *    21. void MeEncoderNew::setDevid(int devid);
 *    22. void MeEncoderNew::setMode(uint8_t mode);
 *    23. void MeEncoderNew::setPWM(int pwm);
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan        2015/11/04     1.0.0            build the new
 * Mark Yan        2015/11/05     1.0.1            fix compile error
 * forfish         2015/11/10     1.0.2            Add description
 * </pre>
 *
 * @example EncoderMotorTestMoveTo.ino
 * @example EncoderMotorTestRunSpeed.ino
 * @example EncoderMotorTestRunSpeedAndTime.ino
 * @example EncoderMotorTestRunTurns.ino
 */

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

/**
 * Alternate Constructor which can call your own function to map the Encoder Motor New to arduino port,
 * you can set any slot for the Encoder Motor New device. 
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 * \param[in]
 *   slot - SLOT1 or SLOT2
 */
MeEncoderNew::MeEncoderNew(uint8_t addr,uint8_t slot)
{
  _slot = slot - 1;
  address = addr;
}

/**
 * Alternate Constructor which can call your own function to map the Encoder Motor New to arduino port,
 * you can set any slot for the Encoder Motor New device.
 * \param[in]
 *   slot - SLOT1 or SLOT2
 */
MeEncoderNew::MeEncoderNew(uint8_t slot)
{
  _slot = slot - 1;
  address = 0x9;
}

/**
 * Alternate Constructor which can call your own function to map the Encoder Motor New to arduino port,
 * you should initialized slot and slaveAddress here for the Encoder Motor New device.
 * \param[in]
 *   None
 */
MeEncoderNew::MeEncoderNew()
{
  address = 0x09;
}

/**
 * \par Function
 *    begin
 * \par Description
 *    Initialize Encoder Motor New.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderNew::begin()
{
  Wire.begin();
}

/**
 * \par Function
 *    move
 * \par Description
 *    Motor move.
 * \param[in]
 *    angle - The angle move of Motor New.
 * \param[in]
 *    speed - The speed move of Motor New.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderNew::move(long angle, int speed)
{
  cmdBuf[0] = _slot;
  cmdBuf[1] = CMD_MOVE_TO;
  memcpy(cmdBuf + 2, &angle, 4);
  memcpy(cmdBuf + 6,&speed,2);
  sendCmd();
}

/**
 * \par Function
 *    moveTo
 * \par Description
 *    Motor New move to the aim.
 * \param[in]
 *    angle - The angle move of Motor New.
 * \param[in]
 *    speed - The speed move of Motor New.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderNew::moveTo(long angle, int speed)
{
  cmdBuf[0] = _slot;
  cmdBuf[1] = CMD_MOVE_TO;
  memcpy(cmdBuf + 2, &angle, 4);
  memcpy(cmdBuf + 6,&speed,2);
  sendCmd();
}

/**
 * \par Function
 *    moveTo
 * \par Description
 *    Motor New move to the aim.
 * \param[in]
 *    angle - The angle move of Motor New.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderNew::moveTo(long angle)
{
  cmdBuf[0] = _slot;
  cmdBuf[1] = CMD_MOVE_TO;
  memcpy(cmdBuf + 2, &angle, 4);
  sendCmd();
}

/**
 * \par Function
 *    runSpeed
 * \par Description
 *    The speed of Motor's movement.
 * \param[in]
 *    speed - The speed move of Motor.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderNew::runSpeed(int speed)
{
  cmdBuf[0] = _slot;
  cmdBuf[1] = CMD_MOVE_SPD;
  memcpy(cmdBuf+2,&speed,2);
  sendCmd();
}

/**
 * \par Function
 *    runSpeedAndTime
 * \par Description
 *    The speed and time of Motor's movement.
 * \param[in]
 *    speed - The speed move of Motor.
 * \param[in]
 *    time - The time move of Motor.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
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

/**
 * \par Function
 *    runTurns
 * \par Description
 *    Motor move turns.
 * \param[in]
 *    turns - The turns move of Motor.
 * \param[in]
 *    speed - The speed move of Motor.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderNew::runTurns(int turns, int speed)
{
    return move(turns * 360, speed);
}

/**
 * \par Function
 *    movetoSpeed
 * \par Description
 *    Motor move to the set speed.
 * \param[in]
 *    angle - The angle move of Motor.
 * \param[in]
 *    speed - The speed move of Motor.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderNew::movetoSpeed(long angle,int speed)
{
  cmdBuf[0] = _slot;
  cmdBuf[1] = CMD_MOVE_TO_SPD;
  memcpy(cmdBuf + 2, &angle, 4);
  memcpy(cmdBuf + 6,&speed,2);
  sendCmd();
}

/**
 * \par Function
 *    setHold
 * \par Description
 *    Hold the state of Motor.
 * \param[in]
 *    hold - The time of Motor hold.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderNew::setHold(uint8_t hold)
{
  cmdBuf[0] = _slot;
  cmdBuf[1] = CMD_SET_HOLD;
  cmdBuf[2] = hold;
  sendCmd();
}

/**
 * \par Function
 *    reset
 * \par Description
 *    Reset the parameter of Motor.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderNew::reset()
{
  cmdBuf[0] = _slot;
  cmdBuf[1] = CMD_RESET;
  sendCmd();
}

/**
 * \par Function
 *    setPID
 * \par Description
 *    Set PID for Motor.
 * \param[in]
 *    p - P means Proportion.
 * \param[in]
 *    i - I means Integration.
 * \param[in]
 *    d - D means Differentiation.
 * \param[in]
 *    s - S means slot of Motor.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
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

/**
 * \par Function
 *    setMaxPower
 * \par Description
 *    Set the max power to Motor.
 * \param[in]
 *    maxPower - The size of power.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderNew::setMaxPower(int8_t maxPower)
{
  cmdBuf[0] = _slot;
  cmdBuf[1] = CMD_SET_POWER;
  cmdBuf[2] = maxPower;
  sendCmd();
}

/**
 * \par Function
 *    setMode
 * \par Description
 *    Set the work mode to Motor.
 * \param[in]
 *    mode - The work mode of Motor.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderNew::setMode(uint8_t mode)
{
  cmdBuf[0] = _slot;
  cmdBuf[1] = CMD_SET_MODE;
  cmdBuf[2] = mode;
  sendCmd();
}

/**
 * \par Function
 *    setPWM
 * \par Description
 *    Set the PWM to Motor.
 * \param[in]
 *    pwm - Pulse-Width Modulation.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderNew::setPWM(int pwm)
{
  cmdBuf[0] = _slot;
  cmdBuf[1] = CMD_SET_PWM;
  memcpy(cmdBuf+2,&pwm,2);
  sendCmd();
}

/**
 * \par Function
 *    getCurrentPosition
 * \par Description
 *    The current position of Motor.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
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

/**
 * \par Function
 *    getPID
 * \par Description
 *    Get PID from Motor.
 * \param[in]
 *    p - P means Proportion.
 * \param[in]
 *    i - I means Integration.
 * \param[in]
 *    d - D means Differentiation.
 * \param[in]
 *    s - S means slot of Motor.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
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

/**
 * \par Function
 *    getCurrentSpeed
 * \par Description
 *    The current speed of Motor's movement.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
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

/**
 * \par Function
 *    getPower
 * \par Description
 *    The current power of Motor's movement.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    Return the size of power.
 * \par Others
 *    None
 */
int8_t MeEncoderNew::getPower()
{
  Wire.beginTransmission(address); 
  Wire.write(_slot);       
  Wire.write(CMD_GET_POWER);              
  Wire.endTransmission(0);
  Wire.requestFrom(address,(uint8_t)1);   
  return (int8_t)Wire.read();
}

/**
 * \par Function
 *    sendCmd
 * \par Description
 *    Send command to Motor.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderNew::sendCmd()
{
  Wire.beginTransmission(address); 
  for(int i=0;i<18;i++)
    Wire.write(cmdBuf[i]);       
  Wire.endTransmission();
}

/**
 * \par Function
 *    getRatio
 * \par Description
 *    Get the ratio of Motor.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    Return the ratio of Motor.
 * \par Others
 *    None
 */
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

/**
 * \par Function
 *    setRatio
 * \par Description
 *    Set the ratio to Motor.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderNew::setRatio(float ratio)
{
  cmdBuf[0] = _slot;
  cmdBuf[1] = CMD_SET_RATIO;
  memcpy(cmdBuf+2,&ratio,4);
  sendCmd();
}

/**
 * \par Function
 *    getPulse
 * \par Description
 *    Get the pulse of Motor.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    Return the pulse of Motor.
 * \par Others
 *    None
 */
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

/**
 * \par Function
 *    setPulse
 * \par Description
 *    Set the pulse to Motor.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderNew::setPulse(int pulse)
{
  cmdBuf[0] = _slot;
  cmdBuf[1] = CMD_SET_PULSE;
  memcpy(cmdBuf+2,&pulse,2);
  sendCmd();
}

/**
 * \par Function
 *    setDevid
 * \par Description
 *    Set the devid to Motor.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeEncoderNew::setDevid(int devid)
{
  cmdBuf[0] = _slot;
  cmdBuf[1] = CMD_SET_DEVID;
  memcpy(cmdBuf+2,&devid,2);
  sendCmd();
}
