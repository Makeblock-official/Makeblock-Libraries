#include "MeBuzzer.h"

uint8_t buzzer_pin;
#ifdef ME_PORT_DEFINED
MeBuzzer::MeBuzzer()
{
  buzzer_pin = 8;
}

MeBuzzer::MeBuzzer(uint8_t port):MePort(port)
{
  buzzer_pin = s2;
}

MeBuzzer::MeBuzzer(uint8_t port, uint8_t slot):MePort(port)
{
  buzzer_pin = s2;
  if(slot == SLOT2)
  {   
    buzzer_pin = s2;
  }
  else
  {
    buzzer_pin = s1;
  }
}
#else // ME_PORT_DEFINED
MeBuzzer::MeBuzzer(int pin)
{
  buzzer_pin = pin;
}
#endif // ME_PORT_DEFINED

void MeBuzzer::setpin(int pin)
{
  buzzer_pin = pin;
}

// frequency (in hertz) and duration (in milliseconds).
void MeBuzzer::tone(int pin, uint16_t frequency, uint32_t duration)
{
  buzzer_pin = pin;
  int period = 1000000L / frequency;
  int pulse = period / 2;
  for (long i = 0; i < duration * 1000L; i += period) 
  {
    digitalWrite(buzzer_pin, HIGH);
    delayMicroseconds(pulse);
    digitalWrite(buzzer_pin, LOW);
    delayMicroseconds(pulse);
  }
}

// frequency (in hertz) and duration (in milliseconds).
void MeBuzzer::tone(uint16_t frequency, uint32_t duration)
{
  int period = 1000000L / frequency;
  int pulse = period / 2;
  for (long i = 0; i < duration * 1000L; i += period) 
  {
    digitalWrite(buzzer_pin, HIGH);
    delayMicroseconds(pulse);
    digitalWrite(buzzer_pin, LOW);
    delayMicroseconds(pulse);
  }
}

void MeBuzzer::noTone(int pin)
{
  buzzer_pin = pin;
  digitalWrite(buzzer_pin, LOW);
}

void MeBuzzer::noTone()
{
  digitalWrite(buzzer_pin, LOW);
}