/*************************************************************************
* File Name          : Firmware_for_Auriga.ino
* Author             : myan
* Updated            : myan
* Version            : V09.01.016
* Date               : 21/06/2017
* Description        : Firmware for Makeblock Electronic modules with Scratch.  
* License            : CC-BY-SA 3.0
* Copyright (C) 2013 - 2016 Maker Works Technology Co., Ltd. All right reserved.
* http://www.makeblock.cc/
* History:
* <Author>         <Time>         <Version>        <Descr>
* Mark Yan         2016/03/12     09.01.001        Build the new.
* Mark Yan         2016/05/03     09.01.002        Added encoder and compass driver and fix some bugs.
* Mark Yan         2016/05/24     09.01.003        Fix issue MBLOCK-1 and MBLOCK-12(JIRA issue).
* Mark Yan         2016/05/30     09.01.004        Add speed calibration for balanced car mode.
* Mark Yan         2016/06/07     09.01.005        Remove the boot animation.
* Mark Yan         2016/06/08     09.01.006        Add 1s blink function.
* Mark Yan         2016/06/25     09.01.007        Fix issue MBLOCK-38(limit switch return value).
* Mark Yan         2016/07/06     09.01.008        Fix issue MBLOCK-61(ultrasonic distance limitations bug).
* Mark Yan         2016/07/27     09.01.009        Add position parameters for encoder motor,fix issue MBLOCK-77.
* Mark Yan         2016/08/01     09.01.010        Fix issue MBLOCK-109 MBLOCK-110(encoder motor exception handling negative).
* Mark Yan         2016/08/10     09.01.011        Fix issue MBLOCK-128(ext encoder motor led to reset).
* Mark Yan         2016/08/24     09.01.012        Fix issue MBLOCK-171(Stepper online execution slow), MBLOCK-189(on board encoder motor reset issue).
* Zzipeng          2016/12/15     09.01.013        Add Pm25Sensor
* Mark Yan         2016/03/07     09.01.014        Fix RGB lights issue, and add Smart servo support.
* Leo lu           2017/04/27     09.01.015        Fix issue of 3-axis gyroscope z output double.
* Mark Yan         2017/06/21     09.01.016        Fix smart servo assign device ID issue.
**************************************************************************/
#include <Arduino.h>
#include <avr/wdt.h>
#include <MeAuriga.h>
#include "MeEEPROM.h"
#include <Wire.h>
#include <SoftwareSerial.h>

//#define DEBUG_INFO
//#define DEBUG_INFO1

Servo servos[12];  
MeDCMotor dc;
MeTemperature ts;
MeRGBLed led;
MeUltrasonicSensor *us = NULL;  //PORT_10
Me7SegmentDisplay seg;
MePort generalDevice;
MeLEDMatrix ledMx;
MeInfraredReceiver *ir = NULL;  //PORT_8
MeGyro gyro_ext(0,0x68);  //external gryo sensor
MeGyro gyro(1,0x69);      //On Board external gryo sensor
MeCompass Compass;
MeJoystick joystick;
MeStepper steppers[4];
MeBuzzer buzzer;
MeHumiture humiture;
MeFlameSensor FlameSensor;
MeGasSensor GasSensor;
MeTouchSensor touchSensor;
Me4Button buttonSensor;
MeEncoderOnBoard Encoder_1(SLOT1);
MeEncoderOnBoard Encoder_2(SLOT2);
MeLineFollower line(PORT_9);
MeEncoderMotor encoders[2];
MePm25Sensor *pm25sensor = NULL;
MeSmartServo *mysmartservo = NULL;

typedef struct MeModule
{
  int16_t device;
  int16_t port;
  int16_t slot;
  int16_t pin;
  int16_t index;
  float values[3];
} MeModule;

union
{
  uint8_t byteVal[4];
  float floatVal;
  long longVal;
}val;

union
{
  uint8_t byteVal[8];
  double doubleVal;
}valDouble;

union
{
  uint8_t byteVal[2];
  int16_t shortVal;
}valShort;
MeModule modules[12];
#if defined(__AVR_ATmega32U4__) 
  int16_t analogs[12]={A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11};
#endif
#if defined(__AVR_ATmega328P__) or defined(__AVR_ATmega168__)
  int16_t analogs[8]={A0,A1,A2,A3,A4,A5,A6,A7};
#endif
#if defined(__AVR_ATmega1280__)|| defined(__AVR_ATmega2560__)
  int16_t analogs[16]={A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15};
#endif

int16_t len = 52;
int16_t servo_pins[12]={0,0,0,0,0,0,0,0,0,0,0,0};
//Just for Auriga
int16_t moveSpeed = 180;
int16_t turnSpeed = 180;
int16_t minSpeed = 45;
int16_t factor = 23;
int16_t distance=0;
int16_t randnum = 0;                                                                               
int16_t auriga_power = 0;
int16_t LineFollowFlag=0;
#define MOVE_STOP       0x00
#define MOVE_FORWARD    0x01
#define MOVE_BACKWARD   0x02

#define BLUETOOTH_MODE                       0x00
#define AUTOMATIC_OBSTACLE_AVOIDANCE_MODE    0x01
#define BALANCED_MODE                        0x02
#define IR_REMOTE_MODE                       0x03
#define LINE_FOLLOW_MODE                     0x04
#define MAX_MODE                             0x05

#define POWER_PORT                           A4
#define BUZZER_PORT                          45
#define RGBLED_PORT                          44

uint8_t command_index = 0;
uint8_t auriga_mode = BLUETOOTH_MODE;
uint8_t index = 0;
uint8_t dataLen;
uint8_t modulesLen=0;
uint8_t irRead = 0;
uint8_t prevc=0;
uint8_t keyPressed = KEY_NULL;
uint8_t serialRead;
uint8_t buffer[52];

double  lastTime = 0.0;
double  currentTime = 0.0;
double  CompAngleY, CompAngleX, GyroXangle;
double  LastCompAngleY, LastCompAngleX, LastGyroXangle;
double  last_turn_setpoint_filter = 0.0;
double  last_speed_setpoint_filter = 0.0;
double  last_speed_error_filter = 0.0;
double  speed_Integral_average = 0.0;
double  angle_speed = 0.0;
double  balance_car_speed_offsets = 0.0;

float angleServo = 90.0;
float dt;

long lasttime_angle = 0;
long lasttime_speed = 0;
long update_sensor = 0;
long blink_time = 0;
long rxruntime = 0;
long lasttime_receive_cmd = 0;
long last_Pulse_pos_encoder1 = 0;
long last_Pulse_pos_encoder2 = 0;

boolean isStart = false;
boolean isAvailable = false;
boolean leftflag;
boolean rightflag;
boolean start_flag = false;
boolean move_flag = false;
boolean boot_show_flag = true;
boolean blink_flag = false;

String mVersion = "09.01.016";

//////////////////////////////////////////////////////////////////////////////////////
float RELAX_ANGLE = -1;                    //Natural balance angle,should be adjustment according to your own car
#define PWM_MIN_OFFSET_BIAS   5

#define VERSION                0
#define ULTRASONIC_SENSOR      1
#define TEMPERATURE_SENSOR     2
#define LIGHT_SENSOR           3
#define POTENTIONMETER         4
#define JOYSTICK               5
#define GYRO                   6
#define SOUND_SENSOR           7
#define RGBLED                 8
#define SEVSEG                 9
#define MOTOR                  10
#define SERVO                  11
#define ENCODER                12
#define IR                     13
#define IRREMOTE               14
#define PIRMOTION              15
#define INFRARED               16
#define LINEFOLLOWER           17
#define IRREMOTECODE           18
#define SHUTTER                20
#define LIMITSWITCH            21
#define BUTTON                 22
#define HUMITURE               23
#define FLAMESENSOR            24
#define GASSENSOR              25
#define COMPASS                26
#define TEMPERATURE_SENSOR_1   27
#define DIGITAL                30
#define ANALOG                 31
#define PWM                    32
#define SERVO_PIN              33
#define TONE                   34
#define BUTTON_INNER           35
#define ULTRASONIC_ARDUINO     36
#define PULSEIN                37
#define STEPPER                40
#define LEDMATRIX              41
#define TIMER                  50
#define TOUCH_SENSOR           51
#define JOYSTICK_MOVE          52
#define COMMON_COMMONCMD       60
  //Secondary command
  #define SET_STARTER_MODE     0x10
  #define SET_AURIGA_MODE      0x11
  #define SET_MEGAPI_MODE      0x12
  #define GET_BATTERY_POWER    0x70
  #define GET_AURIGA_MODE      0x71
  #define GET_MEGAPI_MODE      0x72
#define ENCODER_BOARD          61
  //Read type
  #define ENCODER_BOARD_POS    0x01
  #define ENCODER_BOARD_SPEED  0x02

#define ENCODER_PID_MOTION     62
  //Secondary command
  #define ENCODER_BOARD_POS_MOTION         0x01
  #define ENCODER_BOARD_SPEED_MOTION       0x02
  #define ENCODER_BOARD_PWM_MOTION         0x03
  #define ENCODER_BOARD_SET_CUR_POS_ZERO   0x04
  #define ENCODER_BOARD_CAR_POS_MOTION     0x05
  
#define PM25SENSOR           63
  //Secondary command
  #define GET_PM1_0         0x01
  #define GET_PM2_5         0x02
  #define GET_PM10          0x03


#define SMARTSERVO           64
  //Secondary command
  #define SET_SMART_SERVO_BREAK           0x01
  #define SET_SMART_SERVO_RGB             0x02
  #define SET_SMART_SERVO_SHAKE_HANDS     0x03
  #define SET_SMART_SERVO_MOVE_TO         0x04
  #define SET_SMART_SERVO_MOVE            0x05
  #define SET_SMART_SERVO_PWM             0x06
  #define SET_SMART_SERVO_ZERO_DEGREES    0x07
  #define SET_SMART_SERVO_INIT_ANGLE      0x08

  #define GET_SMART_SERVO_SPEED           0x09
  #define GET_SMART_SERVO_TEMPERATURE     0x0a
  #define GET_SMART_SERVO_CURRENT         0x0b
  #define GET_SMART_SERVO_VOLTAGE         0x0c
  #define GET_SMART_SERVO_ANGLE           0x0d
  
#define GET 1
#define RUN 2
#define RESET 4
#define START 5

typedef struct
{
  double P, I, D;
  double Setpoint, Output, Integral,differential, last_error;
} PID;

PID  PID_angle, PID_speed, PID_turn;
/**
 * function list
 */
 void isr_process_encoder1(void);
 void isr_process_encoder2(void);
 void WriteBalancedDataToEEPROM(void);
 void WriteAurigaModeToEEPROM(void);
 void readEEPROM(void);
 void Forward(void);
 void Backward(void);
 void BackwardAndTurnLeft(void);
 void BackwardAndTurnRight(void);
 void TurnLeft(void);
 void TurnRight(void);
 void TurnLeft1(void);
 void TurnRight1(void);
 void Stop(void);
 void ChangeSpeed(int16_t spd);
 uint8_t readBuffer(int16_t index);
 void writeBuffer(int16_t index,uint8_t c);
 void writeHead(void);
 void writeEnd(void);
 void writeSerial(uint8_t c);
 void readSerial(void);
 void parseData(void);
 void callOK(void);
 void sendByte(uint8_t c);
 void sendString(String s);
 void sendFloat(float value);
 void sendLong(long value);
 void sendShort(int16_t value);
 void sendDouble(double value);
 int16_t readShort(int16_t idx);
 float readFloat(int16_t idx);
 long readLong(int16_t idx);
 char* readString(int16_t idx,int16_t len);
 uint8_t* readUint8(int16_t idx,int16_t len);
 void runModule(uint8_t device);
 int16_t searchServoPin(int16_t pin);
 float calculate_temp(int16_t In_temp);
 void readSensor(uint8_t device);
 float get_power(void);
 void PID_angle_compute(void) ;
 void PID_speed_compute(void);
 void reset(void);
 void parseGcode(char * cmd);
 void parseCmd(char * cmd);
 void balanced_model(void);
 void ultrCarProcess(void);
 void IrProcess();
 void line_model(void);
 boolean read_serial(void);
 void init_form_power(void);
 
/**
 * \par Function
 *    isr_process_encoder1
 * \par Description
 *    This function use to process the interrupt of encoder1 drvicer on board,
 *    used to calculate the number of pulses.
 * \param[in]
 *    None
 * \par Output
 *    The number of pulses on encoder1 driver
 * \return
 *    None
 * \par Others
 *    None
 */
void isr_process_encoder1(void)
{
  if(digitalRead(Encoder_1.getPortB()) == 0)
  {
    Encoder_1.pulsePosMinus();
  }
  else
  {
    Encoder_1.pulsePosPlus();;
  }
}

/**
 * \par Function
 *    isr_process_encoder2
 * \par Description
 *    This function use to process the interrupt of encoder2 drvicer on board,
 *    used to calculate the number of pulses.
 * \param[in]
 *    None
 * \par Output
 *    The number of pulses on encoder2 driver
 * \return
 *    None
 * \par Others
 *    None
 */
void isr_process_encoder2(void)
{
  if(digitalRead(Encoder_2.getPortB()) == 0)
  {
    Encoder_2.pulsePosMinus();
  }
  else
  {
    Encoder_2.pulsePosPlus();
  }
}

/**
 * \par Function
 *    WriteBalancedDataToEEPROM
 * \par Description
 *    This function use to write the balanced car configuration parameters to EEPROM.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void WriteBalancedDataToEEPROM(void)
{
  EEPROM.write(BALANCED_CAR_PARTITION_CHECK, EEPROM_IF_HAVEPID_CHECK1);
  EEPROM.write(BALANCED_CAR_PARTITION_CHECK + 1, EEPROM_IF_HAVEPID_CHECK2);
  EEPROM.write(BALANCED_CAR_START_ADDR, EEPROM_CHECK_START);

  EEPROM.put(BALANCED_CAR_NATURAL_BALANCE, RELAX_ANGLE);
  EEPROM.put(BALANCED_CAR_ANGLE_PID_ADDR, PID_angle.P);
  EEPROM.put(BALANCED_CAR_ANGLE_PID_ADDR+4, PID_angle.I);
  EEPROM.put(BALANCED_CAR_ANGLE_PID_ADDR+8, PID_angle.D);

  EEPROM.put(BALANCED_CAR_SPEED_PID_ADDR, PID_speed.P);
  EEPROM.put(BALANCED_CAR_SPEED_PID_ADDR+4, PID_speed.I);
  EEPROM.put(BALANCED_CAR_SPEED_PID_ADDR+8, PID_speed.D);

  EEPROM.put(BALANCED_CAR_DIR_PID_ADDR, PID_turn.P);
  EEPROM.write(BALANCED_CAR_END_ADDR, EEPROM_CHECK_END);

  EEPROM.write(AURIGA_MODE_START_ADDR, EEPROM_CHECK_START);
  EEPROM.write(AURIGA_MODE_CONFIGURE, auriga_mode);
  EEPROM.write(AURIGA_MODE_END_ADDR, EEPROM_CHECK_END);
}

/**
 * \par Function
 *    WriteAurigaModeToEEPROM
 * \par Description
 *    This function use to write the Auriga Mode configuration parameter to EEPROM.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void WriteAurigaModeToEEPROM(void)
{
  EEPROM.write(AURIGA_MODE_PARTITION_CHECK, EEPROM_IF_HAVEPID_CHECK1);
  EEPROM.write(AURIGA_MODE_PARTITION_CHECK + 1, EEPROM_IF_HAVEPID_CHECK2);
  EEPROM.write(AURIGA_MODE_START_ADDR, EEPROM_CHECK_START);
  EEPROM.write(AURIGA_MODE_CONFIGURE, auriga_mode);
  EEPROM.write(AURIGA_MODE_END_ADDR, EEPROM_CHECK_END);
}

/**
 * \par Function
 *    readEEPROM
 * \par Description
 *    This function use to read the configuration parameters from EEPROM.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void readEEPROM(void)
{
  if((EEPROM.read(BALANCED_CAR_PARTITION_CHECK) == EEPROM_IF_HAVEPID_CHECK1) && (EEPROM.read(BALANCED_CAR_PARTITION_CHECK + 1) == EEPROM_IF_HAVEPID_CHECK2))
  {
    if((EEPROM.read(BALANCED_CAR_START_ADDR)  == EEPROM_CHECK_START) && (EEPROM.read(BALANCED_CAR_END_ADDR)  == EEPROM_CHECK_END))
    {
      EEPROM.get(BALANCED_CAR_NATURAL_BALANCE, RELAX_ANGLE);
      EEPROM.get(BALANCED_CAR_ANGLE_PID_ADDR, PID_angle.P);
      EEPROM.get(BALANCED_CAR_ANGLE_PID_ADDR+4, PID_angle.I);
      EEPROM.get(BALANCED_CAR_ANGLE_PID_ADDR+8, PID_angle.D);

      EEPROM.get(BALANCED_CAR_SPEED_PID_ADDR, PID_speed.P);
      EEPROM.get(BALANCED_CAR_SPEED_PID_ADDR+4, PID_speed.I);
      EEPROM.get(BALANCED_CAR_SPEED_PID_ADDR+8, PID_speed.D);

      EEPROM.get(BALANCED_CAR_DIR_PID_ADDR, PID_turn.P);
#ifdef DEBUG_INFO
      Serial.println( "Read data from EEPROM:");
      Serial.print(RELAX_ANGLE);
      Serial.print( "  ");
      Serial.print(PID_angle.P);
      Serial.print( "  ");
      Serial.print(PID_angle.I);
      Serial.print( "  ");
      Serial.print(PID_angle.D);
      Serial.print( "  ");
      Serial.print(PID_speed.P);
      Serial.print( "  ");
      Serial.print(PID_speed.I);
      Serial.print( "  ");
      Serial.print(PID_speed.D);
      Serial.print( "  ");
      Serial.println(PID_turn.P);
#endif
    }
    else
    {
      Serial.println( "Data area damage on balanced car pid!" );
    }
  }
  else
  {
#ifdef DEBUG_INFO
    Serial.println( "First written Balanced data!" );
#endif
    WriteBalancedDataToEEPROM();
  }

  if((EEPROM.read(AURIGA_MODE_PARTITION_CHECK) == EEPROM_IF_HAVEPID_CHECK1) && (EEPROM.read(AURIGA_MODE_PARTITION_CHECK + 1) == EEPROM_IF_HAVEPID_CHECK2))
  {
    if((EEPROM.read(AURIGA_MODE_START_ADDR)  == EEPROM_CHECK_START) && (EEPROM.read(AURIGA_MODE_END_ADDR)  == EEPROM_CHECK_END))
    {
      EEPROM.get(AURIGA_MODE_CONFIGURE, auriga_mode);
#ifdef DEBUG_INFO
      Serial.print( "Read auriga_mode from EEPROM:");
      Serial.println(auriga_mode);
#endif
    }
    else
    {
      Serial.println( "Data area damage on auriga mode!" );
    }
  }
  else
  {
#ifdef DEBUG_INFO
    Serial.println( "First written auriga mode!" );
#endif
    WriteAurigaModeToEEPROM();
  }
}

/**
 * \par Function
 *    Forward
 * \par Description
 *    This function use to control the car kit go forward.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void Forward(void)
{
  Encoder_1.setMotorPwm(-moveSpeed);
  Encoder_2.setMotorPwm(moveSpeed);
}

/**
 * \par Function
 *    Backward
 * \par Description
 *    This function use to control the car kit go backward.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void Backward(void)
{
  Encoder_1.setMotorPwm(moveSpeed);
  Encoder_2.setMotorPwm(-moveSpeed);
}

/**
 * \par Function
 *    BackwardAndTurnLeft
 * \par Description
 *    This function use to control the car kit go backward and turn left.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void BackwardAndTurnLeft(void)
{
  Encoder_1.setMotorPwm(moveSpeed/4);
  Encoder_2.setMotorPwm(-moveSpeed);
}

/**
 * \par Function
 *    BackwardAndTurnRight
 * \par Description
 *    This function use to control the car kit go backward and turn right.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void BackwardAndTurnRight(void)
{
  Encoder_1.setMotorPwm(moveSpeed);
  Encoder_2.setMotorPwm(-moveSpeed/4);
}

/**
 * \par Function
 *    TurnLeft
 * \par Description
 *    This function use to control the car kit go backward and turn left.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void TurnLeft(void)
{
  Encoder_1.setMotorPwm(-moveSpeed);
  Encoder_2.setMotorPwm(moveSpeed/2);
}

/**
 * \par Function
 *    TurnRight
 * \par Description
 *    This function use to control the car kit go backward and turn right.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void TurnRight(void)
{
  Encoder_1.setMotorPwm(-moveSpeed/2);
  Encoder_2.setMotorPwm(moveSpeed);
}

/**
 * \par Function
 *    TurnLeft1
 * \par Description
 *    This function use to control the car kit go backward and turn left(fast).
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void TurnLeft1(void)
{
  Encoder_1.setMotorPwm(-moveSpeed);
  Encoder_2.setMotorPwm(-moveSpeed);
}

/**
 * \par Function
 *    TurnRight1
 * \par Description
 *    This function use to control the car kit go backward and turn right(fast).
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void TurnRight1(void)
{
  Encoder_1.setMotorPwm(moveSpeed);
  Encoder_2.setMotorPwm(moveSpeed);
}

/**
 * \par Function
 *    Stop
 * \par Description
 *    This function use to stop the car kit.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void Stop(void)
{
  Encoder_1.setMotorPwm(0);
  Encoder_2.setMotorPwm(0);
}

/**
 * \par Function
 *    ChangeSpeed
 * \par Description
 *    This function use to change the speed of car kit.
 * \param[in]
 *    spd - the speed of car kit(-255 ~ 255)
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void ChangeSpeed(int16_t spd)
{
  moveSpeed = spd;
}

/**
 * \par Function
 *    readBuffer
 * \par Description
 *    This function use to read the serial data from its buffer..
 * \param[in]
 *    index - The first address in the array
 * \par Output
 *    None
 * \return
 *    The data need to be read.
 * \par Others
 *    None
 */
uint8_t readBuffer(int16_t index)
{
  return buffer[index]; 
}

/**
 * \par Function
 *    writeBuffer
 * \par Description
 *    This function use to write the serial data to its buffer..
 * \param[in]
 *    index - The data's first address in the array
  * \param[in]
 *    c - The data need to be write.
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void writeBuffer(int16_t index,uint8_t c)
{
  buffer[index]=c;
}

/**
 * \par Function
 *    writeHead
 * \par Description
 *    This function use to write the head of transmission frame.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void writeHead(void)
{
  writeSerial(0xff);
  writeSerial(0x55);
}

/**
 * \par Function
 *    writeEnd
 * \par Description
 *    This function use to write the terminator of transmission frame.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void writeEnd(void)
{
  Serial.println(); 
}

/**
 * \par Function
 *    writeSerial
 * \par Description
 *    This function use to write the data to serial.
 * \param[in]
 *    c - The data need to be write.
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void writeSerial(uint8_t c)
{
  Serial.write(c);
}

/**
 * \par Function
 *    readSerial
 * \par Description
 *    This function use to read the data from serial, and fill the data
 *    to its buffer.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void readSerial(void)
{
  isAvailable = false;
  if(Serial.available() > 0)
  {
    isAvailable = true;
    serialRead = Serial.read();
  }
}

/**
 * \par Function
 *    parseData
 * \par Description
 *    This function use to process the data from the serial port,
 *    call the different treatment according to its action.
 *    ff 55 len idx action device port  slot  data a
 *    0  1  2   3   4      5      6     7     8
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void parseData(void)
{
  isStart = false;
  uint8_t idx = readBuffer(3);
  uint8_t action = readBuffer(4);
  uint8_t device = readBuffer(5);
  command_index = (uint8_t)idx;
  switch(action)
  {
    case GET:
      {
        if((device != ULTRASONIC_SENSOR) &&
           (device != HUMITURE) &&
           (device != ULTRASONIC_ARDUINO))
        {
          writeHead();
          writeSerial(idx);
        }
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
    case RESET:
      {
        //reset
        /* Off on-Board LED lights */
        buzzer.setpin(BUZZER_PORT);
        led.setColor(0,0,0,0);
        led.show();

        /* reset On-Board encoder driver */
        Encoder_1.setPulsePos(0);
        Encoder_2.setPulsePos(0);
        Encoder_1.moveTo(0,10);
        Encoder_2.moveTo(0,10);
        Encoder_1.setMotorPwm(0);
        Encoder_2.setMotorPwm(0);
        Encoder_1.setMotionMode(DIRECT_MODE);
        Encoder_2.setMotionMode(DIRECT_MODE);

        /* reset dc motor on driver port */
        dc.reset(PORT_1);
        dc.run(0);
        dc.reset(PORT_2);
        dc.run(0);
        dc.reset(PORT_3);
        dc.run(0);
        dc.reset(PORT_4);
        dc.run(0);

        /* reset ext encoder driver */
        encoders[0].runSpeed(0);
        encoders[1].runSpeed(0);

        /* reset stepper motor driver */
        steppers[0].setCurrentPosition(0);
        steppers[1].setCurrentPosition(0);
        steppers[2].setCurrentPosition(0);
        steppers[3].setCurrentPosition(0);
        steppers[0].moveTo(0);
        steppers[1].moveTo(0);
        steppers[2].moveTo(0);
        steppers[3].moveTo(0);

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

/**
 * \par Function
 *    callOK
 * \par Description
 *    Response for executable commands.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void callOK(void)
{
  writeSerial(0xff);
  writeSerial(0x55);
  writeEnd();
}

/**
 * \par Function
 *    sendByte
 * \par Description
 *    Send byte data
 * \param[in]
 *    c - the byte data need be sent.
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void sendByte(uint8_t c)
{
  writeSerial(1);
  writeSerial(c);
}

/**
 * \par Function
 *    sendString
 * \par Description
 *    Send string data
 * \param[in]
 *    s - the string data need be sent.
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
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

/**
 * \par Function
 *    sendFloat
 * \par Description
 *    Sned float data
 * \param[in]
 *    value - the float data need be sent.
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void sendFloat(float value)
{ 
  writeSerial(2);
  val.floatVal = value;
  writeSerial(val.byteVal[0]);
  writeSerial(val.byteVal[1]);
  writeSerial(val.byteVal[2]);
  writeSerial(val.byteVal[3]);
}

/**
 * \par Function
 *    sendLong
 * \par Description
 *    Sned long data
 * \param[in]
 *    value - the long data need be sent.
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void sendLong(long value)
{ 
  writeSerial(6);
  val.longVal = value;
  writeSerial(val.byteVal[0]);
  writeSerial(val.byteVal[1]);
  writeSerial(val.byteVal[2]);
  writeSerial(val.byteVal[3]);
}

/**
 * \par Function
 *    sendShort
 * \par Description
 *    Sned short data
 * \param[in]
 *    value - the short data need be sent.
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void sendShort(int16_t value)
{
  writeSerial(3);
  valShort.shortVal = value;
  writeSerial(valShort.byteVal[0]);
  writeSerial(valShort.byteVal[1]);
}

/**
 * \par Function
 *    sendDouble
 * \par Description
 *    Sned double data, same as float data on arduino.
 * \param[in]
 *    value - the double data need be sent.
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void sendDouble(double value)
{
  writeSerial(5);
  valDouble.doubleVal = value;
  writeSerial(valDouble.byteVal[0]);
  writeSerial(valDouble.byteVal[1]);
  writeSerial(valDouble.byteVal[2]);
  writeSerial(valDouble.byteVal[3]);
}

/**
 * \par Function
 *    readShort
 * \par Description
 *    read the short data.
 * \param[in]
 *    idx - The data's first address in the array.
 * \par Output
 *    None
 * \return
 *    the short data.
 * \par Others
 *    None
 */
int16_t readShort(int16_t idx)
{
  valShort.byteVal[0] = readBuffer(idx);
  valShort.byteVal[1] = readBuffer(idx+1);
  return valShort.shortVal; 
}

/**
 * \par Function
 *    readFloat
 * \par Description
 *    read the float data.
 * \param[in]
 *    idx - The data's first address in the array.
 * \par Output
 *    None
 * \return
 *    the float data.
 * \par Others
 *    None
 */
float readFloat(int16_t idx)
{
  val.byteVal[0] = readBuffer(idx);
  val.byteVal[1] = readBuffer(idx+1);
  val.byteVal[2] = readBuffer(idx+2);
  val.byteVal[3] = readBuffer(idx+3);
  return val.floatVal;
}

/**
 * \par Function
 *    readLong
 * \par Description
 *    read the long data.
 * \param[in]
 *    idx - The data's first address in the array.
 * \par Output
 *    None
 * \return
 *    the long data.
 * \par Others
 *    None
 */
long readLong(int16_t idx)
{
  val.byteVal[0] = readBuffer(idx);
  val.byteVal[1] = readBuffer(idx+1);
  val.byteVal[2] = readBuffer(idx+2);
  val.byteVal[3] = readBuffer(idx+3);
  return val.longVal;
}

char _receiveStr[20] = {};
uint8_t _receiveUint8[16] = {};

/**
 * \par Function
 *    readString
 * \par Description
 *    read the string data.
 * \param[in]
 *    idx - The string's first address in the array.
 * \param[in]
 *    len - The length of the string data.
 * \par Output
 *    None
 * \return
 *    the address of string data.
 * \par Others
 *    None
 */
char* readString(int16_t idx,int16_t len)
{
  for(int16_t i=0;i<len;i++)
  {
    _receiveStr[i]=readBuffer(idx+i);
  }
  _receiveStr[len] = '\0';
  return _receiveStr;
}

/**
 * \par Function
 *    readUint8
 * \par Description
 *    read the uint8 data.
 * \param[in]
 *    idx - The Uint8 data's first address in the array.
 * \param[in]
 *    len - The length of the uint8 data.
 * \par Output
 *    None
 * \return
 *    the address of uint8 data.
 * \par Others
 *    None
 */
uint8_t* readUint8(int16_t idx,int16_t len)
{
  for(int16_t i=0;i<len;i++)
  {
    if(i > 15)
    {
      break;
    }
    _receiveUint8[i] = readBuffer(idx+i);
  }
  return _receiveUint8;
}

/**
 * \par Function
 *    runModule
 * \par Description
 *    Processing execute commands.
 * \param[in]
 *    device - The definition of all execute commands.
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void runModule(uint8_t device)
{
  //0xff 0x55 0x6 0x0 0x1 0xa 0x9 0x0 0x0 0xa
  uint8_t port = readBuffer(6);
  uint8_t pin = port;
  switch(device)
  {
    case MOTOR:
      {
        int16_t speed = readShort(7);
        dc.reset(port);
        dc.run(speed);
      }
      break;
    case ENCODER_BOARD:
      if(port == 0)
      {
        uint8_t slot = readBuffer(7);
        int16_t speed_value = readShort(8);
        if(slot == SLOT_1)
        {
          Encoder_1.setTarPWM(speed_value);
        }
        else if(slot == SLOT_2)
        {
          Encoder_2.setTarPWM(speed_value);
        }
      }
      break;
    case JOYSTICK:
      {
        int16_t leftSpeed = readShort(6);
        Encoder_1.setTarPWM(leftSpeed);
        int16_t rightSpeed = readShort(8);
        Encoder_2.setTarPWM(rightSpeed);
      }
      break;
    case STEPPER:
      {
        int16_t maxSpeed = readShort(7);
        long distance = readLong(9);
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
        else if(port == PORT_3)
        {
         steppers[2] = MeStepper(PORT_3);
         steppers[2].moveTo(distance);
         steppers[2].setMaxSpeed(maxSpeed);
         steppers[2].setSpeed(maxSpeed);
        }
        else if(port == PORT_4)
        {
         steppers[3] = MeStepper(PORT_4);
         steppers[3].moveTo(distance);
         steppers[3].setMaxSpeed(maxSpeed);
         steppers[3].setSpeed(maxSpeed);
        }
      } 
      break;
    case ENCODER:
      {
        uint8_t slot = readBuffer(7);
        int16_t maxSpeed = readShort(8);
        float distance = readFloat(10);
        if(slot == SLOT_1)
        {
           encoders[0].move(distance,maxSpeed);
           delay(40);
        }
        else if(slot == SLOT_2)
        {
           encoders[1].move(distance,maxSpeed);
           delay(40);
        }
      }
      break;
    case RGBLED:
      {
        uint8_t slot = readBuffer(7);
        uint8_t idx = readBuffer(8);
        uint8_t r = readBuffer(9);
        uint8_t g = readBuffer(10);
        uint8_t b = readBuffer(11);
        if((port != 0) && ((led.getPort() != port) || (led.getSlot() != slot)))
        {
          led.reset(port,slot);
        }
        else if((port == 0) && ((led.getPort() != port) || (led.getSlot() != slot)))
        {
          led.setpin(RGBLED_PORT);
        }
        if(idx>0)
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
    case COMMON_COMMONCMD:
      {
        uint8_t subcmd = port;
        uint8_t cmd_data = readBuffer(7);
        if(SET_AURIGA_MODE == subcmd)
        {
          Stop();
          if((cmd_data == BALANCED_MODE) || 
             (cmd_data == AUTOMATIC_OBSTACLE_AVOIDANCE_MODE) || 
             (cmd_data == BLUETOOTH_MODE) ||
             (cmd_data == IR_REMOTE_MODE) ||
             (cmd_data == LINE_FOLLOW_MODE))
          {
            auriga_mode = cmd_data;
            if(EEPROM.read(AURIGA_MODE_CONFIGURE) != auriga_mode)
            {
              EEPROM.write(AURIGA_MODE_CONFIGURE, auriga_mode);
            }
          }
          else
          {
            auriga_mode = BLUETOOTH_MODE;
            if(EEPROM.read(AURIGA_MODE_CONFIGURE) != auriga_mode)
            {
              EEPROM.write(AURIGA_MODE_CONFIGURE, auriga_mode);
            }
          }
        }
      }
      break;
    case SERVO:
      {
        uint8_t slot = readBuffer(7);
        pin = slot==1?mePort[port].s1:mePort[port].s2;
        uint8_t v = readBuffer(8);
        Servo sv = servos[searchServoPin(pin)];
        if(v >= 0 && v <= 180)
        {
          if(!sv.attached())
          {
            sv.attach(pin);
          }
          sv.write(v);
        }
      }
      break;
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
    case LEDMATRIX:
      {
        if(ledMx.getPort()!=port)
        {
          ledMx.reset(port);
        }
        uint8_t action = readBuffer(7);
        if(action==1)
        {
          int8_t px = readBuffer(8);
          int8_t py = readBuffer(9);
          int8_t len = readBuffer(10);
          char *s = readString(11,len);
          ledMx.drawStr(px,py,s);
        }
        else if(action==2)
        {
          int8_t px = readBuffer(8);
          int8_t py = readBuffer(9);
          uint8_t *ss = readUint8(10,16);
          ledMx.drawBitmap(px,py,16,ss);
        }
        else if(action==3)
        {
          int8_t point = readBuffer(8);
          int8_t hours = readBuffer(9);
          int8_t minutes = readBuffer(10);
          ledMx.showClock(hours,minutes,point);
        }
        else if(action == 4)
        {
          ledMx.showNum(readFloat(8),3);
        }
      }
      break;
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
    case TONE:
      {
        pinMode(pin,OUTPUT);
        int16_t hz = readShort(7);
        int16_t ms = readShort(9);
        if(ms > 0)
        {
          buzzer.tone(pin, hz, ms); 
        }
        else
        {
          buzzer.noTone(pin); 
        }
      }
      break;
    case SERVO_PIN:
      {
        uint8_t v = readBuffer(7);
        if(v >= 0 && v <= 180)
        {
          Servo sv = servos[searchServoPin(pin)];
          if(!sv.attached())
          {
            sv.attach(pin);
          }
          sv.write(v);
        }
      }
      break;
    case TIMER:
      {
        lastTime = millis()/1000.0; 
      }
      break;
    case JOYSTICK_MOVE:
      {
        if(port == 0)
        {
           int16_t joy_x = readShort(7);
           int16_t joy_y = readShort(9);
           double joy_x_temp = -(double)joy_x * 0.18;//0.3
           double joy_y_temp = (double)joy_y * 0.25; //0.2
           PID_speed.Setpoint = joy_y_temp;
           PID_turn.Setpoint = joy_x_temp;
           if(abs(PID_speed.Setpoint) > 1)
           { 
             move_flag = true;
           }
        }
      }
      break;
    case ENCODER_PID_MOTION:
      {
        uint8_t subcmd = port;
        uint8_t slot_num = readBuffer(7);
        if(ENCODER_BOARD_POS_MOTION == subcmd)
        {
          long pos_temp = readLong(8);
          int16_t speed_temp = readShort(12);
          speed_temp = abs(speed_temp);
          if(slot_num == SLOT_1)
          {
            Encoder_1.move(pos_temp,(float)speed_temp);
          }
          else if(slot_num == SLOT_2)
          {
            Encoder_2.move(pos_temp,(float)speed_temp);
          }
        }
        else if(ENCODER_BOARD_SPEED_MOTION == subcmd)
        {
          int16_t speed_temp = readShort(8);  
          if(slot_num == SLOT_1)
          {
            Encoder_1.runSpeed((float)speed_temp);
          }
          else if(slot_num == SLOT_2)
          {
            Encoder_2.runSpeed((float)speed_temp);
          }
        }
        else if(ENCODER_BOARD_PWM_MOTION == subcmd)
        {
          int16_t speed_temp = readShort(8);  
          if(slot_num == SLOT_1)
          {
            Encoder_1.setTarPWM(speed_temp);
          }
          else if(slot_num == SLOT_2)
          {
            Encoder_2.setTarPWM(speed_temp);
          }
        }
        else if(ENCODER_BOARD_SET_CUR_POS_ZERO == subcmd)
        {
          if(slot_num == SLOT_1)
          {
            Encoder_1.setPulsePos(0);
          }
          else if(slot_num == SLOT_2)
          {
            Encoder_2.setPulsePos(0);
          }
        }
        else if(ENCODER_BOARD_CAR_POS_MOTION == subcmd)
        {
          long pos_temp = readLong(8);
          int16_t speed_temp = readShort(12);
          if(slot_num == 1)
          {
            Encoder_1.move(-pos_temp,(float)speed_temp);
            Encoder_2.move(pos_temp,(float)speed_temp);
          }
          else if(slot_num == 2)
          {
            Encoder_1.move(pos_temp,(float)speed_temp);
            Encoder_2.move(-pos_temp,(float)speed_temp);
          }
          else if(slot_num == 3)
          {
            Encoder_1.move(-pos_temp,(float)speed_temp);
            Encoder_2.move(-pos_temp,(float)speed_temp);
          }
          else if(slot_num == 4)
          {
            Encoder_1.move(pos_temp,(float)speed_temp);
            Encoder_2.move(pos_temp,(float)speed_temp);
          }
        }
      }
      break;
    case SMARTSERVO:
      {
        uint8_t subcmd = port;
        uint8_t port = readBuffer(7);
        uint8_t servoNum = readBuffer(8);
        if(mysmartservo == NULL)
        {
           mysmartservo = new MeSmartServo(port);
           mysmartservo->begin(115200);
           delay(5);
           mysmartservo->assignDevIdRequest();
           delay(50);
        }
        else if(mysmartservo->getPort() != port)
        {
           delete mysmartservo;
           mysmartservo = new MeSmartServo(port);
           mysmartservo->begin(115200);
           delay(5);
           mysmartservo->assignDevIdRequest();
           delay(50);
        }
        if(SET_SMART_SERVO_BREAK == subcmd)
        {
          uint8_t breakState = readBuffer(9);
          mysmartservo->setBreak(servoNum,breakState);
        }
        else if(SET_SMART_SERVO_RGB == subcmd)
        {
          uint8_t rValue = readBuffer(9);
          uint8_t gValue = readBuffer(10);
          uint8_t bValue = readBuffer(11);
          mysmartservo->setRGBLed(servoNum,rValue,gValue,bValue);
        }
        else if( SET_SMART_SERVO_SHAKE_HANDS == subcmd)
        {
          mysmartservo->handSharke(servoNum);
        }
        else if( SET_SMART_SERVO_MOVE_TO == subcmd)
        {
          long posValue = readLong(9);
          float speedValue = readFloat(13);
          mysmartservo->moveTo(servoNum,posValue,speedValue);
        }
        else if( SET_SMART_SERVO_MOVE == subcmd)
        {
          long posValue = readLong(9);
          float speedValue = readFloat(13);
          mysmartservo->move(servoNum,posValue,speedValue);
        }
        else if( SET_SMART_SERVO_PWM == subcmd)
        {
          int pwmValue = readShort(9);
          mysmartservo->setPwmMove(servoNum,pwmValue);
        }
        else if( SET_SMART_SERVO_ZERO_DEGREES == subcmd)
        {
          mysmartservo->setZero(servoNum);
        }
        else if( SET_SMART_SERVO_INIT_ANGLE == subcmd)
        {
          mysmartservo->setInitAngle(servoNum);
        }
      }
      break;
  }
}

/**
 * \par Function
 *    searchServoPin
 * \par Description
 *    Check if the pin has been allocated, if it is not allocated,
 *    then allocate it.
 * \param[in]
 *    pin - arduino gpio number
 * \par Output
 *    None
 * \return
 *    the servo number be assigned
 * \par Others
 *    None
 */
int16_t searchServoPin(int16_t pin)
{
  for(uint8_t i=0;i<12;i++)
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

const int16_t TEMPERATURENOMINAL     = 25;    //Nominl temperature depicted on the datasheet
const int16_t SERIESRESISTOR         = 10000; // Value of the series resistor
const int16_t BCOEFFICIENT           = 3380;  // Beta value for our thermistor(3350-3399)
const int16_t TERMISTORNOMINAL       = 10000; // Nominal temperature value for the thermistor

/**
 * \par Function
 *    calculate_temp
 * \par Description
 *    This function is used to convert the temperature.
 * \param[in]
 *    In_temp - Analog values from sensor.
 * \par Output
 *    None
 * \return
 *    the temperature in degrees Celsius
 * \par Others
 *    None
 */
float calculate_temp(int16_t In_temp)
{
  float media;
  float temperatura;
  media = (float)In_temp;
  // Convert the thermal stress value to resistance
  media = 1023.0 / media - 1;
  media = SERIESRESISTOR / media;
  //Calculate temperature using the Beta Factor equation

  temperatura = media / TERMISTORNOMINAL;              // (R/Ro)
  temperatura = log(temperatura); // ln(R/Ro)
  temperatura /= BCOEFFICIENT;                         // 1/B * ln(R/Ro)
  temperatura += 1.0 / (TEMPERATURENOMINAL + 273.15);  // + (1/To)
  temperatura = 1.0 / temperatura;                     // Invert the value
  temperatura -= 273.15;                               // Convert it to Celsius
  return temperatura;
}

/**
 * \par Function
 *    readSensor
 * \par Description
 *    This function is used to process query command.
 * \param[in]
 *    device - The definition of all query commands.
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void readSensor(uint8_t device)
{
  /**************************************************
      ff 55 len idx action device port slot data a
      0  1  2   3   4      5      6    7    8
  ***************************************************/
  float value=0.0;
  uint8_t port,slot,pin;
  port = readBuffer(6);
  pin = port;
  switch(device)
  {
    case ULTRASONIC_SENSOR:
      {
        if(us == NULL)
        {
          us = new MeUltrasonicSensor(port);
        }
        else if(us->getPort() != port)
        {
          delete us;
          us = new MeUltrasonicSensor(port);
        }
        value = (float)us->distanceCm();
        writeHead();
        writeSerial(command_index);
        sendFloat(value);
      }
      break;
    case  TEMPERATURE_SENSOR:
      {
        slot = readBuffer(7);
        if(ts.getPort() != port || ts.getSlot() != slot)
        {
          ts.reset(port,slot);
        }
        value = ts.temperature();
        sendFloat(value);
      }
      break;
    case  LIGHT_SENSOR:
    case  SOUND_SENSOR:
    case  POTENTIONMETER:
      {
        if(generalDevice.getPort() != port)
        {
          generalDevice.reset(port);
          pinMode(generalDevice.pin2(),INPUT);
        }
        value = generalDevice.aRead2();
        sendFloat(value);
      }
      break;
    case  TEMPERATURE_SENSOR_1:
      {
        if(generalDevice.getPort() != port)
        {
          generalDevice.reset(port);
          pinMode(generalDevice.pin2(),INPUT);
        }
        value = calculate_temp(generalDevice.aRead2());
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
    case  INFRARED:
      {
        if(ir == NULL)
        {
          ir = new MeInfraredReceiver(port);
          ir->begin();
        }
        else if(ir->getPort() != port)
        {
          delete ir;
          ir = new MeInfraredReceiver(port);
          ir->begin();
        }
        irRead = ir->getCode();
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
    case  PIRMOTION:
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
        if(generalDevice.getPort() != port)
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
        if(generalDevice.getPort() != port || generalDevice.getSlot() != slot)
        {
          generalDevice.reset(port,slot);
        }
        if(slot == 1)
        {
          pinMode(generalDevice.pin1(),INPUT_PULLUP);
          value = !generalDevice.dRead1();
        }
        else
        {
          pinMode(generalDevice.pin2(),INPUT_PULLUP);
          value = !generalDevice.dRead2();
        }
        sendFloat(value);  
      }
      break;
    case COMPASS:
      {
        if(Compass.getPort() != port)
        {
          Compass.reset(port);
          Compass.setpin(Compass.pin1(),Compass.pin2());
        }
        double CompassAngle;
        CompassAngle = Compass.getAngle();
        sendFloat((float)CompassAngle);
      }
      break;
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
        writeHead();
        writeSerial(command_index);
        sendByte(HumitureData);
      }
      break;
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
    case GASSENSOR:
      {
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
    case  GYRO:
      {
        uint8_t axis = readBuffer(7);
        if((port == 0) && (gyro_ext.getDevAddr() == 0x68))      //extern gyro
        {
          value = gyro_ext.getAngle(axis);
          sendFloat(value);
        }
        else if((port == 1) && (gyro.getDevAddr() == 0x69))
        {
          value = gyro.getAngle(axis);
          sendFloat(value);
        }
      }
      break;
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
        pin = analogs[pin];
        pinMode(pin,INPUT);
        sendFloat(analogRead(pin));
      }
      break;
    case  PULSEIN:
      {
        int16_t pw = readShort(7);
        pinMode(pin, INPUT);
        sendLong(pulseIn(pin,HIGH,pw));
      }
      break;
    case ULTRASONIC_ARDUINO:
      {
        uint8_t trig = readBuffer(6);
        uint8_t echo = readBuffer(7);
        long pw_data;
        float dis_data;
        pinMode(trig,OUTPUT);
        digitalWrite(trig,LOW);
        delayMicroseconds(2);
        digitalWrite(trig,HIGH);
        delayMicroseconds(10);
        digitalWrite(trig,LOW);
        pinMode(echo, INPUT);
        pw_data = pulseIn(echo,HIGH,30000);
        dis_data = pw_data/58.0;
        delay(5);
        writeHead();
        writeSerial(command_index);
        sendFloat(pw_data);
      }
      break;
    case TIMER:
      {
        sendFloat((float)currentTime);
      }
      break;
    case TOUCH_SENSOR:
      {
        if(touchSensor.getPort() != port)
        {
          touchSensor.reset(port);
        }
        sendByte(touchSensor.touched());
      }
      break;
    case BUTTON:
      {
        if(buttonSensor.getPort() != port)
        {
          buttonSensor.reset(port);
        }
        sendByte(keyPressed == readBuffer(7));
      }
      break;
    case ENCODER_BOARD:
      {
        if(port == 0)
        {
          slot = readBuffer(7);
          uint8_t read_type = readBuffer(8);
          if(slot == SLOT_1)
          {
            if(read_type == ENCODER_BOARD_POS)
            {
              sendLong(Encoder_1.getCurPos());
            }
            else if(read_type == ENCODER_BOARD_SPEED)
            {
              sendFloat(Encoder_1.getCurrentSpeed());
            }
          }
          else if(slot == SLOT_2)
          {
            if(read_type == ENCODER_BOARD_POS)
            {
              sendLong(Encoder_2.getCurPos());
            }
            else if(read_type == ENCODER_BOARD_SPEED)
            {
              sendFloat(Encoder_2.getCurrentSpeed());
            }
          }
        }
      }
      break;
    case COMMON_COMMONCMD:
      {
        uint8_t subcmd = port;
        if(GET_BATTERY_POWER == subcmd)
        {
          sendFloat(get_power());
        }
        else if(GET_AURIGA_MODE == subcmd)
        {
          sendByte(auriga_mode);
        }
      }
      break;
      case PM25SENSOR:
      {
        uint8_t secondorder = readBuffer(7);
        uint16_t temp = 0;
        static uint16_t tempreserve = 0;
        static uint8_t  dataflag = 1;
        if(pm25sensor == NULL)
        {
          pm25sensor = new MePm25Sensor(port);
          pm25sensor->begin(9600);
          pm25sensor->setOutputIntimePeriod(3);//3S
          delay(10);
          pm25sensor->setOutputIntimePeriod(3);//3S
          dataflag = 1;
        }
        else if(pm25sensor->getPort() != port)//initial
        {
          delete pm25sensor;
          pm25sensor = new MePm25Sensor(port);
          pm25sensor->begin(9600);
          pm25sensor->setOutputIntimePeriod(3);//3S
          delay(10);
          pm25sensor->setOutputIntimePeriod(3);//3S
          dataflag = 0;
        }
        else
        {
          dataflag = 1;
        }

        if(dataflag)
        {
          if(secondorder==GET_PM1_0)
          {
            temp = pm25sensor->readPm1_0Concentration();
          }
          else if(secondorder==GET_PM2_5)
          {
            temp = pm25sensor->readPm2_5Concentration();        
          }
          else if(secondorder==GET_PM10)
          {
            temp = pm25sensor->readPm10Concentration();
          }
          tempreserve = temp;
          sendShort(temp);
        }
        else
        {
          sendShort(tempreserve);
        }
      }
      break;
      case SMARTSERVO:
      {
        uint8_t subcmd = port;
        port = readBuffer(7);
        uint8_t servoNum = readBuffer(8);

        if(mysmartservo == NULL)
        {
           mysmartservo = new MeSmartServo(port);
           mysmartservo->begin(115200);
           delay(5);
           mysmartservo->assignDevIdRequest();
           delay(50);
        }
        else if(mysmartservo->getPort() != port)
        {
           delete mysmartservo;
           mysmartservo = new MeSmartServo(port);
           mysmartservo->begin(115200);
           delay(5);
           mysmartservo->assignDevIdRequest();
           delay(50);
        }
        if(GET_SMART_SERVO_SPEED == subcmd)
        {
          sendFloat(mysmartservo->getSpeedRequest(servoNum));
        }
        else if(GET_SMART_SERVO_TEMPERATURE == subcmd)
        {
          sendFloat(mysmartservo->getTempRequest(servoNum));
        }
        else if( GET_SMART_SERVO_CURRENT == subcmd)
        {
          sendFloat(mysmartservo->getCurrentRequest(servoNum));
        }
        else if(GET_SMART_SERVO_VOLTAGE == subcmd)
        {
          sendFloat(mysmartservo->getVoltageRequest(servoNum));
        }
        else if(GET_SMART_SERVO_ANGLE == subcmd)
        {
          sendLong(mysmartservo->getAngleRequest(servoNum));
        }
      }
      break;
  }//switch
}

/**
 * \par Function
 *    get_power
 * \par Description
 *    This function used to get the value of power supply
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    The power vlaue(unit is V)
 * \par Others
 *    None
 */
float get_power(void)
{
  float power;
  auriga_power = analogRead(POWER_PORT);
  power = (auriga_power/1024.0) * 15;
  return power;
}

/**
 * \par Function
 *    PID_angle_compute
 * \par Description
 *    The angle process for balance car
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void PID_angle_compute(void)   //PID
{
  CompAngleX = -gyro.getAngleX();
  double error = CompAngleX - PID_angle.Setpoint;
  PID_angle.Integral += error;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
  if(abs(CompAngleX - PID_angle.Setpoint) < 1)
  {
    PID_angle.Integral = 0;
  }

  PID_angle.differential = angle_speed;
  PID_angle.Output = PID_angle.P * error + PID_angle.I * PID_angle.Integral + PID_angle.D * PID_angle.differential;
  if(PID_angle.Output > 0)
  {
    PID_angle.Output = PID_angle.Output + PWM_MIN_OFFSET_BIAS;
  }
  else
  {
    PID_angle.Output = PID_angle.Output - PWM_MIN_OFFSET_BIAS;
  }

  double pwm_left = PID_angle.Output - PID_turn.Output;
  double pwm_right = -PID_angle.Output - PID_turn.Output;
  if(move_flag == true)
  { 
    balance_car_speed_offsets = 0;
  }
  else
  {
    balance_car_speed_offsets = 1.1 * (abs(Encoder_1.getCurrentSpeed()) - abs(Encoder_2.getCurrentSpeed()));
  }

  if(balance_car_speed_offsets > 0)
  {
    if(pwm_left > 0)
    {
      pwm_right = pwm_right - abs(balance_car_speed_offsets);
    }
    else
    {
      pwm_right = pwm_right + abs(balance_car_speed_offsets);
    }
  }
  else if(balance_car_speed_offsets < 0)
  {
    if(pwm_right > 0)
    {
      pwm_left = pwm_left - abs(balance_car_speed_offsets);
    }
    else
    {
      pwm_left = pwm_left + abs(balance_car_speed_offsets);
    }
  }

#ifdef DEBUG_INFO
  Serial.print("Relay: ");
  Serial.print(PID_angle.Setpoint);
  Serial.print(" AngX: ");
  Serial.print(CompAngleX);
  Serial.print(" Output: ");
  Serial.print(PID_angle.Output);
  Serial.print(" dif: ");
  Serial.println(PID_angle.differential);
#endif

  pwm_left = constrain(pwm_left, -255, 255);
  pwm_right = constrain(pwm_right, -255, 255);

  Encoder_1.setMotorPwm(pwm_left);
  Encoder_2.setMotorPwm(pwm_right);
}

/**
 * \par Function
 *    PID_speed_compute
 * \par Description
 *    The speed process for balance car
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void PID_speed_compute(void)
{
  double speed_now = (Encoder_2.getCurrentSpeed() - Encoder_1.getCurrentSpeed())/2;

  last_speed_setpoint_filter  = last_speed_setpoint_filter  * 0.8;
  last_speed_setpoint_filter  += PID_speed.Setpoint * 0.2;
 
  if((move_flag == true) && (abs(speed_now) < 8) && (PID_speed.Setpoint == 0))
  {
    move_flag = false;
    last_speed_setpoint_filter = 0;
    PID_speed.Integral = speed_Integral_average;
  }

  double error = speed_now - last_speed_setpoint_filter;
  PID_speed.Integral += error;

  if(move_flag == true) 
  {
    PID_speed.Integral = constrain(PID_speed.Integral , -1500, 1500);
    PID_speed.Output = PID_speed.P * error + PID_speed.I * PID_speed.Integral;
    PID_speed.Output = constrain(PID_speed.Output , -15.0, 15.0);
  }
  else
  { 
    PID_speed.Integral = constrain(PID_speed.Integral , -1500, 1500);
    PID_speed.Output = PID_speed.P * speed_now + PID_speed.I * PID_speed.Integral;
    PID_speed.Output = constrain(PID_speed.Output , -15.0, 15.0);
    speed_Integral_average = 0.8 * speed_Integral_average + 0.2 * PID_speed.Integral;
  }
  
#ifdef DEBUG_INFO
  Serial.print(speed_now);
  Serial.print(","); 
  Serial.print(PID_speed.Setpoint);
  Serial.print(",");      
  Serial.print(last_speed_error_filter);
  Serial.print(",");
  Serial.print(last_speed_setpoint_filter);
  Serial.print(",");
  Serial.print(PID_speed.Integral);
  Serial.print(",");
  Serial.println(PID_speed.Output);
#endif
  PID_angle.Setpoint =  RELAX_ANGLE - PID_speed.Output;
}

int16_t agx_start_count;

/**
 * \par Function
 *    reset
 * \par Description
 *    The exception process for balance car
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void reset(void)
{
  if((start_flag == false) && (abs(gyro.getAngleX()) < 5))
  {
    agx_start_count++;
  }
  if((start_flag == true) && (abs(gyro.getAngleX()) > 32))
  {
    agx_start_count = 0;
    Encoder_1.setMotorPwm(0);
    Encoder_2.setMotorPwm(0);
    PID_speed.Integral = 0;
    PID_angle.Setpoint = RELAX_ANGLE;
    PID_speed.Setpoint = 0;
    PID_turn.Setpoint = 0;
    Encoder_1.setPulsePos(0);
    Encoder_2.setPulsePos(0);
    PID_speed.Integral = 0;
    start_flag = false;
    last_speed_setpoint_filter = 0.0;
    last_turn_setpoint_filter = 0.0;
#ifdef DEBUG_INFO
    Serial.println("> 32");
#endif
  }
  else if(agx_start_count > 20)
  {
    agx_start_count = 0;
    PID_speed.Integral = 0;
    Encoder_1.setMotorPwm(0);
    Encoder_2.setMotorPwm(0);
    PID_angle.Setpoint = RELAX_ANGLE;
    Encoder_1.setPulsePos(0);
    Encoder_2.setPulsePos(0);
    lasttime_speed = lasttime_angle = millis();
    start_flag = true;
#ifdef DEBUG_INFO
    Serial.println("< 5");
#endif
  }
}

/**
 * \par Function
 *    parseGcode
 * \par Description
 *    The function used to configure parameters for balance car.
 * \param[in]
 *    cmd - Gcode command
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void parseGcode(char * cmd)
{
  char * tmp;
  char * str;
  char g_code_cmd;
  float p_value = 0;
  float i_value = 0;
  float d_value = 0;
  float relax_angle = 0;
  str = strtok_r(cmd, " ", &tmp);
  g_code_cmd = str[0];
  while(str!=NULL)
  {
    str = strtok_r(0, " ", &tmp);
    if((str[0]=='P') || (str[0]=='p')){
      p_value = atof(str+1);
    }else if((str[0]=='I') || (str[0]=='i')){
      i_value = atof(str+1);
    }else if((str[0]=='D') || (str[0]=='d')){
      d_value = atof(str+1);
    }
    else if((str[0]=='Z') || (str[0]=='z')){
      relax_angle  = atof(str+1);
    }
    else if((str[0]=='M') || (str[0]=='m')){
      auriga_mode  = atof(str+1);
    }
  }
//#ifdef DEBUG_INFO
  Serial.print("PID: ");
  Serial.print(p_value);
  Serial.print(", ");
  Serial.print(i_value);
  Serial.print(",  ");
  Serial.println(d_value);
//#endif
  if(g_code_cmd == '1')
  {
    PID_angle.P = p_value;
    PID_angle.I = i_value;
    PID_angle.D = d_value;
    EEPROM.put(BALANCED_CAR_ANGLE_PID_ADDR, PID_angle.P);
    EEPROM.put(BALANCED_CAR_ANGLE_PID_ADDR+4, PID_angle.I);
    EEPROM.put(BALANCED_CAR_ANGLE_PID_ADDR+8, PID_angle.D);
  }
  else if(g_code_cmd == '2')
  {
    PID_speed.P = p_value;
    PID_speed.I = i_value;
    PID_speed.D = d_value;
    EEPROM.put(BALANCED_CAR_SPEED_PID_ADDR, PID_speed.P);
    EEPROM.put(BALANCED_CAR_SPEED_PID_ADDR+4, PID_speed.I);
    EEPROM.put(BALANCED_CAR_SPEED_PID_ADDR+8, PID_speed.D);
  }
  else if(g_code_cmd == '3')
  {
    RELAX_ANGLE = relax_angle;
    EEPROM.put(BALANCED_CAR_NATURAL_BALANCE, relax_angle);
  }
  else if(g_code_cmd == '4')
  {
    if(EEPROM.read(AURIGA_MODE_CONFIGURE) != auriga_mode)
    {
      EEPROM.write(AURIGA_MODE_CONFIGURE, auriga_mode);
    }
    Serial.print("auriga_mode: ");
    Serial.println(auriga_mode);
  }
}

/**
 * \par Function
 *    parseCmd
 * \par Description
 *    The function used to parse Gcode command.
 * \param[in]
 *    cmd - Gcode command
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void parseCmd(char * cmd)
{
  if((cmd[0]=='g') || (cmd[0]=='G'))
  { 
    // gcode
    parseGcode(cmd+1);
  }
}

/**
 * \par Function
 *    balanced_model
 * \par Description
 *    The main function for balanced car model
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void balanced_model(void)
{
  reset();
  if(start_flag == true)
  {
    if((millis() - lasttime_angle) > 10)
    {
      PID_angle_compute();
      lasttime_angle = millis();
    }    
    if((millis() - lasttime_speed) > 100)
    {
      PID_speed_compute();
      last_turn_setpoint_filter  = last_turn_setpoint_filter * 0.8;
      last_turn_setpoint_filter  += PID_turn.Setpoint * 0.2;
      PID_turn.Output = last_turn_setpoint_filter;
      lasttime_speed = millis();
    }
  }
  else
  {
    Encoder_1.setMotorPwm(0);
    Encoder_2.setMotorPwm(0);
  } 
}

/**
 * \par Function
 *    ultrCarProcess
 * \par Description
 *    The main function for ultrasonic automatic obstacle avoidance
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void ultrCarProcess(void)
{
  if(us == NULL)
  {
    us = new MeUltrasonicSensor(PORT_10);
  }
  moveSpeed = 150;
  if(us != NULL)
  {
    distance = us->distanceCm();
  }
  else
  {
    return;
  }

  if((distance > 20) && (distance < 40))
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
  else if((distance < 20) && (distance > 0))
  {
    randnum=random(300);
    if(randnum > 190)
    {
      BackwardAndTurnLeft();
      for(int16_t i=0;i<300;i++)
      {
        wdt_reset();
        if(read_serial() == true)
        {
          break;
        }
        else
        {
          delay(2);
        }
      }
    }
    else
    {
      BackwardAndTurnRight();
      for(int16_t i=0;i<300;i++)
      {
        wdt_reset();
        if(read_serial() == true)
        {
          break;
        }
        else
        {
          delay(2);
        }
      }
    }
  }
  else
  {
    leftflag=false;
    rightflag=false;
    Forward();
  }
}

/**
 * \par Function
 *    IrProcess
 * \par Description
 *    The main function for IR control mode
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void IrProcess()
{
  if(ir == NULL)
  {
    ir = new MeInfraredReceiver(PORT_8);
    ir->begin();
  }
  ir->loop();
  irRead =  ir->getCode();
  if((irRead != IR_BUTTON_TEST) && (auriga_mode != IR_REMOTE_MODE))
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
    case IR_BUTTON_0:
      Stop();
      break;
    case IR_BUTTON_TEST:
      Stop();
      while( ir->buttonState() != 0)
      {
        ir->loop();
      }
      auriga_mode = auriga_mode + 1;
      if(auriga_mode == MAX_MODE)
      { 
        auriga_mode = BLUETOOTH_MODE;
      }
      break;
    default:
      Stop();
      break;
  }
}

/**
 * \par Function
 *    line_model
 * \par Description
 *    The main function for Patrol Line navigation mode
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void line_model(void)
{
  uint8_t val;
  val = line.readSensors();
  moveSpeed=120;
  switch (val)
  {
    case S1_IN_S2_IN:
      Forward();
      LineFollowFlag=10;
      break;

    case S1_IN_S2_OUT:
       Forward();
      if (LineFollowFlag>1) LineFollowFlag--;
      break;

    case S1_OUT_S2_IN:
      Forward();
      if (LineFollowFlag<20) LineFollowFlag++;
      break;

    case S1_OUT_S2_OUT:
      if(LineFollowFlag==10) Backward();
      if(LineFollowFlag<10) TurnLeft1();
      if(LineFollowFlag>10) TurnRight1();
      break;
  }
}

uint8_t buf[64];
uint8_t bufindex;

/**
 * \par Function
 *    read_serial
 * \par Description
 *    The function used to process serial data.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    Is there a valid command 
 * \par Others
 *    None
 */
boolean read_serial(void)
{
  boolean result = false;
  readSerial();
  if(isAvailable)
  {
    uint8_t c = serialRead & 0xff;
    result = true;
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
        else if(index > 2)
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
    return result;
  }
}

/**
 * \par Function
 *    init_form_power
 * \par Description
 *    Boot animation for auriga car kit
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void init_form_power(void)
{
  uint8_t R_Bright = 0;
  for(uint8_t i = 0;i < 40;i++)
  {
    led.setColor(0,R_Bright,R_Bright,R_Bright);
    led.show();
    R_Bright += 1;
    delay(12);
    wdt_reset();
  }

  R_Bright = 40;
  for(uint8_t i = 0;i < 40;i++)
  {
    led.setColor(0,R_Bright,R_Bright,R_Bright);
    led.show();
    R_Bright -= 1;
    delay(12);
    wdt_reset();
  }

  buzzer.tone(988, 125);   //NOTE_B5
  led.setColor(0,20,0,0);
  led.show();
  delay(200);
  wdt_reset();

  led.setColor(0,20,5,0);
  led.show();
  delay(200);

  led.setColor(0,15,15,0);
  led.show();
  delay(200);

  led.setColor(0,0,20,0);
  led.show();
  delay(200);
  wdt_reset();

  led.setColor(0,0,0,20);
  led.show();
  delay(200);

  led.setColor(0,10,0,20);
  led.show();
  delay(200);

  led.setColor(0,20,0,20);
  led.show();
  delay(200);
  wdt_reset();

  led.setColor(0,0,0,0);
  buzzer.tone(1976, 125);  //NOTE_B6
  led.setColor(12,20,10,20);
  led.setColor(1,20,10,20);
  led.setColor(2,20,10,20);
  led.show();
  delay(375);
  wdt_reset();
  buzzer.tone(1976, 125);  //NOTE_B6
  led.setColor(3,20,20,0);
  led.setColor(4,20,20,0);
  led.setColor(5,20,20,0);
  led.show();
  delay(375);
  wdt_reset();
  buzzer.tone(1976, 125);  //NOTE_B69
  led.setColor(6,0,10,20);
  led.setColor(7,0,10,20);
  led.setColor(8,0,10,20);
  led.show();
  delay(500);
  wdt_reset();
  buzzer.tone(1976, 125);  //NOTE_B69
  led.setColor(9,10,0,0);
  led.setColor(10,10,0,0);
  led.setColor(11,10,0,0);
  led.show();
  delay(500);
  wdt_reset();

  led.setColor(0,0,0,0);
  for(uint8_t i=0;i<4;i++)
  {
    led.setColor(12,20,10,20);
    led.setColor(1,20,10,20);
    led.setColor(2,20,10,20);
    
    led.setColor(3,20,20,0);
    led.setColor(4,20,20,0);
    led.setColor(5,20,20,0);

    led.setColor(6,0,10,20);
    led.setColor(7,0,10,20);
    led.setColor(8,0,10,20);

    led.setColor(9,10,0,0);
    led.setColor(10,10,0,0);
    led.setColor(11,10,0,0);
    led.show();
    delay(2);
    buzzer.tone(2349, 250);  //NOTE_D7
    led.setColor(0,0,0,0);
    led.show();
    delay(100);
    wdt_reset();
  }

  buzzer.tone(262, 250);   //NOTE_D5
  buzzer.tone(294, 250);   //NOTE_E5
  buzzer.tone(330, 250);   //NOTE_C5
  led.setColor(0,0,0,0);
  led.show();
}

/**
 * \par Function
 *    setup
 * \par Description
 *    Initialization function for arduino
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void setup()
{
  delay(5);
  Serial.begin(115200);
  delay(5);
  attachInterrupt(Encoder_1.getIntNum(), isr_process_encoder1, RISING);
  attachInterrupt(Encoder_2.getIntNum(), isr_process_encoder2, RISING);
  led.setpin(RGBLED_PORT);
  buzzer.setpin(BUZZER_PORT);
  led.setColor(0,0,0,0);
  led.show();
  buzzer.tone(1000,100); 
  buzzer.noTone();
  Serial.print("Version: ");
  Serial.println(mVersion);
//  lasttime_receive_cmd =  millis();
//  while(millis() - lasttime_receive_cmd < 300)
//  {
//    if(Serial.available() > 0)
//    {
//      char c = Serial.read();
//      if((c=='\n') || (c=='#'))
//      {
//        boot_show_flag = false;
//        break;
//      }
//    }
//  }

  // enable the watchdog
  wdt_enable(WDTO_2S);
  delay(5);
  gyro_ext.begin();
  delay(5);
  wdt_reset();
  gyro.begin();
  delay(5);
  pinMode(13,OUTPUT);
  encoders[0] = MeEncoderMotor(SLOT_1);
  encoders[1] = MeEncoderMotor(SLOT_2);
  encoders[0].begin();
  encoders[1].begin();
  wdt_reset();
//  if(boot_show_flag == true)
//  {
//    init_form_power();
//  }
  wdt_reset();
  encoders[0].runSpeed(0);
  encoders[1].runSpeed(0);

  //Set Pwm 8KHz
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);

  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);

  Encoder_1.setPulse(9);
  Encoder_2.setPulse(9);
  Encoder_1.setRatio(39.267);
  Encoder_2.setRatio(39.267);
  Encoder_1.setPosPid(1.8,0,1.2);
  Encoder_2.setPosPid(1.8,0,1.2);
  Encoder_1.setSpeedPid(0.18,0,0);
  Encoder_2.setSpeedPid(0.18,0,0);
  Encoder_1.setMotionMode(DIRECT_MODE);
  Encoder_2.setMotionMode(DIRECT_MODE);

  leftflag=false;
  rightflag=false;
  PID_angle.Setpoint = RELAX_ANGLE;
  PID_angle.P = 17;          //17;
  PID_angle.I = 0;           //0;
  PID_angle.D = -0.2;        //-0.2  PID_speed.Setpoint = 0;
  PID_speed.P = -0.1;        // -0.1
  PID_speed.I = -0.008;      // -0.008
  readEEPROM();
//  auriga_mode = BALANCED_MODE;
  update_sensor = lasttime_speed = lasttime_angle = millis();
  blink_time = millis();
  rxruntime = millis();
}

/**
 * \par Function
 *    loop
 * \par Description
 *    main function for arduino
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void loop()
{
  currentTime = millis()/1000.0-lastTime;
  keyPressed = buttonSensor.pressed();

  if(millis() - blink_time > 1000)
  {
    blink_time = millis();
    blink_flag = !blink_flag;
    digitalWrite(13,blink_flag);
  }

  wdt_reset();
  if(ir != NULL)
  {
    IrProcess();
  }
  steppers[0].runSpeedToPosition();
  steppers[1].runSpeedToPosition();
  steppers[2].runSpeedToPosition();
  steppers[3].runSpeedToPosition();
  get_power();
  Encoder_1.loop();
  Encoder_2.loop();

  if(millis() - rxruntime>500)
  {
    rxruntime = millis();
    if(pm25sensor != NULL)
    {
      pm25sensor->rxloop();
    }
  }
//  while(Serial.available() > 0)
//  {
//    char c = Serial.read();
//    Serial.write(c);
//    buf[bufindex++]=c;
//    if((c=='\n') || (c=='#'))
//    {
//      parseCmd(buf);
//      memset(buf,0,64);
//      bufindex = 0;
//    }
//  }

  readSerial();
  while(isAvailable)
  {
    unsigned char c = serialRead & 0xff;
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
        else if(index > 2)
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
    readSerial();
  }
  if(Compass.getPort() != 0)
  {
    Compass.getAngle();
  }
  angle_speed = gyro.getGyroY();

  if(auriga_mode == BLUETOOTH_MODE)
  {
    if(millis() - update_sensor > 10)
    {
      update_sensor = millis();
      gyro.fast_update();
      gyro_ext.update();
    }
  }
  else if(auriga_mode == AUTOMATIC_OBSTACLE_AVOIDANCE_MODE)
  { 
    Encoder_1.setMotionMode(DIRECT_MODE);
    Encoder_2.setMotionMode(DIRECT_MODE);
    ultrCarProcess();
  }
  else if(auriga_mode == BALANCED_MODE)
  { 
    gyro.fast_update();
    Encoder_1.setMotionMode(DIRECT_MODE);
    Encoder_2.setMotionMode(DIRECT_MODE);
    balanced_model();
  }
  else if(auriga_mode == LINE_FOLLOW_MODE)
  { 
    Encoder_1.setMotionMode(DIRECT_MODE);
    Encoder_2.setMotionMode(DIRECT_MODE);
    line_model();
  }
}

