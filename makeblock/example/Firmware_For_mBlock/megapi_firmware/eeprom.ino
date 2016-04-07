float RELAX_ANGLE = 0;     //�
void WriteBalancedDataToEEPROM(void)
{
  EEPROM.write(BALANCED_CAR_PARTITION_CHECK, EEPROM_IF_HAVEPID_CHECK1);
  EEPROM.write(BALANCED_CAR_PARTITION_CHECK + 1, EEPROM_IF_HAVEPID_CHECK2);
  EEPROM.write(BALANCED_CAR_START_ADDR, EEPROM_CHECK_START);

  EEPROM.put(BALANCED_CAR_NATURAL_BALANCE, RELAX_ANGLE);
  EEPROM.put(BALANCED_CAR_ANGLE_PID_ADDR, PID_angle.P);
  EEPROM.put(BALANCED_CAR_ANGLE_PID_ADDR+4, PID_angle.I);
  EEPROM.put(BALANCED_CAR_ANGLE_PID_ADDR+8, PID_angle.D);

  EEPROM.put(BALANCED_CAR_SPEED_PID_ADDR, PID_speed.P);
  EEPROM.put(BALANCED_CAR_SPEED_PID_ADDR+4, PID_speed.I);
  EEPROM.put(BALANCED_CAR_SPEED_PID_ADDR+8, PID_speed.D);

  EEPROM.put(BALANCED_CAR_DIR_PID_ADDR, PID_turn.P);
  EEPROM.write(BALANCED_CAR_END_ADDR, EEPROM_CHECK_END);

  EEPROM.write(AURIGA_MODE_START_ADDR, EEPROM_CHECK_START);
  EEPROM.write(BALANCED_CAR_SPEED_PID_ADDR, megapi_mode);
  EEPROM.write(AURIGA_MODE_END_ADDR, EEPROM_CHECK_END);
}

void WriteAurigaModeToEEPROM(void)
{
  EEPROM.write(MEGAPI_MODE_PARTITION_CHECK, EEPROM_IF_HAVEPID_CHECK1);
  EEPROM.write(MEGAPI_MODE_PARTITION_CHECK + 1, EEPROM_IF_HAVEPID_CHECK2);
  EEPROM.write(MEGAPI_MODE_START_ADDR, EEPROM_CHECK_START);
  EEPROM.write(MEGAPI_MODE_CONFIGURE, megapi_mode);
  EEPROM.write(MEGAPI_MODE_END_ADDR, EEPROM_CHECK_END);
}

int readEEPROM(void)
{
  if((EEPROM.read(BALANCED_CAR_PARTITION_CHECK) == EEPROM_IF_HAVEPID_CHECK1) && (EEPROM.read(BALANCED_CAR_PARTITION_CHECK + 1) == EEPROM_IF_HAVEPID_CHECK2))
  {
    if((EEPROM.read(BALANCED_CAR_START_ADDR)  == EEPROM_CHECK_START) && (EEPROM.read(BALANCED_CAR_END_ADDR)  == EEPROM_CHECK_END))
    {
      EEPROM.get(BALANCED_CAR_NATURAL_BALANCE, RELAX_ANGLE);
      EEPROM.get(BALANCED_CAR_ANGLE_PID_ADDR, PID_angle.P);
      EEPROM.get(BALANCED_CAR_ANGLE_PID_ADDR+4, PID_angle.I);
      EEPROM.get(BALANCED_CAR_ANGLE_PID_ADDR+8, PID_angle.D);

      EEPROM.get(BALANCED_CAR_SPEED_PID_ADDR, PID_speed.P);
      EEPROM.get(BALANCED_CAR_SPEED_PID_ADDR+4, PID_speed.I);
      EEPROM.get(BALANCED_CAR_SPEED_PID_ADDR+8, PID_speed.D);

      EEPROM.get(BALANCED_CAR_DIR_PID_ADDR, PID_turn.P);
#ifdef DEBUG_INFO
      Serial.println( "Read data from EEPROM:");
      Serial.print(RELAX_ANGLE);
      Serial.print( "  ");
      Serial.print(PID_angle.P);
      Serial.print( "  ");
      Serial.print(PID_angle.I);
      Serial.print( "  ");
      Serial.print(PID_angle.D);
      Serial.print( "  ");
      Serial.print(PID_speed.P);
      Serial.print( "  ");
      Serial.print(PID_speed.I);
      Serial.print( "  ");
      Serial.print(PID_speed.D);
      Serial.print( "  ");
      Serial.println(PID_turn.P);
#endif
    }
    else
    {
      Serial.println( "Data area damage on balanced car pid!" );
    }
  }
  else
  {
#ifdef DEBUG_INFO
    Serial.println( "First written Balanced data!" );
#endif
    WriteBalancedDataToEEPROM();
  }

  if((EEPROM.read(AURIGA_MODE_PARTITION_CHECK) == EEPROM_IF_HAVEPID_CHECK1) && (EEPROM.read(AURIGA_MODE_PARTITION_CHECK + 1) == EEPROM_IF_HAVEPID_CHECK2))
  {
    if((EEPROM.read(AURIGA_MODE_START_ADDR)  == EEPROM_CHECK_START) && (EEPROM.read(AURIGA_MODE_END_ADDR)  == EEPROM_CHECK_END))
    {
      EEPROM.get(AURIGA_MODE_CONFIGURE, megapi_mode);
#ifdef DEBUG_INFO
      Serial.print( "Read auriga_mode from EEPROM:");
      Serial.println(auriga_mode);
#endif
    }
    else
    {
      Serial.println( "Data area damage on auriga mode!" );
    }
  }
  else
  {
#ifdef DEBUG_INFO
    Serial.println( "First written auriga mode!" );
#endif
    WriteAurigaModeToEEPROM();
  }
}
