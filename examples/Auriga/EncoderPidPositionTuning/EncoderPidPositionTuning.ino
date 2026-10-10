/**
 * @file EncoderPidPositionTuning.ino
 * @brief Tune the Auriga encoder position controller from the serial monitor.
 *
 * Serial commands (115200 baud, newline ending):
 *   P <value>                 Set position P gain
 *   I <value>                 Set position I gain
 *   D <value>                 Set position D gain
 *   B <degrees>               Set position deadband
 *   M <relative degrees> <rpm> Move both motors in opposite directions
 *
 * Example: M 360 50
 */

#include <MeAuriga.h>
#include <ctype.h>
#include <stdlib.h>

MeEncoderOnBoard Encoder_1(SLOT1);
MeEncoderOnBoard Encoder_2(SLOT2);

enum AppState { STOP, STATE_A };

AppState appState = STOP;
unsigned long currentTime = 0;

float positionP = 1.8f;
float positionI = 0.0f;
float positionD = 1.2f;
long positionDeadband = 10;
float moveSpeed = 50.0f;

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

void applyPositionPid(void)
{
  Encoder_1.setPosPid(positionP, positionI, positionD);
  Encoder_2.setPosPid(positionP, positionI, positionD);
}

void startMove(long relativeDegrees, float speed)
{
  moveSpeed = speed;
  Encoder_1.moveTo(Encoder_1.getCurPos() + relativeDegrees, speed);
  Encoder_2.moveTo(Encoder_2.getCurPos() - relativeDegrees, speed);
  appState = STATE_A;
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
  double value;
  if(!parseNextNumber(&text, &value))
  {
    return;
  }

  switch(command)
  {
    case 'P':
      positionP = (float)value;
      applyPositionPid();
      break;
    case 'I':
      positionI = (float)value;
      applyPositionPid();
      break;
    case 'D':
      positionD = (float)value;
      applyPositionPid();
      break;
    case 'B':
      positionDeadband = (value < 0.0) ? 0 : (long)value;
      Encoder_1.setPosDeadBand(positionDeadband);
      Encoder_2.setPosDeadBand(positionDeadband);
      break;
    case 'M':
    {
      double speed;
      if(parseNextNumber(&text, &speed) && speed > 0.0)
      {
        startMove((long)value, (float)speed);
      }
      break;
    }
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

void movingState(unsigned long ct)
{
  static unsigned long lastTime = 0;
  const unsigned long rate = 50;
  static bool firstTime = true;

  if(firstTime)
  {
    lastTime = ct;
    firstTime = false;
    return;
  }

  if(ct - lastTime < rate)
  {
    return;
  }

  lastTime = ct;

  bool transition = Encoder_1.isTarPosReached() && Encoder_2.isTarPosReached();
  if(transition)
  {
    appState = STOP;
    firstTime = true;
  }
}

void stateManager(unsigned long ct)
{
  switch(appState)
  {
    case STOP:
      break;
    case STATE_A:
      movingState(ct);
      break;
  }
}

void serialOutputTask(unsigned long ct)
{
  static unsigned long lastTime = 0;
  const unsigned long rate = 50;
  static bool firstOutput = true;

  if(ct - lastTime < rate)
  {
    return;
  }

  lastTime = ct;

  if(firstOutput)
  {
    Serial.println(F("Commands: P/I/D <value>, B <degrees>, M <relative degrees> <rpm>"));
    firstOutput = false;
  }

  Serial.print(F("State="));
  Serial.print(appState == STATE_A ? F("MOVING") : F("STOP"));
  Serial.print(F(" P="));
  Serial.print(positionP, 3);
  Serial.print(F(" I="));
  Serial.print(positionI, 3);
  Serial.print(F(" D="));
  Serial.print(positionD, 3);
  Serial.print(F(" Deadband="));
  Serial.print(positionDeadband);
  Serial.print(F("deg Speed="));
  Serial.print(moveSpeed, 1);
  Serial.print(F("rpm DistanceToGo1="));
  Serial.print(Encoder_1.distanceToGo());
  Serial.print(F(" DistanceToGo2="));
  Serial.println(Encoder_2.distanceToGo());
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

  Encoder_1.setPulse(9);
  Encoder_2.setPulse(9);
  Encoder_1.setRatio(39.267);
  Encoder_2.setRatio(39.267);
  applyPositionPid();
  Encoder_1.setFullPositionPidEnabled(true);
  Encoder_2.setFullPositionPidEnabled(true);
  Encoder_1.setSpeedPid(0.18f, 0.0f, 0.0f);
  Encoder_2.setSpeedPid(0.18f, 0.0f, 0.0f);
  Encoder_1.setPosDeadBand(positionDeadband);
  Encoder_2.setPosDeadBand(positionDeadband);
}

void loop()
{
  currentTime = millis();

  serialInputTask();
  Encoder_1.loop();
  Encoder_2.loop();
  stateManager(currentTime);
  serialOutputTask(currentTime);
}