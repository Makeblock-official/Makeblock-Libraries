/**
 * @file EncoderPidPositionTest.ino
 * @brief Automated test bench for the encoder position PID (legacy vs cascade).
 *
 * Runs a sequence of moves and reports, for each motor: time to reach the
 * target, overshoot, final error, PWM at rest, callback count and whether the
 * lock was lost while settling.
 *
 * Serial commands (115200 baud, newline ending):
 *   T                  Run the full test sequence in the current mode
 *   M <deg> <rpm>      Single relative move (motor 2 goes the opposite way)
 *   H                  Hand push watch: report lock lost / regained events
 *   C <0|1>            0 = legacy mode, 1 = cascade mode
 *   P/I/D <value>      Set position gains
 *   B <degrees>        Set position deadband
 *   S                  Stop the sequence and hold the current position
 *   ?                  Print the current configuration
 */

#include <MeAuriga.h>
#include <ctype.h>
#include <stdlib.h>

MeEncoderOnBoard Encoder_1(SLOT1);
MeEncoderOnBoard Encoder_2(SLOT2);
MeEncoderOnBoard *encoders[2] = { &Encoder_1, &Encoder_2 };

struct TestMove
{
  long degrees;
  float rpm;
};

const TestMove TEST_MOVES[] = {
  { 360, 50 }, { -360, 50 }, { 360, 20 }, { 360, 100 }, { 360, 180 },
  { 15, 50 },  { 30, 50 },   { -30, 50 }, { 720, 100 }
};
const uint8_t TEST_COUNT = sizeof(TEST_MOVES) / sizeof(TEST_MOVES[0]);

const unsigned long MOVE_TIMEOUT = 8000;
const unsigned long SETTLE_TIME = 1500;
const unsigned long SAMPLE_RATE = 10;

enum AppState { IDLE, MOVING, SETTLING, WATCHING };
AppState appState = IDLE;

bool cascadeMode = true;
float positionP = 1.8f;
float positionI = 0.0f;
float positionD = 1.2f;
long positionDeadband = 10;

bool sequenceRunning = false;
uint8_t testIndex = 0;
unsigned long moveStartTime = 0;
unsigned long settleStartTime = 0;
bool lastReached[2];

struct MotorStats
{
  long target;
  int8_t dir;
  long maxOvershoot;
  unsigned long reachedTime;
  long pwmSum;
  uint16_t pwmSamples;
  int16_t pwmMaxAbs;
  bool lockLost;
  uint8_t callbackCount;
};

MotorStats stats[2];

const uint8_t SERIAL_BUFFER_SIZE = 48;
char serialBuffer[SERIAL_BUFFER_SIZE];
uint8_t serialBufferLength = 0;
bool serialBufferOverflow = false;

void isrProcessEncoder1(void)
{
  if(digitalRead(Encoder_1.getPortB()) == 0)
  {
    Encoder_1.pulsePosMinus();
  }
  else
  {
    Encoder_1.pulsePosPlus();
  }
}

void isrProcessEncoder2(void)
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

// Called from Encoder.loop(), not from an interrupt.
void onTargetReached(int16_t slot, int16_t extId)
{
  if(extId >= 1 && extId <= 2)
  {
    stats[extId - 1].callbackCount++;
  }
}

void applyConfig(void)
{
  for(uint8_t i = 0; i < 2; i++)
  {
    encoders[i]->setPosPid(positionP, positionI, positionD);
    encoders[i]->setFullPositionPidEnabled(cascadeMode);
    encoders[i]->setPosDeadBand(positionDeadband);
  }
}

void printConfig(void)
{
  Serial.print(F("Mode="));
  Serial.print(cascadeMode ? F("CASCADE") : F("LEGACY"));
  Serial.print(F(" P="));
  Serial.print(positionP, 3);
  Serial.print(F(" I="));
  Serial.print(positionI, 3);
  Serial.print(F(" D="));
  Serial.print(positionD, 3);
  Serial.print(F(" Deadband="));
  Serial.println(positionDeadband);
}

void startMove(long relativeDegrees, float rpm)
{
  for(uint8_t i = 0; i < 2; i++)
  {
    long start = encoders[i]->getCurPos();
    long target = start + ((i == 0) ? relativeDegrees : -relativeDegrees);

    stats[i].target = target;
    stats[i].dir = (target >= start) ? 1 : -1;
    stats[i].maxOvershoot = 0;
    stats[i].reachedTime = 0;
    stats[i].pwmSum = 0;
    stats[i].pwmSamples = 0;
    stats[i].pwmMaxAbs = 0;
    stats[i].lockLost = false;
    stats[i].callbackCount = 0;

    encoders[i]->moveTo(target, rpm, i + 1, onTargetReached);
  }

  Serial.print(F("--- Move "));
  Serial.print(relativeDegrees);
  Serial.print(F("deg @ "));
  Serial.print(rpm, 0);
  Serial.print(F("rpm, "));
  printConfig();

  moveStartTime = millis();
  appState = MOVING;
}

void startNextTest(void)
{
  if(testIndex >= TEST_COUNT)
  {
    sequenceRunning = false;
    appState = IDLE;
    Serial.println(F("=== Sequence done ==="));
    return;
  }

  Serial.print(F("[Test "));
  Serial.print(testIndex + 1);
  Serial.print(F("/"));
  Serial.print(TEST_COUNT);
  Serial.print(F("] "));
  startMove(TEST_MOVES[testIndex].degrees, TEST_MOVES[testIndex].rpm);
  testIndex++;
}

void reportResults(bool timedOut)
{
  for(uint8_t i = 0; i < 2; i++)
  {
    long finalError = encoders[i]->distanceToGo();
    float pwmAvg = (stats[i].pwmSamples > 0) ? (float)stats[i].pwmSum / stats[i].pwmSamples : 0.0f;

    Serial.print(F("  M"));
    Serial.print(i + 1);
    Serial.print(F(" reach="));
    if(stats[i].reachedTime > 0)
    {
      Serial.print(stats[i].reachedTime);
      Serial.print(F("ms"));
    }
    else
    {
      Serial.print(F("NEVER"));
    }
    Serial.print(F(" overshoot="));
    Serial.print(stats[i].maxOvershoot);
    Serial.print(F("deg finalErr="));
    Serial.print(finalError);
    Serial.print(F("deg restPwmAvg="));
    Serial.print(pwmAvg, 1);
    Serial.print(F(" restPwmMax="));
    Serial.print(stats[i].pwmMaxAbs);
    Serial.print(F(" callbacks="));
    Serial.print(stats[i].callbackCount);
    Serial.print(F(" lockLost="));
    Serial.println(stats[i].lockLost ? F("yes") : F("no"));

    if(timedOut)
    {
      Serial.println(F("    !! TIMEOUT"));
    }
    // A lost lock means the controller corrected a coast past the deadband,
    // so PWM while settling is expected in that case.
    if(stats[i].pwmMaxAbs != 0 && !stats[i].lockLost)
    {
      Serial.println(F("    !! PWM at rest inside the deadband"));
    }
    if(stats[i].callbackCount != 1)
    {
      Serial.println(F("    !! Callback count != 1"));
    }
    if(abs(finalError) > positionDeadband)
    {
      Serial.println(F("    !! Final error outside deadband"));
    }
  }
}

void finishMove(bool timedOut)
{
  reportResults(timedOut);

  if(sequenceRunning)
  {
    startNextTest();
  }
  else
  {
    appState = IDLE;
  }
}

void trackOvershoot(void)
{
  for(uint8_t i = 0; i < 2; i++)
  {
    long overshoot = stats[i].dir * (encoders[i]->getCurPos() - stats[i].target);
    if(overshoot > stats[i].maxOvershoot)
    {
      stats[i].maxOvershoot = overshoot;
    }
  }
}

void movingState(unsigned long ct)
{
  trackOvershoot();

  for(uint8_t i = 0; i < 2; i++)
  {
    if(stats[i].reachedTime == 0 && encoders[i]->isTarPosReached())
    {
      stats[i].reachedTime = ct - moveStartTime;
    }
  }

  if(stats[0].reachedTime > 0 && stats[1].reachedTime > 0)
  {
    settleStartTime = ct;
    appState = SETTLING;
  }
  else if(ct - moveStartTime > MOVE_TIMEOUT)
  {
    finishMove(true);
  }
}

void settlingState(unsigned long ct)
{
  trackOvershoot();

  for(uint8_t i = 0; i < 2; i++)
  {
    int16_t pwm = abs(encoders[i]->getCurPwm());
    stats[i].pwmSum += pwm;
    stats[i].pwmSamples++;
    if(pwm > stats[i].pwmMaxAbs)
    {
      stats[i].pwmMaxAbs = pwm;
    }
    if(!encoders[i]->isTarPosReached())
    {
      stats[i].lockLost = true;
    }
  }

  if(ct - settleStartTime >= SETTLE_TIME)
  {
    finishMove(false);
  }
}

void watchingState(void)
{
  for(uint8_t i = 0; i < 2; i++)
  {
    bool reached = encoders[i]->isTarPosReached();
    if(reached != lastReached[i])
    {
      lastReached[i] = reached;
      Serial.print(F("  M"));
      Serial.print(i + 1);
      Serial.print(reached ? F(" lock regained") : F(" lock lost"));
      Serial.print(F(" err="));
      Serial.print(encoders[i]->distanceToGo());
      Serial.print(F(" pwm="));
      Serial.print(encoders[i]->getCurPwm());
      Serial.print(F(" callbacks="));
      Serial.println(stats[i].callbackCount);
    }
  }
}

void stateManager(unsigned long ct)
{
  static unsigned long lastTime = 0;

  if(ct - lastTime < SAMPLE_RATE)
  {
    return;
  }

  lastTime = ct;

  switch(appState)
  {
    case IDLE:
      break;
    case MOVING:
      movingState(ct);
      break;
    case SETTLING:
      settlingState(ct);
      break;
    case WATCHING:
      watchingState();
      break;
  }
}

bool parseNextNumber(char **text, double *value)
{
  while(isspace((unsigned char)**text))
  {
    (*text)++;
  }

  char *end;
  *value = strtod(*text, &end);
  if(end == *text)
  {
    return false;
  }

  *text = end;
  return true;
}

void handleSerialCommand(char *commandLine)
{
  char *text = commandLine;
  while(isspace((unsigned char)*text))
  {
    text++;
  }

  char command = toupper((unsigned char)*text++);
  double value = 0;
  bool hasValue = parseNextNumber(&text, &value);

  switch(command)
  {
    case 'T':
      Serial.println(F("=== Sequence start ==="));
      sequenceRunning = true;
      testIndex = 0;
      startNextTest();
      break;
    case 'M':
    {
      double rpm;
      if(hasValue && parseNextNumber(&text, &rpm) && rpm > 0.0)
      {
        sequenceRunning = false;
        startMove((long)value, (float)rpm);
      }
      break;
    }
    case 'H':
      sequenceRunning = false;
      for(uint8_t i = 0; i < 2; i++)
      {
        lastReached[i] = encoders[i]->isTarPosReached();
      }
      appState = WATCHING;
      Serial.println(F("Watching: push the wheels by hand, S to stop"));
      break;
    case 'C':
      if(hasValue)
      {
        cascadeMode = (value != 0.0);
        applyConfig();
        printConfig();
      }
      break;
    case 'P':
      if(hasValue) { positionP = (float)value; applyConfig(); printConfig(); }
      break;
    case 'I':
      if(hasValue) { positionI = (float)value; applyConfig(); printConfig(); }
      break;
    case 'D':
      if(hasValue) { positionD = (float)value; applyConfig(); printConfig(); }
      break;
    case 'B':
      if(hasValue)
      {
        positionDeadband = (value < 0.0) ? 0 : (long)value;
        applyConfig();
        printConfig();
      }
      break;
    case 'S':
      sequenceRunning = false;
      for(uint8_t i = 0; i < 2; i++)
      {
        encoders[i]->moveTo(encoders[i]->getCurPos(), 50);
      }
      appState = IDLE;
      Serial.println(F("Stopped"));
      break;
    case '?':
      printConfig();
      break;
    default:
      break;
  }
}

void serialInputTask(void)
{
  while(Serial.available() > 0)
  {
    char input = Serial.read();

    if(input == '\r')
    {
      continue;
    }

    if(input == '\n')
    {
      if(!serialBufferOverflow && serialBufferLength > 0)
      {
        serialBuffer[serialBufferLength] = '\0';
        handleSerialCommand(serialBuffer);
      }
      serialBufferLength = 0;
      serialBufferOverflow = false;
    }
    else if(serialBufferLength < SERIAL_BUFFER_SIZE - 1)
    {
      serialBuffer[serialBufferLength++] = input;
    }
    else
    {
      serialBufferOverflow = true;
    }
  }
}

void setup()
{
  Serial.begin(115200);

  attachInterrupt(Encoder_1.getIntNum(), isrProcessEncoder1, RISING);
  attachInterrupt(Encoder_2.getIntNum(), isrProcessEncoder2, RISING);

  // Configure PWM timers as in the Auriga encoder PID example.
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);
  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);

  for(uint8_t i = 0; i < 2; i++)
  {
    encoders[i]->setPulse(9);
    encoders[i]->setRatio(39.267);
    encoders[i]->setSpeedPid(0.18f, 0.0f, 0.0f);
  }
  applyConfig();

  Serial.println(F("Commands: T, M <deg> <rpm>, H, C <0|1>, P/I/D <v>, B <deg>, S, ?"));
  printConfig();
}

void loop()
{
  serialInputTask();
  Encoder_1.loop();
  Encoder_2.loop();
  stateManager(millis());
}
