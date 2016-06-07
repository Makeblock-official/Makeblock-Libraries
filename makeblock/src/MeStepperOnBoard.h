/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class MeStepperOnBoard
 * \brief   Driver for Me Stepper on MegaPi.
 * @file    MeStepperOnBoard.h
 * @author  MakeBlock
 * @version V1.0.1
 * @date    2016/05/06
 * @brief   Header for MeStepperOnBoard.cpp module
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
 * This file is a drive for Me Stepper device.
 *
 * \par Method List:
 *
 *    1. void MeStepperOnBoard::setMicroStep(int8_t value);
 *    2. void MeStepperOnBoard::setpin(int slot);
 *    3. void MeStepperOnBoard::moveTo(long absolute);
 *    4. void MeStepperOnBoard::moveTo(long absolute, cb callback, int extId);
 *    5. void MeStepperOnBoard::move(long relative);
 *    6. void MeStepperOnBoard::move(long relative, cb callback, int extId);
 *    7. boolean MeStepperOnBoard::run(void);
 *    8. boolean MeStepperOnBoard::runSpeed(void);
 *    9. void MeStepperOnBoard::setMaxSpeed(float speed);
 *    10. void MeStepperOnBoard::setAcceleration(float acceleration);
 *    11. void MeStepperOnBoard::setSpeed(float speed);
 *    12. float MeStepperOnBoard::speed(void);
 *    13. long MeStepperOnBoard::distanceToGo(void);
 *    14. long MeStepperOnBoard::targetPosition(void);
 *    15. long MeStepperOnBoard::currentPosition(void);  
 *    16. void MeStepperOnBoard::setCurrentPosition(long position);  
 *    17. void MeStepperOnBoard::runToPosition(void);
 *    18. boolean MeStepperOnBoard::runSpeedToPosition(void);
 *    19. void MeStepperOnBoard::runToNewPosition(long position);
 *    20. void MeStepperOnBoard::disableOutputs(void);
 *    21. void MeStepperOnBoard::enableOutputs(void);
 *    22. void MeStepperOnBoard::update(void);
 *    23. int16_t MeStepperOnBoard::getPort(void);
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan        2016/03/05     1.0.0            Bulid the new
 * Mark Yan        2016/05/06     1.0.1            Add function move and moveTo
 * </pre>
 */
 
#ifndef MeStepperOnBoard_h
#define MeStepperOnBoard_h

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
 #include "stdlib.h"
 #include "wiring.h"
#endif

#define NUM_SLOTS          4
#define SLOT_NUM_PINS      8
#define SLOT_1             1
#define SLOT_2             2
#define SLOT_3             3
#define SLOT_4             4
#define SLOT1             SLOT_1
#define SLOT2             SLOT_2
#define SLOT3             SLOT_3
#define SLOT4             SLOT_4

typedef struct megaPi_slot
{
  uint8_t pin[SLOT_NUM_PINS];
} megaPi_slot_type;


// These defs cause trouble on some versions of Arduino
#undef round
typedef void (*cb)(int,int); 
/**
 * Class: MeStepperOnBoard
 * \par Description
 * Declaration of Class MeStepperOnBoard.
 */
class MeStepperOnBoard
{
public:
/**
 * Alternate Constructor which can call your own function to map the stepper to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
  MeStepperOnBoard();
  
/**
 * Alternate Constructor which can call your own function to map the stepper to arduino port.
 * \param[in]
 * slot - The slot of MegaPi boards.
 */
  MeStepperOnBoard(int slot);

/**
 * \par Function
 *    setMicroStep
 * \par Description
 *    Set the micro step.
 * \param[in]
 *    value - the Subdivided value.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void setMicroStep(int8_t value);

/**
 * \par Function
 *    setpin
 * \par Description
 *    Set pin for Stepper.
 * \param[in]
 *    slot - The slot of MegaPi boards.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void setpin(int slot);

/**
 * \par Function
 *    moveTo
 * \par Description
 *    Stepper moves to the absolute position.
 * \param[in]
 *    absolute - The absolute length to Stepper's movement.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void moveTo(long absolute);

/**
 * \par Function
 *    moveTo
 * \par Description
 *    Stepper moves to the absolute position.
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
  void  moveTo(long absolute,cb callback,int extId); 

/**
 * \par Function
 *    move
 * \par Description
 *    Stepper moves to the relative positions.
 * \param[in]
 *    relative - The relative length to Stepper's movement.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void move(long relative);

/**
 * \par Function
 *    move
 * \par Description
 *    Stepper moves to the relative positions.
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
  void  move(long relative,cb callback,int extId);
  
/**
 * \par Function
 *    run
 * \par Description
 *    Stepper's status----run or not.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    Return the status.
 * \par Others
 *    None
 */
  boolean run(void);
  
/**
 * \par Function
 *    runSpeed
 * \par Description
 *    The speed of Stepper's running.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    Return true or false.
 * \par Others
 *    None
 */
  boolean runSpeed(void);
  
/**
 * \par Function
 *    setMaxSpeed
 * \par Description
 *    Set Max Speed for Stepper.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void setMaxSpeed(float speed);
  
/**
 * \par Function
 *    setAcceleration
 * \par Description
 *    Set Acceleration for Stepper.
 * \param[in]
 *    acceleration - The acceleration for Stepper.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void setAcceleration(float acceleration);
  
/**
 * \par Function
 *    setSpeed
 * \par Description
 *    Set Speed for Stepper.
 * \param[in]
 *    speed - The speed of Stepper.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void setSpeed(float speed);
  
/**
 * \par Function
 *    speed
 * \par Description
 *    The Speed of Stepper.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    Return the Stepper's speed.
 * \par Others
 *    None
 */
  float speed(void);
  
/**
 * \par Function
 *    distanceToGo
 * \par Description
 *    The distance that Stepper should go.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    Return the length of Stepper's running.
 * \par Others
 *    None
 */
  long distanceToGo(void);
  
/**
 * \par Function
 *    targetPosition
 * \par Description
 *    Stepper goes to target position.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    Return the position of Stepper.
 * \par Others
 *    None
 */
  long targetPosition(void);
  
/**
 * \par Function
 *    currentPosition
 * \par Description
 *    Stepper's current position.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    Return the current position of Stepper.
 * \par Others
 *    None
 */
  long currentPosition(void);
  
/**
 * \par Function
 *    setCurrentPosition
 * \par Description
 *    Set Stepper's current position.
 * \param[in]
 *    position - The current position for Stepper.
 * \par Output
 *    None
 * \par Return
 *    Return the current position of Stepper.
 * \par Others
 *    None
 */  
  void setCurrentPosition(long position);
  
/**
 * \par Function
 *    runToPosition
 * \par Description
 *    Stepper runs to position.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */  
  void runToPosition(void);
  
/**
 * \par Function
 *    runSpeedToPosition
 * \par Description
 *    The speed of Stepper on the way to position.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    Return true or false.
 * \par Others
 *    None
 */
  boolean runSpeedToPosition(void);
  
/**
 * \par Function
 *    runToNewPosition
 * \par Description
 *    The Stepper runs to new position.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void runToNewPosition(long position);

/**
 * \par Function
 *    disableOutputs
 * \par Description
 *    The Stepper disable the outputs.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void disableOutputs(void);

/**
 * \par Function
 *    enableOutputs
 * \par Description
 *    The Stepper disable the outputs.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void enableOutputs(void);

/**
 * \par Function
 *    update
 * \par Description
 *    The Stepper loop function, used to move the stepper.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void update(void);
/**
 * \par Function
 *    step
 * \par Description
 *    Stepper runs step by step.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  virtual void step(void);
  
/**
 * \par Function
 *    getPort
 * \par Description
 *    get the slot number of the stepper.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    The Slot number Stepper used.
 * \par Others
 *    None
 */
  int16_t getPort(void);

protected:
/**
 * \par Function
 *    computeNewSpeed
 * \par Description
 *    Compute New Speed of Stepper.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
  void computeNewSpeed();
private:
  uint8_t _dir_data;
  uint8_t _step_data;
  uint8_t _enable_pin;
  uint8_t _micro_step_pin1;
  uint8_t _micro_step_pin2;
  uint8_t _micro_step_pin3;
  uint8_t _reset_pin;
  uint8_t _sleep_pin;
  uint8_t _micro_step;
  uint8_t _dir;          // 2 or 4
  long _currentPos;      // Steps
  long _targetPos;       // Steps
  float _speed;          // Steps per second
  float _maxSpeed;
  float _acceleration;
  unsigned long _stepInterval;
  unsigned long _lastStepTime;
  /// The step counter for speed calculations
  long _n;

  /// Initial step size in microseconds
  float _c0;

  /// Last step size in microseconds
  float _cn;

  /// Min step size in microseconds based on maxSpeed
  float _cmin; // at max speed
  cb _callback;
  int16_t _slot;
  int16_t _extId;
  boolean _moving;
  int16_t _mode;
};

#endif 
