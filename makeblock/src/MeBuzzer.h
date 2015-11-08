#ifndef MeBuzzer_H
#define MeBuzzer_H

#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>
#include "MeConfig.h"

#ifdef ME_PORT_DEFINED
#include "MePort.h"
#endif // ME_PORT_DEFINED

///@brief Class for MeBuzzer module
class MeBuzzer : public MePort
{
public:
#ifdef ME_PORT_DEFINED
  MeBuzzer();
  MeBuzzer(uint8_t port);
  MeBuzzer(uint8_t port, uint8_t slot);
#else // ME_PORT_DEFINED
  MeBuzzer(int pin);
#endif // ME_PORT_DEFINED
  void setpin(int pin);
  void tone(int pin, uint16_t frequency, uint32_t duration);
  void tone(uint16_t frequency, uint32_t duration = 0);
  void noTone(int pin);
  void noTone();
};

#endif
