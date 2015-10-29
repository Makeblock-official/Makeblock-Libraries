// MeStepper.cpp

#include "MeStepper.h"

typedef enum
{
  DIRECTION_CCW = 0,  ///< Clockwise
  DIRECTION_CW  = 1   ///< Counter-Clockwise
} Direction;

MeStepper::MeStepper(): MePort(0)
{

}

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

void MeStepper::moveTo(long absolute)
{
  if (_targetPos != absolute)
  {
    _targetPos = absolute;
    computeNewSpeed();
  }
}

void MeStepper::move(long relative)
{
  moveTo(_currentPos + relative);
}

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

long MeStepper::distanceToGo()
{
  return _targetPos - _currentPos;
}

long MeStepper::targetPosition()
{
  return _targetPos;
}

long MeStepper::currentPosition()
{
  return _currentPos;
}

void MeStepper::setCurrentPosition(long position)
{
  _targetPos = _currentPos = position;
  _n = 0;
  _stepInterval = 0;
}

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

float MeStepper::speed()
{
  return _speed;
}

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

// Blocks until the target position is reached
void MeStepper::runToPosition()
{
  while (run())
  {
    ;
  }
}

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

// Blocks until the new target position is reached
void MeStepper::runToNewPosition(long position)
{
  moveTo(position);
  runToPosition();
}

