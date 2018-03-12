Servo servos[8];  
MeMegaPiDCMotor dc;
MeTemperature ts;
MeRGBLed led;
MeUltrasonicSensor *us = NULL;
Me7SegmentDisplay seg;
MePort generalDevice;
MeLEDMatrix ledMx;
MeInfraredReceiver *ir = NULL;
MeGyro gyro;
MeCompass Compass;
MeJoystick joystick;
MeBuzzer buzzer;
MeHumiture humiture;
MeFlameSensor FlameSensor;
MeGasSensor GasSensor;
MePressureSensor pressureSensor;
MeSerial mySerial(PORT_9);
MeStepperOnBoard steppers[4];
MeEncoderOnBoard encoders[4];
long encodersPosition[4] = {0,0,0,0};
long steppersPosition[4] = {0,0,0,0};
boolean encodersMoving[4] = {false,false,false,false};
boolean steppersMoving[4] = {false,false,false,false};

int16_t servo_pins[8]={0,0,0,0,0,0,0,0};
int16_t moveSpeed = 255;

#define MOVE_STOP       0x00
#define MOVE_FORWARD    0x01
#define MOVE_BACKWARD   0x02

int16_t move_status = MOVE_STOP;

#define BLUETOOTH_MODE                       0x00
#define AUTOMATIC_OBSTACLE_AVOIDANCE_MODE    0x01
#define BALANCED_MODE                        0x02
#define IR_REMOTE_MODE                       0x03

//#define POWER_PORT                           A4
//#define BUZZER_PORT                          45
//#define RGBLED_PORT                          44

#define DATA_SERIAL                            0
#define DATA_SERIAL1                           1
#define DATA_SERIAL2                           2
#define DATA_SERIAL3                           3

uint8_t megapi_mode = BLUETOOTH_MODE;
uint8_t index = 0;
uint8_t dataLen;
uint8_t modulesLen=0;
uint8_t irRead = 0;
uint8_t prevc=0;
uint8_t BluetoothSource = DATA_SERIAL;
char serialRead;
char buffer[52];
char bufferBt1[52];
char bufferBt2[52];

char buf[64];
char bufindex;

double lastTime = 0.0;
double currentTime = 0.0;
float angleServo = 90.0;

boolean isStart = false;
boolean isAvailable = false;

#define VERSION 0
#define ULTRASONIC_SENSOR 1
#define TEMPERATURE_SENSOR 2
#define LIGHT_SENSOR 3
#define POTENTIONMETER 4
#define JOYSTICK 5
#define GYRO 6
#define SOUND_SENSOR 7
#define RGBLED 8
#define SEVSEG 9
#define MOTOR 10
#define SERVO 11
#define ENCODER 12
#define IR 13
#define PIRMOTION 15
#define INFRARED 16
#define LINEFOLLOWER 17
#define RGBLED_DISPLAY 18
#define RGBLED_SHOW 19
#define SHUTTER 20
#define LIMITSWITCH 21
#define BUTTON 22
#define HUMITURE 23
#define FLAMESENSOR 24
#define GASSENSOR 25
#define COMPASS 26
#define TEMPERATURE_SENSOR_1 27
#define ANGULAR_SENSOR 28
#define PRESSURE_SENSOR 29
#define DIGITAL 30
#define ANALOG 31
#define PWM 32
#define SERVO_PIN 33
#define TONE 34
#define PULSEIN 35
#define ULTRASONIC_ARDUINO 36
#define STEPPER 40
#define LEDMATRIX 41
#define TIMER 50
#define JOYSTICK_MOVE 52
#define COMMON_COMMONCMD 60
  //Secondary command
  #define SET_STARTER_MODE     0x10
  #define SET_AURIGA_MODE      0x11
  #define SET_MEGAPI_MODE      0x12
  #define GET_BATTERY_POWER    0x70
  
#define ENCODER_BOARD 61
  //Motion type
  #define ENCODER_BOARD_RUN  0x01
  #define ENCODER_BOARD_MOVE 0x02
  #define ENCODER_BOARD_MOVE_TO 0x03
  #define ENCODER_BOARD_SETTING 0x04
  //Read type
  #define ENCODER_BOARD_POS    0x01
  #define ENCODER_BOARD_SPEED  0x02
  
#define STEPPER_BOARD 62  
  //Motion type
  #define STEPPER_BOARD_RUN  0x01
  #define STEPPER_BOARD_MOVE 0x02
  #define STEPPER_BOARD_MOVE_TO 0x03
  #define STEPPER_BOARD_SETTING 0x04
  //Read type
  #define STEPPER_BOARD_POS    0x01
  #define STEPPER_BOARD_SPEED  0x02
  
#define GET 1
#define RUN 2
#define RESET 4
#define START 5


typedef struct MeModule
{
  int16_t device;
  int16_t port;
  int16_t slot;
  int16_t pin;
  int16_t index;
  float values[3];
} MeModule;

union
{
  byte byteVal[4];
  float floatVal;
  long longVal;
}val;

union
{
  byte byteVal[8];
  double doubleVal;
}valDouble;

union
{
  byte byteVal[2];
  short shortVal;
}valShort;
MeModule modules[12];
#if defined(__AVR_ATmega32U4__) 
  int analogs[12]={A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11};
#endif
#if defined(__AVR_ATmega328P__) or defined(__AVR_ATmega168__)
  int analogs[8]={A0,A1,A2,A3,A4,A5,A6,A7};
#endif
#if defined(__AVR_ATmega1280__)|| defined(__AVR_ATmega2560__)
  int analogs[16]={A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15};
#endif
long measurement_speed_time = 0;
long lasttime_angle = 0;
long lasttime_speed = 0;
long last_Pulse_pos_encoder1 = 0;
long last_Pulse_pos_encoder2 = 0;
long last_Pulse_pos_encoder3 = 0;
long last_Pulse_pos_encoder4 = 0;
//////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
  double P, I, D;
  double Setpoint, Output, Integral,differential, last_error;
} PID;

PID  PID_angle, PID_speed, PID_turn;
PID  PID_speed_left, PID_speed_right;
