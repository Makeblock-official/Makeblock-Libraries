/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class MeStepper
 * \brief   Driver for Me Stepper device.
 * @file    MeStepper.cpp
 * @author  MakeBlock
 * @version V1.0.3
 * @date    2016/01/20
 * @brief   Driver for Stepper device.
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
 *    1. void MeStepper::setpin(uint8_t dir_data, uint8_t step_data);
 *    2. void MeStepper::moveTo(long absolute); 
 *    3. void MeStepper::move(long relative);
 *    4. boolean MeStepper::run();
 *    5. boolean MeStepper::runSpeed();
 *    6. void MeStepper::setMaxSpeed(float speed);
 *    7. void MeStepper::setAcceleration(float acceleration);
 *    8. void MeStepper::setSpeed(float speed);
 *    9. float MeStepper::speed();
 *    10. long MeStepper::distanceToGo();
 *    11. long MeStepper::targetPosition();
 *    12. long MeStepper::currentPosition();  
 *    13. void MeStepper::setCurrentPosition(long position);  
 *    14. void MeStepper::runToPosition();
 *    15. boolean MeStepper::runSpeedToPosition();
 *    16. void MeStepper::runToNewPosition(long position);
 *    17. void MeStepper::disableOutputs();
 *    18. void MeStepper::enableOutputs();
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan        2015/10/09     1.0.0            Bulid the new
 * Mark Yan        2015/10/29     1.0.1            Fix minor errors in format
 * forfish         2015/11/11     1.0.2            Add description.
 * Mark Yan        2016/01/20     1.0.3            Fix minor errors in comment.
 * </pre>
 */

#include "MeStepper.h"

typedef enum
{
  DIRECTION_CCW = 0,  ///< Clockwise
  DIRECTION_CW  = 1   ///< Counter-Clockwise
} Direction;

/**
 * Alternate Constructor which can call your own function to map the stepper to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
MeStepper::MeStepper(): MePort(0)
{

}

/**
 * Alternate Constructor which can call your own function to map the stepper to arduino port,
 * the slot2 pin will be used here since specify slot is not be set.
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
MeStepper::MeStepper(uint8_t port): MePort(port)
{
  _currentPos = 0;
  _targetPos = 0;
  _acceleration = 0;
  _lastStepTime = micros();
  _speed = 0;
  _dir = DIRECTION_CW;
  setCurrentPosition(0);
  pinMode(s1,OUTPUT);
  pinMode(s2,OUTPUT);
}

/**
 * \par Function
 *    setpin
 * \par Description
 *    Set pin for Stepper.
 * \param[in]
 *    dir_data - The direction data of Stepper's movement.
 * \param[in]
 *    step_data - The command data for Stepper.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeStepper::setpin(uint8_t dir_data, uint8_t step_data)
{
  _dir_data = dir_data;
  _step_data = step_data;
  pinMode(_dir_data, OUTPUT);
  pinMode(_step_data, OUTPUT);
#ifdef ME_PORT_DEFINED
  s1 = _dir_data;
  s2 = _step_data;
#endif // ME_PORT_DEFINED
}


/**
 * \par Function
 *    moveTo
 * \par Description
 *    Stepper moves to the aim.
 * \param[in]
 *    absolute - The absolute length to Stepper's movement.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeStepper::moveTo(long absolute)
{
  if (_targetPos != absolute)
  {
    _targetPos = absolute;
    computeNewSpeed();
  }
}

/**
 * \par Function
 *    move
 * \par Description
 *    Stepper moves to the aim.
 * \param[in]
 *    relative - The relative length to Stepper's movement.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeStepper::move(long relative)
{
  moveTo(_currentPos + relative);
}

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
boolean MeStepper::runSpeed()
{
  // Dont do anything unless we actually have a step interval
  if (!_stepInterval)
  {
    return false;
  }

  if (micros() - _lastStepTime > _stepInterval)
  {
    if (_dir == DIRECTION_CW)
    {
      // Clockwise
      _currentPos += 1;
    }
    else
    {
      // Anticlockwise  
      _currentPos -= 1;
    }
    step();
    _lastStepTime = micros();
    return true;
  }
  else
  {
    return false;
  }
}

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
long MeStepper::distanceToGo()
{
  return _targetPos - _currentPos;
}

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
long MeStepper::targetPosition()
{
  return _targetPos;
}

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
long MeStepper::currentPosition()
{
  return _currentPos;
}

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
void MeStepper::setCurrentPosition(long position)
{
  _targetPos = _currentPos = position;
  _n = 0;
  _stepInterval = 0;
}

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
void MeStepper::computeNewSpeed()
{
  long distanceTo = distanceToGo();
  long stepsToStop = (long)((_speed * _speed) / (2.0 * _acceleration));
  if (distanceTo == 0 && stepsToStop <= 1)
  {
    // We are at the target and its time to stop
    _stepInterval = 0;
    _speed = 0.0;
    _n = 0;
    return;
  }

  if (distanceTo > 0)
  {
    // We are anticlockwise from the target
    // Need to go clockwise from here, maybe decelerate now
    if (_n > 0)
    {
      // Currently accelerating, need to decel now? Or maybe going the wrong way?
      if ((stepsToStop >= distanceTo) || _dir == DIRECTION_CCW)
      {
        _n = -stepsToStop; // Start deceleration
      }
    }
    else if (_n < 0)
    {
      // Currently decelerating, need to accel again?
      if ((stepsToStop < distanceTo) && _dir == DIRECTION_CW)
      {
        _n = -_n; // Start accceleration
      }
    }
  }
  else if (distanceTo < 0)
  {
    // We are clockwise from the target
    // Need to go anticlockwise from here, maybe decelerate
    if (_n > 0)
    {
      // Currently accelerating, need to decel now? Or maybe going the wrong way?
      if ((stepsToStop >= -distanceTo) || _dir == DIRECTION_CW)
      {
        _n = -stepsToStop; // Start deceleration
      }
    }
    else if (_n < 0)
    {
      // Currently decelerating, need to accel again?
      if ((stepsToStop < -distanceTo) && _dir == DIRECTION_CCW)
      {
        _n = -_n; // Start accceleration
      }
    }
  }

  // Need to accelerate or decelerate
  if (_n == 0)
  {
    // First step from stopped
    _cn = _c0;
    _dir = (distanceTo > 0) ? DIRECTION_CW : DIRECTION_CCW;
  }
  else
  {
    // Subsequent step. Works for accel (n is +_ve) and decel (n is -ve).
    _cn = _cn - ((2.0 * _cn) / ((4.0 * _n) + 1)); // Equation 13
    _cn = max(_cn, _cmin);
  }
  _n++;
  _stepInterval = _cn;
  _speed = 1000000.0 / _cn;
  if (_dir == DIRECTION_CCW)
  {
    _speed = -_speed;
  }
}

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
boolean MeStepper::run()
{
  if((_speed == 0.0) || (distanceToGo() == 0))
  {
    return false;
  }

  if (runSpeed())
  {
	computeNewSpeed();
    return true;
  }
}

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
void MeStepper::setMaxSpeed(float speed)
{
  if (_maxSpeed != speed)
  {
    _maxSpeed = speed;
    _cmin = 1000000.0 / speed;
    // Recompute _n from current speed and adjust speed if accelerating or cruising
    if (_n > 0)
    {
      _n = (long)((_speed * _speed) / (2.0 * _acceleration)); // Equation 16
      computeNewSpeed();
    }
  }
}

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
void MeStepper::setAcceleration(float acceleration)
{
  if(acceleration == 0.0)
  {
    return;
  }
  if(_acceleration != acceleration)
  {
    _n = _n * (_acceleration / acceleration);
    //	_c0 = sqrt(2.0 / acceleration) * 1000000.0;
    // Accelerates at half the expected rate. Why?
    _c0 = sqrt(1.0/acceleration) * 1000000.0;
    _acceleration = acceleration;
    computeNewSpeed();
  }
}

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
void MeStepper::setSpeed(float speed)
{
  if (speed == _speed)
  {
    return;
  }
  speed = constrain(speed, -_maxSpeed, _maxSpeed);
  if (speed == 0.0)
  {
    _stepInterval = 0;
  }

  else
  {
    _stepInterval = fabs(1000000.0 /speed);
    _dir = (speed > 0.0) ? DIRECTION_CW : DIRECTION_CCW;
  }
  _speed = speed;
}

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
float MeStepper::speed()
{
  return _speed;
}

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
void MeStepper::step()
{
  if(_dir == DIRECTION_CW)
  {
    digitalWrite(s1,LOW);
  }
  else
  {
    digitalWrite(s1,HIGH);
  }
  digitalWrite(s2, HIGH);
  delayMicroseconds(1);
  digitalWrite(s2, LOW);
}

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
void MeStepper::runToPosition()
{
  while (run())
  {
    ;
  }
}

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
boolean MeStepper::runSpeedToPosition()
{
  if (_targetPos == _currentPos)
  {
    return false;
  }
  if (_targetPos >_currentPos)
  {
	_dir = DIRECTION_CW;
  }
  else
  {
	_dir = DIRECTION_CCW;
  }
  return runSpeed();
}

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
void MeStepper::runToNewPosition(long position)
{
  moveTo(position);
  runToPosition();
}

