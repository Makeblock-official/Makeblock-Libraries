void encodersInit()
{
  encoders[0].reset(SLOT1);
  encoders[1].reset(SLOT2);
  encoders[2].reset(SLOT3);
  encoders[3].reset(SLOT4);
  attachInterrupt(encoders[0].getIntNum(), isr_process_encoder1, RISING);
  attachInterrupt(encoders[1].getIntNum(), isr_process_encoder2, RISING);
  attachInterrupt(encoders[2].getIntNum(), isr_process_encoder3, RISING);
  attachInterrupt(encoders[3].getIntNum(), isr_process_encoder4, RISING);
  
  measurement_speed_time = lasttime_speed = lasttime_angle = millis();
  encoders[0].setPulsePos(0);
  encoders[1].setPulsePos(0);
  encoders[2].setPulsePos(0);
  encoders[3].setPulsePos(0);
  PID_speed_left.Setpoint = 0;
  PID_speed_right.Setpoint = 0;
}
void encodersUpdate()
{
  for(int i=0;i<4;i++)
  {
    encoders[i].loop();
  }
}
void onEncoderMovingFinish(int slot,int extId)
{
  writeHead();
  writeSerial(extId);
  sendByte(slot);
  writeEnd();
}
void PWM_Calcu()
{
  double speed1;
  double speed2;
  if((millis() - lasttime_speed) > 20)
  {
    speed1 = encoders[0].getCurrentSpeed();
    speed2 = encoders[1].getCurrentSpeed();

#ifdef DEBUG_INFO
    Serial.print("S1: ");
    Serial.print(speed1);
    Serial.print(" S2: ");
    Serial.print(speed2);
    Serial.print("left: ");
    Serial.print(PID_speed_left.Setpoint);
    Serial.print(" right: ");
    Serial.println(PID_speed_right.Setpoint);
#endif

    if(abs(abs(PID_speed_left.Setpoint) - abs(PID_speed_right.Setpoint)) >= 0)
    {
      encoders[0].setMotorPwm(PID_speed_left.Setpoint);
      encoders[1].setMotorPwm(PID_speed_right.Setpoint);
      return;
    }

    if((abs(PID_speed_left.Setpoint) == 0) && (abs(PID_speed_right.Setpoint) == 0))
    {
      return;
    }

    if(abs(speed1) - abs(speed2) >= 0)
    {
      if(PID_speed_left.Setpoint > 0)
      {
        encoders[0].setMotorPwm(PID_speed_left.Setpoint - (abs(speed1) - abs(speed2)));
        encoders[1].setMotorPwm(PID_speed_right.Setpoint);
      }
      else
      {
        encoders[0].setMotorPwm(PID_speed_left.Setpoint + (abs(speed1) - abs(speed2)));
        encoders[1].setMotorPwm(PID_speed_right.Setpoint);
      }
    }
    else
    {
      if(PID_speed_right.Setpoint > 0)
      {
        encoders[0].setMotorPwm(PID_speed_left.Setpoint);
        encoders[1].setMotorPwm(PID_speed_right.Setpoint - (abs(speed2) - abs(speed1)));
      }
      else
      {
        encoders[0].setMotorPwm(PID_speed_left.Setpoint);
        encoders[1].setMotorPwm(PID_speed_right.Setpoint + (abs(speed2) - abs(speed1)));
      }
    }
    lasttime_speed = millis(); 
  }
}

void Forward(void)
{
//  Encoder_1.setMotorPwm(-moveSpeed);
//  Encoder_2.setMotorPwm(moveSpeed);
  PID_speed_left.Setpoint = -moveSpeed;
  PID_speed_right.Setpoint = moveSpeed;
  move_status = MOVE_FORWARD;
  PWM_Calcu();
}

void Backward(void)
{
//  Encoder_1.setMotorPwm(moveSpeed);
//  Encoder_2.setMotorPwm(-moveSpeed);
  PID_speed_left.Setpoint = moveSpeed;
  PID_speed_right.Setpoint = -moveSpeed;
  move_status = MOVE_BACKWARD;
  PWM_Calcu();
}

void Stop(void)
{
  encoders[0].setMotorPwm(0);
  encoders[1].setMotorPwm(0);
  move_status = MOVE_STOP;
}
void isr_process_encoder1(void)
{
  if(digitalRead(encoders[0].getPortB()) == 0)
  {
    encoders[0].pulsePosMinus();
  }
  else
  {
    encoders[0].pulsePosPlus();
  }
}

void isr_process_encoder2(void)
{
  if(digitalRead(encoders[1].getPortB()) == 0)
  {
    encoders[1].pulsePosMinus();
  }
  else
  {
    encoders[1].pulsePosPlus();
  }
}

void isr_process_encoder3(void)
{
  if(digitalRead(encoders[2].getPortB()) == 0)
  {
    encoders[2].pulsePosMinus();
  }
  else
  {
    encoders[2].pulsePosPlus();
  }
}

void isr_process_encoder4(void)
{
  if(digitalRead(encoders[3].getPortB()) == 0)
  {
    encoders[3].pulsePosMinus();
  }
  else
  {
    encoders[3].pulsePosPlus();
  }
}
