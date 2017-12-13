/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    MeMegaPiDCMotorTest.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2016/05/17
 * @brief   Description: this file is sample code for MegaPi DC motor device.
 *
 * Function List:
 *    1. void MeMegaPiDCMotorTest::run(int16_t speed)
 *    2. void MeMegaPiDCMotorTest::stop(void)
 *
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Zzipeng     2017/05/17    1.0.0          build the new
 * </pre>
 */
#include "MeMegaPiPro.h"

// MeStepperOnBoard stepper1(SLOT_1); 
// MeStepperOnBoard stepper2(SLOT_2); 
MeStepperOnBoard stepper3(SLOT_3); 
MeStepperOnBoard stepper4(SLOT_4); 

int i=0;
void setup()
{
   // Change these to suit your stepper if you want
  // stepper1.setMaxSpeed(3000);
  // stepper1.setAcceleration(20000);
  // stepper1.setMicroStep(4);
  // stepper1.enableOutputs();

  // stepper2.setMaxSpeed(3000);
  // stepper2.setAcceleration(20000);
  // stepper2.setMicroStep(4);
  // stepper2.enableOutputs();

  stepper3.setMaxSpeed(3000);
  stepper3.setAcceleration(20000);
  stepper3.setMicroStep(4);
  stepper3.enableOutputs();

  stepper4.setMaxSpeed(3000);
  stepper4.setAcceleration(20000);
  stepper4.setMicroStep(4);
  stepper4.enableOutputs();
}

void loop()
{
    while(1)
    {
      // stepper1.move(2000);
      // stepper2.move(2000);
      stepper3.move(2000);
      stepper4.move(2000);
      for(i=1000;i>0;i--)
      {
         // stepper1.run();
         // stepper2.run();
         stepper3.run();
         stepper4.run();
         delay(1);
      }
    }
}

