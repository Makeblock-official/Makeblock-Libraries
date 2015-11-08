#ifndef MeMbotDCMotor_H
#define MeMbotDCMotor_H

#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>
#include "MeConfig.h"

#ifdef ME_PORT_DEFINED
#include "MePort.h"
#endif

#ifdef ME_PORT_DEFINED
#include "MeDCMotor.h"

class MBotDCMotor : public MeDCMotor
{
public:
  MBotDCMotor(uint8_t port);

  void move(int direction, int speed);
};
#endif //ME_PORT_DEFINED
#endif //MeMbotDCMotor_H

