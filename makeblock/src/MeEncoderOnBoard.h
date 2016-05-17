/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MeEncoderOnBoard
 * \brief   Driver for Encoder module on MeAuriga and MeMegaPi.
 * @file    MeEncoderOnBoard.h
 * @author  MakeBlock
 * @version V1.0.2
 * @date    2016/05/17
 * @brief   Header for MeEncoderOnBoard.cpp module
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
 * This file is a drive for Encoder On MeAuriga and MeMegaPi.
 *
 * \par Method List:
 *
 *    1. void MeEncoderOnBoard::reset(uint8_t slot);
 *    2. uint8_t MeEncoderOnBoard::GetSlotNum(void);
 *    3. uint8_t MeEncoderOnBoard::GetIntNum(void);
 *    4. uint8_t MeEncoderOnBoard::GetPortA(void);
 *    5. uint8_t MeEncoderOnBoard::GetPortB(void);
 *    6. long MeEncoderOnBoard::GetPulsePos(void);
 *    7. void MeEncoderOnBoard::SetPulsePos(long pulse_pos);
 *    8. void MeEncoderOnBoard::PulsePosPlus(void);
 *    9. void MeEncoderOnBoard::PulsePosMinus(void);
 *    10. void MeEncoderOnBoard::SetCurrentSpeed(double speed);
 *    11. double MeEncoderOnBoard::GetCurrentSpeed(void);
 *    12. int MeEncoderOnBoard::GetPwm(void);
 *    13. void MeEncoderOnBoard::setMotorPwm(int pwm);
 *    14. void MeEncoderOnBoard::Update_speed(void);
 *    15. void MeEncoderOnBoard::update(void);
 *    16. long MeEncoderOnBoard::distanceToGo(void);
 *    17. void MeEncoderOnBoard::runSpeed(double speed);
 *    18. void MeEncoderOnBoard::setSpeed(double speed);
 *    19. void MeEncoderOnBoard::move(long distance,cb callback,int extId);
 *    20. void MeEncoderOnBoard::moveTo(long position,cb callback,int extId);
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/12/15     1.0.0            Build New
 * Mark Yan         2016/04/07     1.0.1            fix motor reset issue.
 * Mark Yan         2016/05/17     1.0.2            add some comments.
 * </pre>
 */

#ifndef ME_Encoder_OnBoard_H
#define ME_Encoder_OnBoard_H

#include <Arduino.h>
#include <stdbool.h>
#include <avr/interrupt.h>

typedef struct
{
  int pulse;
  
  // current output pwm
  int pwm;

  // target speed
  double targetSpeed;
  // current speed
  double currentSpeed;

  // target position
  long targetPos;
  // current position
  long currentPos;
  // position counter the pulse
  long pulse_pos;
 
  // pid
  float p;
  float i;
  float d;
  float s;

  //ratio
  float ratio;
}Me_Encoder_type;

typedef struct
{
  uint8_t start;
  float p;
  float i;
  float d;
  float s;
  float ratio;
  uint16_t pulse;
  uint8_t end;
}eeprom_store_type;

typedef struct
{
  uint8_t port_A;   //INT
  uint8_t port_B;   //DIR
  uint8_t port_PWM; //PWM
  uint8_t port_H1;
  uint8_t port_H2;
} Encoder_port_type;

extern Encoder_port_type encoder_Port[6];  // encoder_Port[0] is nonsense
typedef void (*cb)(int,int); 

/**
 * Class: MeEncoderOnBoard
 * \par Description
 * Declaration of Class MeEncoderOnBoard
 */
class MeEncoderOnBoard 
{
public:
/**
 * Alternate Constructor which can call your own function to map the Encoder motor to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
  MeEncoderOnBoard();

/**
 * Alternate Constructor which can call your own function to map the DC motor to arduino port
 * \param[in]
 *   port - megapi dc port from PORT_1 to PORT_12
 */
  MeEncoderOnBoard(uint8_t slot);

/**
 * \par Function
 *   GetSlotNum
 * \par Description
 *   This function used to get the Auriga/MegaPi slot number of current objects.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   uint8_t - The slot number of current objects \n
 * \par Others
 *   None
 */
  uint8_t GetSlotNum(void);

/**
 * \par Function
 *   GetIntNum
 * \par Description
 *   This function used to get the Auriga/MegaPi Interrupt number of current objects.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   uint8_t - The Interrupt number of current objects \n
 * \par Others
 *   None
 */
  uint8_t GetIntNum(void);
  
/**
 * \par Function
 *   GetPortA
 * \par Description
 *   This function used to get the GPIO number of current objects's dir port A.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   uint8_t - The GPIO number of current objects's dir port A \n
 * \par Others
 *   None
 */
  uint8_t GetPortA(void);

/**
 * \par Function
 *   GetPortB
 * \par Description
 *   This function used to get the GPIO number of current objects's dir port B.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   uint8_t - The GPIO number of current objects's dir port B \n
 * \par Others
 *   None
 */
  uint8_t GetPortB(void);

/**
 * \par Function
 *   GetPulsePos
 * \par Description
 *   This function used to get the current pos value(pulse counter).
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   long - current pos value \n
 * \par Others
 *   None
 */
  long GetPulsePos(void);

/**
 * \par Function
 *   SetPulsePos
 * \par Description
 *   This function used to Set the current pos value(pulse counter). Generally used for\n
 *   reset the distance calculation.
 * \param[in]
 *   pulse_pos - the value of pos value(pulse counter)
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void SetPulsePos(long pulse_pos);

/**
 * \par Function
 *   setMotorPwm
 * \par Description
 *   This function used to set current pwm setting.
 * \param[in]
 *   pwm - the pwm setting
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void setMotorPwm(int pwm);
  
/**
 * \par Function
 *   GetPwm
 * \par Description
 *   This function used to get the current pwm setting.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  int GetPwm(void);

/**
 * \par Function
 *   PulsePosPlus
 * \par Description
 *   This function used to increase the current pos value(pulse counter).
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void PulsePosPlus(void);
  
/**
 * \par Function
 *   PulsePosPlus
 * \par Description
 *   This function used to reduction the current pos value(pulse counter).
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void PulsePosMinus(void);

/**
 * \par Function
 *   SetCurrentSpeed
 * \par Description
 *   This function used to set the current speed(The unit is rpm).
 * \param[in]
 *   speed - the speed value(The unit is rpm).
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void SetCurrentSpeed(double speed);

/**
 * \par Function
 *   GetCurrentSpeed
 * \par Description
 *   This function used to get the current speed(The unit is rpm).
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
*   double - the speed value(The unit is rpm).
 * \par Others
 *   None
 */
  double GetCurrentSpeed(void);
  
/**
 * \par Function
 *   Update_speed
 * \par Description
 *   This function used to update current speed.
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void Update_speed(void);

/**
 * \par Function
 *   reset
 * \par Description
 *   Reset the encoder motor available PIN by Auriga/MegaPi slot.
 * \param[in]
 *   slot - MegaPi encoder port from SLOT1 to SLOT4(Auriga SLOT1 and SLOT2).
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void reset(uint8_t slot);

/**
 * \par Function
 *   update
 * \par Description
 *   This function called in main loop if you need run to the set position.\n
 *   It should be used with move and moveTo function;
 * \param[in]
 *   None
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void update();

/**
 * \par Function
 *    runSpeed
 * \par Description
 *    The speed of encode, and encoder motor will running with the setting speed.
 * \param[in]
 *    speed - the speed value(The unit is rpm).
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void runSpeed(double speed);

/**
 * \par Function
 *   setSpeed
 * \par Description
 *   This function used to set the current speed(The unit is rpm).\n
 *   Note: used with update function.
 * \param[in]
 *   speed - the speed value(The unit is rpm).
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
  void setSpeed(double speed);

/**
 * \par Function
 *    move
 * \par Description
 *    encoder motor moves to the relative positions.
 * \param[in]
 *    relative - The relative length to Stepper's movement.
 * \param[in]
 *    absolute - callback function when the target position has been reached.
 * \param[in]
 *    extId - It is used to indicate the ID of motor.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void move(long distance,cb callback,int extId=0);

/**
 * \par Function
 *    moveTo
 * \par Description
 *    encoder motor moves to the absolute position.
 * \param[in]
 *    absolute - The absolute length to Stepper's movement.
 * \param[in]
 *    absolute - callback function when the target position has been reached.
 * \param[in]
 *    extId - It is used to indicate the ID of motor.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void moveTo(long position,cb callback,int extId=0);
  
/**
 * \par Function
 *    distanceToGo
 * \par Description
 *    The distance that encoder should go.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    long - Return the length that encoder need run.
 * \par Others
 *    None
 */
  long distanceToGo();

private:
   volatile Me_Encoder_type encode_structure;
   uint8_t _mode;
   uint8_t _extId;
   uint8_t  _Port_A;
   uint8_t  _Port_B;
   uint8_t  _Port_PWM;
   uint8_t  _Port_H1;
   uint8_t  _Port_H2;
   uint8_t  _IntNum;
   uint8_t  _Slot;
   int16_t  _last_speed;
   long _Measurement_speed_time;
   long _Last_pulse_pos;
   double _targetSpeed;
   long _targetPosition;
   bool _moving;
   cb _callback;
};
#endif



