# Makeblock Library v3.31.0 - Updated

Arduino Library for Makeblock Electronic Modules

## Note
The original library from Makeblock is not maintained anymore. I have a fleet of Makeblock Ranger robots and I needed to fix some issues with the library. I will try to keep it up to date with the original library.

Feel free to open an issue if you have any problem with the library or create a pull request if you have a fix.

# Copyright notice

In makeblock's library, some of the modules are derived from other open source projects, and also part of some code is inspired by the algorithms of other individuals or organizations. We will retain the copyright of the original open source code.

These modules is derived from other open source projects:

- MeRGBLed
- MeHumitureSensor
- Me7SegmentDisplay
- MeOneWire
- MeStepper
- [MeRGBLineFollower](./doc/devices/MeRGBLineFollower.md)

and these modules is inspired by some projects:

- MeUSBHost

As an open source library, we respect all contributors to the open source community and thank you very much for everyone's supervision.

If you have a discussion about licensing issues, please contact me (myan@makeblock.com -- Mark Yan)

## How to use:

1. Download the library from the Arduino Library manager
2. Search for Makeblock in the Library Manager and select `Makeblock Drive Updated`
4. Click "File-> Examples". There are some test programs in "MakeBlock Drive Updated->"
5. Depending on the type of board you're using, you need to modify the header file to match.

   For example, if you're using a mCore. You should change `#include <MeOrion.h>` to `#include <MeMCore.h>`
   Corresponding boards and there header file are:

   Orion <-------->  MeOrion.h

   BaseBoard <---->  MeBaseBoard.h

   mCore <-------->  MeMCore.h

   Shield <------->  MeShield.h

   Auriga <------->  MeAuriga.h

   MegaPi <------->  MeMegaPi.h

### Include guidance

For sketches and examples, using the board header is the simplest option:

```cpp
#include <MeAuriga.h>
```

For reusable classes and custom libraries, prefer including only the module headers you actually use, for example:

```cpp
#include <MeRGBLed.h>
#include <MeGyro.h>
#include <MeSoundSensor.h>
```

This keeps dependencies smaller and avoids pulling board-level globals into unrelated headers, which helps prevent linker redefinition issues such as `encoder_Port`.

### IR receiver / Tone (buzzer) conflict

The library includes an IR receiver implementation (`MeIR`) that uses a hardware timer ISR. Some buzzer/tone libraries (for example `Tone` or `ezBuzzer`) use the same AVR timer and will cause a linker error like:

```
multiple definition of `__vector_13`
collect2.exe: error: ld returned 1 exit status
```

To avoid this conflict you can disable the built-in IR interrupt handler. The library provides a small configuration header `src/MeIR_config.h` with a toggle:

1. Open `src/MeIR_config.h` in the library folder.
2. Uncomment the line `#define ME_IR_DISABLE_ISR` to disable MeIR's ISR.
3. Restart the Arduino IDE (or PlatformIO) to force a clean library rebuild.

When `ME_IR_DISABLE_ISR` is defined, the MeIR implementation will not define the timer interrupt vector and will not conflict with other libraries that need that timer. You can still use MeIR's send functions; however, receiving via the built-in ISR will be disabled — you'll need to poll `MeIR::loop()` or use another receiver approach if necessary.

If you prefer not to edit the library each time, create a local patch or a config file under your copy of the library (the repository includes `MeIR_config.h` for that purpose).


## New Features in v3.29.0 - Enhanced MeGyro Class

The MeGyro class has been significantly enhanced with full accelerometer and temperature sensor support for the MPU-6050 chip:

### New Methods Added:

#### Accelerometer Methods:
- `double getAccX()` - Get X-axis acceleration in g units (gravity = 1.0g)
- `double getAccY()` - Get Y-axis acceleration in g units
- `double getAccZ()` - Get Z-axis acceleration in g units  
- `double getAcc(uint8_t index)` - Get acceleration for specified axis (1=X, 2=Y, 3=Z)

#### Temperature Method:
- `double getTemperature()` - Get temperature from MPU-6050 internal sensor in degrees Celsius

### Usage Example:
```cpp
#include "MeAuriga.h"

MeGyro gyro(0, 0x69); // For Auriga onboard gyro

void setup() {
  Serial.begin(115200);
  gyro.begin();
}

void loop() {
  gyro.update();
  
  // Read gyroscope angles (degrees)
  Serial.print("Angles - X: "); Serial.print(gyro.getAngleX(), 2);
  Serial.print(" Y: "); Serial.print(gyro.getAngleY(), 2);
  Serial.print(" Z: "); Serial.println(gyro.getAngleZ(), 2);
  
  // Read accelerations (g units)
  Serial.print("Accel - X: "); Serial.print(gyro.getAccX(), 3);
  Serial.print(" Y: "); Serial.print(gyro.getAccY(), 3);
  Serial.print(" Z: "); Serial.println(gyro.getAccZ(), 3);
  
  // Read temperature
  Serial.print("Temperature: "); Serial.print(gyro.getTemperature(), 1);
  Serial.println(" °C");
  
  delay(100);
}
```

### Example Programs:
- **Auriga_MeGyroTest**: Enhanced test program with Serial Plotter compatibility
- **AurigaTempSensors**: Demonstrates reading from both gyro temperature sensor and onboard NTC thermistor

## Revision of history:

|Author      |       Time      |   Version    |    Descr     |
|:--------   |      :-----:    |   :----:     |    :-----    |
|Mark Yan    |     2015/07/24  |   3.0.0      |    Rebuild the old lib.|
|Rafael Lee  |     2015/09/02  |   3.1.0      |    Added some comments and macros.|
|Lawrence    |     2015/09/09  |   3.2.0      |    Include some Arduino's official headfiles which path specified.|
|Mark Yan    |     2015/11/02  |   3.2.1      |    fix bug on MACOS.|
|Mark Yan    |     2016/01/21  |   3.2.2      |    fix some library bugs.|
|Mark Yan    |     2016/05/17  |   3.2.3      |    add support for MegaPi and Auriga Board.|
|Mark Yan    |     2016/07/27  |   3.2.4      |    fix some JIRA issue, add PID motion for Megapi/Auriga on board encoder motor.|
|Mark Yan    |     2018/05/16  |   3.2.5      |    Correct copyright information.|
|Vincent He  |     2019/01/04  |   3.2.6      |    1.Mbot /ranger adds the function of communication variables. 2.Solve the blocking problem of 9g steering gear. 3.Solve the problem that the intelligent steering gear cannot read back the parameters. 4.Add version number function. 5.High power code motor reinforcement version query function. 6.Solve the problem of SetColor (uint8_t index, long value) function error in mergharp.cpp. 7.The mBot board cannot extinguish the RGB. First upload the program with the RGB in any color, and then upload the program with the RGB in all colors. The RGB cannot extinguish (MeRGBLed bled. CPP file). 8.In the MegaPi firmware, SLOT1 is changed to slot_num instead of parameter transmission in the command processing stepper motor.|
|Vincent He  |     2019/09/02  |   3.2.7      |    1.fix the problem that the electronic compass Mecompass is hung on the Orion mainboard 7 or 8 ports and communication will be hung dead. 2.fix the problem that the function getPointFast() in MeHumitureSensor.cpp does not normally output the value. 3.fix the problem that compile smartservo_test.ino firmware error report using the arduino1.6.5 environment with mBlock V3.4.12. 4.remove MeSuperVariable.cpp/MeSuperVariable.h. 5.fix the problem that ultrasonic module can only measure the maximum range of 375cm,and the maximum range of normal requirements is 400cm.|
| Nicolas Bourré |  2023/10/16 | 3.28 | 1. Added missing `gyro.getGyroZ`. 2. Compliant semver.org version number. 3. Added `gyro.resetData` function. 4. Modified the gyro address if it is an Auriga board. |
| Nicolas Bourré |  2025/04/30 | 3.28.1 | Added the MeRGBLineFollower class. |
| Nick B |  2025/09/24 | 3.29.0 | 1. Added `getTemperature()` method to MeGyro class for MPU-6050 internal temperature sensor. 2. Added full accelerometer functionality to MeGyro class with `getAccX()`, `getAccY()`, `getAccZ()`, and `getAcc(index)` methods. 3. Fixed multiple definition linker errors by moving MeAuriga global array initializations to .cpp file. 4. Fixed accelerometer initialization by properly configuring register 0x1c for ±2g range. 5. Added AurigaTempSensors example demonstrating dual temperature sensor reading (gyro + onboard NTC). 6. Updated Auriga_MeGyroTest example with Serial Plotter compatibility and comprehensive sensor data display. 7. Enhanced MeGyro class with proper accelerometer sensitivity handling and raw data processing. |
| koekjesbakker |  2026/05/07 | 3.30.0 | 1. Added the `MeAudioPlayer` class back to the library. 2. Restored the `SimplePlayback` example. 3. Kept the Auriga-related integration changes included with the audio player update. |
| Nick B |  2026/10/05 | 3.31.0 | Added opt-in cascaded P/I/D position control while preserving legacy behavior by default, configurable position deadband (PWM cutoff inside the deadband in both modes, legacy P/D hold outside it), and an Auriga serial PID tuning example. |

# Issues

## Remove old MakeBlock library
If you have problem with the compiler complaining of ambiguous reference to `MePort`, you may have an old version of the MakeBlock library installed. You can remove it by uninstalling the official Makeblock library via the library manager.