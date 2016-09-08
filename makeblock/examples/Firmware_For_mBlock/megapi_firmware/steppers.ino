//blue red green black
//A+ A- B+ B-
void steppersInit()
{
  steppers[0] = MeStepperOnBoard(SLOT1);
  steppers[1] = MeStepperOnBoard(SLOT2);
  steppers[2] = MeStepperOnBoard(SLOT3);
  steppers[3] = MeStepperOnBoard(SLOT4);
  for(int i=0;i<4;i++){
    steppers[i].setMaxSpeed(10000);
    steppers[i].setAcceleration(10000);
    steppers[i].setMicroStep(16);
  }
}
void steppersUpdate()
{
  for(int i=0;i<4;i++){
    steppers[i].update();
  }
}
void onStepperMovingFinish(int slot,int extId)
{
  writeHead();
  writeSerial(extId);
  sendByte(slot);
  writeEnd();
}

