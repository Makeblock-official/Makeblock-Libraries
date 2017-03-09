/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class MeSmartServo
 * \brief   Driver for Me Smart Servo device.
 * @file    MeSmartServo.h
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2016/08/23
 * @brief   Header for for MeSmartServo.cpp module
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
 * This file is a drive for Smart Servo device, The Smart Servo inherited the 
 * MeSerial class from SoftwareSerial.
 *
 * \par Method List:
 *    1. uint8_t MeSmartServo::readByte(uint8_t *argv,int16_t idx);
 *    2. short MeSmartServo::readShort(uint8_t *argv,int16_t idx,boolean ignore_high);
 *    3. float MeSmartServo::readFloat(uint8_t *argv,int16_t idx);
 *    4. long MeSmartServo::readLong(uint8_t *argv,int idx);
 *    5. uint8_t MeSmartServo::sendByte(uint8_t val);
 *    6. uint8_t MeSmartServo::sendShort(int16_t val,boolean ignore_high);
 *    7. uint8_t MeSmartServo::sendFloat(float val);
 *    8. uint8_t MeSmartServo::sendLong(long val);
 *    9. boolean MeSmartServo::assignDevIdRequest(void);
 *    10. boolean MeSmartServo::moveTo(uint8_t dev_id,long angle_value,float speed);
 *    11. boolean MeSmartServo::move(uint8_t dev_id,long angle_value,float speed);
 *    12. boolean MeSmartServo::setZero(uint8_t dev_id);
 *    13. boolean MeSmartServo::setBreak(uint8_t dev_id, uint8_t breakStatus);
 *    14. boolean MeSmartServo::setRGBLed(uint8_t dev_id, uint8_t r_value, uint8_t g_value, uint8_t b_value);
 *    15. boolean MeSmartServo::handSharke(uint8_t dev_id);
 *    16. boolean MeSmartServo::setPwmMove(uint8_t dev_id, int16_t pwm_value);
 *    17. boolean MeSmartServo::setInitAngle(uint8_t dev_id,uint8_t mode,int16_t speed);
 *    18. long MeSmartServo::getAngleRequest(uint8_t devId);
 *    19. float MeSmartServo::getSpeedRequest(uint8_t devId);
 *    20. float MeSmartServo::getVoltageRequest(uint8_t devId);
 *    21. float MeSmartServo::getTempRequest(uint8_t devId);
 *    22. float MeSmartServo::getCurrentRequest(uint8_t devId);
 *    23. void MeSmartServo::assignDevIdResponse(void *arg);
 *    24. void MeSmartServo::processSysexMessage(void);
 *    25. void MeSmartServo::smartServoEventHandle(void);
 *    26. void MeSmartServo::errorCodeCheckResponse(void *arg);
 *    27. void MeSmartServo::smartServoCmdResponse(void *arg);
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2016/08/23     1.0.0            Build the new.
 * </pre>
 *
 */

#ifndef MeSmartServo_H
#define MeSmartServo_H

#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>
#include "MeConfig.h"
#include "MeSerial.h"

#ifdef ME_PORT_DEFINED
#include "MePort.h"
#endif /* ME_PORT_DEFINED */

#define ALL_DEVICE              0xff    // Broadcast command identifies
#define CUSTOM_TYPE             0x00    // 0x00 indicates no external module

#define CTL_ASSIGN_DEV_ID       0x10 // Assignment device ID
#define CTL_SYSTEM_RESET        0x11 // reset from host
#define CTL_READ_DEV_VERSION    0x12 // read the firmware version
#define CTL_SET_BAUD_RATE       0x13 // Set the bandrate
#define CTL_CMD_TEST            0x14 // Just for test
#define CTL_ERROR_CODE          0x15 // error code

#define SMART_SERVO              0x60
  /* Secondary command */
  #define SET_SERVO_PID                          0x10
  #define SET_SERVO_ABSOLUTE_POS                 0x11
  #define SET_SERVO_RELATIVE_POS                 0x12
  #define SET_SERVO_CONTINUOUS_ROTATION          0x13
  #define SET_SERVO_MOTION_COMPENSATION          0x14
  #define CLR_SERVO_MOTION_COMPENSATION          0x15
  #define SET_SERVO_BREAK                        0x16
  #define SET_SERVO_RGB_LED                      0x17
  #define SERVO_SHARKE_HAND                      0x18
  #define SET_SERVO_CMD_MODE                     0x19

  #define GET_SERVO_STATUS                       0x20
  #define GET_SERVO_PID                          0x21
  #define GET_SERVO_CUR_POS                      0x22
  #define GET_SERVO_SPEED                        0x23
  #define GET_SERVO_MOTION_COMPENSATION          0x24
  #define GET_SERVO_TEMPERATURE                  0x25
  #define GET_SERVO_ELECTRIC_CURRENT             0x26
  #define GET_SERVO_VOLTAGE                      0x27

  #define SET_SERVO_CURRENT_ANGLE_ZERO_DEGREES   0x30
  #define SET_SERVO_ABSOLUTE_ANGLE               0x31
  #define SET_SERVO_RELATIVE_ANGLE               0x32
  #define SET_SERVO_ABSOLUTE_ANGLE_LONG          0x33
  #define SET_SERVO_RELATIVE_ANGLE_LONG          0x34
  #define SET_SERVO_PWM_MOVE                     0x35
  #define GET_SERVO_CUR_ANGLE                    0x36
  #define SET_SERVO_INIT_ANGLE                   0x37

#define START_SYSEX             0xF0 // start a MIDI Sysex message
#define END_SYSEX               0xF7 // end a MIDI Sysex message

/* report error code */
#define PROCESS_SUC             0x0F
#define PROCESS_BUSY            0x10
#define PROCESS_ERROR           0x11
#define WRONG_TYPE_OF_SERVICE   0x12

#define DEFAULT_UART_BUF_SIZE      64

typedef struct{
  uint8_t dev_id;
  uint8_t srv_id;
  uint8_t value[DEFAULT_UART_BUF_SIZE - 2];
}sysex_message_type;

typedef struct{
  uint8_t service_id;
  void (*request_fun)(void *arg);
  void (*response_fun)(void *arg);
}Cmd_list_tab_type;

union sysex_message{
  uint8_t storedInputData[DEFAULT_UART_BUF_SIZE];
  sysex_message_type val;
};

union{
  uint8_t byteVal[8];
  double doubleVal;
}val8byte;

union{
  uint8_t byteVal[4];
  float floatVal;
  long longVal;
}val4byte;

union{
  uint8_t byteVal[2];
  short shortVal;
}val2byte;

union{
  uint8_t byteVal[1];
  uint8_t charVal;
}val1byte;

typedef struct
{
  long angleValue;
  float servoSpeed;
  float voltage;
  float temperature;
  float current;
}servo_device_type;

/**
 * Class: MeSmartServo
 * \par Description
 * Declaration of Class MeSmartServo.
 */
#ifndef ME_PORT_DEFINED
class MeSmartServo
#else /* !ME_PORT_DEFINED */
class MeSmartServo : public MeSerial
#endif /* !ME_PORT_DEFINED */
{
public:
#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the Me Smart Servo to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
  MeSmartServo();

/**
 * Alternate Constructor which can call your own function to map the Me Smart Servo to arduino port,
 * If the hardware serial was selected, it will used the hardware serial.
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
  MeSmartServo(uint8_t port);
#else // ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the Me Smart Servo to arduino port,
 * If the hardware serial was selected, it will used the hardware serial.
 * \param[in]
 *   receivePin - the rx pin of serial(arduino port)
 * \param[in]
 *   transmitPin - the tx pin of serial(arduino port)
 * \param[in]
 *   inverse_logic - Whether the Serial level need inv.
 */
  MeSmartServo(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic);
#endif // ME_PORT_DEFINED

/**
 * \par Function
 *   readByte
 * \par Description
 *   change 2byte 7bit read date to 1byte byte data.
 * \param[in]
 *   *argv - the buffer for input data.\n
 * \param[in]
 *   idx - the index used to identify the first address of read data.\n
 * \par Output
 *   return the converted data.
 * \return
 *   None
 * \par Others
 *   None
 */
  uint8_t readByte(uint8_t *argv,int16_t idx);

/**
 * \par Function
 *   readShort
 * \par Description
 *   change (2byte 7bit) or (3byte 7bit) read date to 2byte short data.
 * \param[in]
 *   *argv - the buffer for input data.\n
 * \param[in]
 *   idx - the index used to identify the first address of read data.\n
 * \param[in]
 *   ignore_high - is there have third byte high-level data.\n
 * \par Output
 *   return the converted data.
 * \return
 *   None
 * \par Others
 *   None
 */
  short readShort(uint8_t *argv,int16_t idx,boolean ignore_high);

/**
 * \par Function
 *   readFloat
 * \par Description
 *   change (5byte 7bit) read date to 4byte float data.
 * \param[in]
 *   *argv - the buffer for input data.\n
 * \param[in]
 *   idx - the index used to identify the first address of read data.\n
 * \par Output
 *   return the converted data.
 * \return
 *   None
 * \par Others
 *   None
 */
  float readFloat(uint8_t *argv,int16_t idx);

/**
 * \par Function
 *   readLong
 * \par Description
 *   change (5byte 7bit) read date to 4byte long data.
 * \param[in]
 *   *argv - the buffer for input data.\n
 * \param[in]
 *   idx - the index used to identify the first address of read data.\n
 * \par Output
 *   return the converted data.
 * \return
 *   None
 * \par Others
 *   None
 */
  long readLong(uint8_t *argv,int idx);

/**
 * \par Function
 *   sendByte
 * \par Description
 *   send (1byte 8bit) date to 2byte 7bit data.
 * \param[in]
 *   val - the byte data to be converted.\n
 * \par Output
 *   return the checksum data.
 * \return
 *   None
 * \par Others
 *   None
 */
  uint8_t sendByte(uint8_t val);

/**
 * \par Function
 *   sendShort
 * \par Description
 *   send (2byte short) date to 2byte or 3byte 7bit data.
 * \param[in]
 *   val - the short data to be converted.\n
 * \param[in]
 *   ignore_high - is there have third byte high-level data.\n
 * \par Output
 *   return the checksum data.
 * \return
 *   None
 * \par Others
 *   None
 */
  uint8_t sendShort(int16_t val,boolean ignore_high);

/**
 * \par Function
 *   sendFloat
 * \par Description
 *   send (4byte float) date to 5byte 7bit data.
 * \param[in]
 *   val - the short data to be converted.\n
 * \par Output
 *   return the checksum data.
 * \return
 *   None
 * \par Others
 *   None
 */
  uint8_t sendFloat(float val);

/**
 * \par Function
 *   sendLong
 * \par Description
 *   send (4byte long) date to 5byte 7bit data.
 * \param[in]
 *   val - the short data to be converted.\n
 * \par Output
 *   return the checksum data.
 * \return
 *   None
 * \par Others
 *   None
 */
  uint8_t sendLong(long val);

/**
 * \par Function
 *   assignDevIdRequest
 * \par Description
 *   distribution device ID number to the smart servo link.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   If the assignment is successful, return true. 
 * \par Others
 *   None
 */
  boolean assignDevIdRequest(void);

/**
 * \par Function
 *   moveTo
 * \par Description
 *   smart servo moves to the absolute angle.
 * \param[in]
 *    dev_id - the device id of servo that we want to move.
 * \param[in]
 *    angle_value - the absolute angle value we want move to.
 * \param[in]
 *    speed - move speed value(The unit is rpm).
 * \par Output
 *   None
 * \return
 *   If the assignment is successful, return true. 
 * \par Others
 *   None
 */
  boolean moveTo(uint8_t dev_id,long angle_value,float speed);

/**
 * \par Function
 *   move
 * \par Description
 *   smart servo moves to the relative angle.
 * \param[in]
 *    dev_id - the device id of servo that we want to move.
 * \param[in]
 *    angle_value - the relative angle value we want move to.
 * \param[in]
 *    speed - move speed value(The unit is rpm).
 * \par Output
 *   None
 * \return
 *   If the assignment is successful, return true. 
 * \par Others
 *   None
 */
  boolean move(uint8_t dev_id,long angle_value,float speed);

/**
 * \par Function
 *   setZero
 * \par Description
 *   set smart servo current angle zero postion.
 * \param[in]
 *    dev_id - the device id of servo that we want to initialization position.
 * \par Output
 *   None
 * \return
 *   If the assignment is successful, return true. 
 * \par Others
 *   None
 */
  boolean setZero(uint8_t dev_id);

/**
 * \par Function
 *   setBreak
 * \par Description
 *   set smart servo break status.
 * \param[in]
 *    dev_id - the device id of servo that we want to set.
 * \param[in]
 *    breakStatus - the break status of servo.
 * \par Output
 *   None
 * \return
 *   If the assignment is successful, return true. 
 * \par Others
 *   None
 */
  boolean setBreak(uint8_t dev_id, uint8_t breakStatus);

/**
 * \par Function
 *   setRGBLed
 * \par Description
 *   set the color of smart servo's RGB LED.
 * \param[in]
 *    dev_id - the device id of servo that we want to set.
 * \param[in]
 *    r_value - Red component.
 * \param[in]
 *    g_value - green component.
 * \param[in]
 *    B_value - Blue component.
 * \par Output
 *   None
 * \return
 *   If the assignment is successful, return true. 
 * \par Others
 *   None
 */
  boolean setRGBLed(uint8_t dev_id, uint8_t r_value, uint8_t g_value, uint8_t b_value);

/**
 * \par Function
 *   handSharke
 * \par Description
 *   This function is used MCU and servo handshake.
 * \param[in]
 *    dev_id - the device id of servo that we want to handsharke.
 * \par Output
 *   None
 * \return
 *   If the assignment is successful, return true. 
 * \par Others
 *   None
 */
  boolean handSharke(uint8_t dev_id);

/**
 * \par Function
 *   setPwmMove
 * \par Description
 *   This function is used to set the pwm motion of smart servo.
 * \param[in]
 *    dev_id - the device id of servo that we want to set.
 * \param[in]
 *    pwm_value - the pwm value we wan't set the servo motor.
 * \par Output
 *   None
 * \return
 *   If the assignment is successful, return true. 
 * \par Others
 *   None
 */
  boolean setPwmMove(uint8_t dev_id, int16_t pwm_value);

/**
 * \par Function
 *   setInitAngle
 * \par Description
 *   This function is used to move smart servo to its 0 degrees.
 * \param[in]
 *    dev_id - the device id of servo that we want to set.
 * \param[in]
 *    mode - the return mode,  0 is the quick return mode. 
 * \param[in]
 *    speed - the speed value return to init angle.
 * \par Output
 *   None
 * \return
 *   If the assignment is successful, return true. 
 * \par Others
 *   None
 */
  boolean setInitAngle(uint8_t dev_id,uint8_t = 0,int16_t = 40);

/**
 * \par Function
 *   getAngleRequest
 * \par Description
 *   This function used to get the smart servo's angle.
 * \param[in]
 *   devId - the device id of servo that we want to read its angle.
 * \par Output
 *   None
 * \return
 *   the angle of smart servo.
 * \par Others
 *   None
 */
  long getAngleRequest(uint8_t devId);

/**
 * \par Function
 *   getSpeedRequest
 * \par Description
 *   This function used to get the smart servo's speed.
 * \param[in]
 *   devId - the device id of servo that we want to read its speed.
 * \par Output
 *   None
 * \return
 *   the speed of smart servo.
 * \par Others
 *   None
 */
  float getSpeedRequest(uint8_t devId);

/**
 * \par Function
 *   getVoltageRequest
 * \par Description
 *   This function used to get the smart servo's voltage.
 * \param[in]
 *   devId - the device id of servo that we want to read its voltage.
 * \par Output
 *   None
 * \return
 *   the voltage of smart servo.
 * \par Others
 *   None
 */
  float getVoltageRequest(uint8_t devId);

/**
 * \par Function
 *   getTempRequest
 * \par Description
 *   This function used to get the smart servo's temperature.
 * \param[in]
 *   devId - the device id of servo that we want to read its temperature.
 * \par Output
 *   None
 * \return
 *   the temperature of smart servo.
 * \par Others
 *   None
 */
  float getTempRequest(uint8_t devId);

/**
 * \par Function
 *   getCurrentRequest
 * \par Description
 *   This function used to get the smart servo's current.
 * \param[in]
 *   devId - the device id of servo that we want to read its current.
 * \par Output
 *   None
 * \return
 *   the current of smart servo.
 * \par Others
 *   None
 */
  float getCurrentRequest(uint8_t devId);

/**
 * \par Function
 *   assignDevIdResponse
 * \par Description
 *   distribution device ID number return packet processing.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void assignDevIdResponse(void *arg);

 /**
 * \par Function
 *   errorCodeCheckResponse
 * \par Description
 *   This function is used to process the error response message of smart servo.
 * \param[in]
 *   *arg - address of input parameter.
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void errorCodeCheckResponse(void *arg);

/**
 * \par Function
 *   smartServoCmdResponse
 * \par Description
 *   This function is used to process the response message of smart servo.
 * \param[in]
 *   *arg - address of input parameter.
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void smartServoCmdResponse(void *arg);

/**
 * \par Function
 *   processSysexMessage
 * \par Description
 *   This function is used to process protocol messages of smart servo.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void processSysexMessage(void);

/**
 * \par Function
 *   smartServoEventHandle
 * \par Description
 *   This function is used to process protocol messages of smart servo.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void smartServoEventHandle(void);

private:
  union sysex_message sysex;
  volatile int16_t sysexBytesRead;
  volatile uint16_t resFlag;
  volatile servo_device_type servo_dev_list[8];
  volatile long cmdTimeOutValue;
  volatile boolean parsingSysex;
};
#endif