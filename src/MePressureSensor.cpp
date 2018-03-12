#include "MePressureSensor.h"

MePressureSensor::MePressureSensor() {
}


boolean MePressureSensor::begin(uint8_t mode) {
  if (mode > BMP180_ULTRAHIGHRES) 
    mode = BMP180_ULTRAHIGHRES;
  oversampling = mode;

  Wire.begin();

  if (readUInt8(0xD0) != 0x55) return false;

  ac1 = readUInt16(BMP180_CAL_AC1);
  ac2 = readUInt16(BMP180_CAL_AC2);
  ac3 = readUInt16(BMP180_CAL_AC3);
  ac4 = readUInt16(BMP180_CAL_AC4);
  ac5 = readUInt16(BMP180_CAL_AC5);
  ac6 = readUInt16(BMP180_CAL_AC6);

  b1 = readUInt16(BMP180_CAL_B1);
  b2 = readUInt16(BMP180_CAL_B2);

  mb = readUInt16(BMP180_CAL_MB);
  mc = readUInt16(BMP180_CAL_MC);
  md = readUInt16(BMP180_CAL_MD);
  return true;
}

int32_t MePressureSensor::computeB5(int32_t UT) {
  int32_t X1 = (UT - (int32_t)ac6) * ((int32_t)ac5) >> 15;
  int32_t X2 = ((int32_t)mc << 11) / (X1+(int32_t)md);
  return X1 + X2;
}

uint16_t MePressureSensor::readRawTemperature(void) {
  writeUInt8(BMP180_CONTROL, BMP180_READTEMPCMD);
  delay(5);
  return readUInt16(BMP180_TEMPDATA);
}

uint32_t MePressureSensor::readRawPressure(void) {
  uint32_t raw;

  writeUInt8(BMP180_CONTROL, BMP180_READPRESSURECMD + (oversampling << 6));

  if (oversampling == BMP180_ULTRALOWPOWER) 
    delay(5);
  else if (oversampling == BMP180_STANDARD) 
    delay(8);
  else if (oversampling == BMP180_HIGHRES) 
    delay(14);
  else 
    delay(26);

  raw = readUInt16(BMP180_PRESSUREDATA);

  raw <<= 8;
  raw |= readUInt8(BMP180_PRESSUREDATA+2);
  raw >>= (8 - oversampling);
  return raw;
}


int32_t MePressureSensor::readPressure(void) {
  int32_t UT, UP, B3, B5, B6, X1, X2, X3, p;
  uint32_t B4, B7;

  UT = readRawTemperature();
  UP = readRawPressure();
  B5 = computeB5(UT);
  B6 = B5 - 4000;
  X1 = ((int32_t)b2 * ( (B6 * B6)>>12 )) >> 11;
  X2 = ((int32_t)ac2 * B6) >> 11;
  X3 = X1 + X2;
  B3 = ((((int32_t)ac1*4 + X3) << oversampling) + 2) / 4;
  X1 = ((int32_t)ac3 * B6) >> 13;
  X2 = ((int32_t)b1 * ((B6 * B6) >> 12)) >> 16;
  X3 = ((X1 + X2) + 2) >> 2;
  B4 = ((uint32_t)ac4 * (uint32_t)(X3 + 32768)) >> 15;
  B7 = ((uint32_t)UP - B3) * (uint32_t)( 50000UL >> oversampling );
  if (B7 < 0x80000000) {
    p = (B7 * 2) / B4;
  } else {
    p = (B7 / B4) * 2;
  }
  X1 = (p >> 8) * (p >> 8);
  X1 = (X1 * 3038) >> 16;
  X2 = (-7357 * p) >> 16;

  p = p + ((X1 + X2 + (int32_t)3791)>>4);
  return p;
}

int32_t MePressureSensor::readSealevelPressure(float altitude_meters) {
  float pressure = readPressure();
  return (int32_t)(pressure / pow(1.0-altitude_meters/44330, 5.255));
}

float MePressureSensor::readTemperature(void) {
  int32_t UT, B5; 
  float temp;

  UT = readRawTemperature();

  B5 = computeB5(UT);
  temp = (B5+8) >> 4;
  temp /= 10;
  
  return temp;
}

float MePressureSensor::readAltitude(float sealevelPressure) {
  float altitude;

  float pressure = readPressure();

  altitude = 44330 * (1.0 - pow(pressure /sealevelPressure,0.1903));

  return altitude;
}

uint8_t MePressureSensor::readUInt8(uint8_t a) {
  uint8_t ret;

  Wire.beginTransmission(BMP180_I2CADDR); 
#if (ARDUINO >= 100)
  Wire.write(a); 
#else
  Wire.send(a); 
#endif
  Wire.endTransmission(); 
  Wire.beginTransmission(BMP180_I2CADDR); 
  Wire.requestFrom(BMP180_I2CADDR, 1);
#if (ARDUINO >= 100)
  ret = Wire.read(); 
#else
  ret = Wire.receive();
#endif
  Wire.endTransmission();

  return ret;
}

uint16_t MePressureSensor::readUInt16(uint8_t a) {
  uint16_t ret;

  Wire.beginTransmission(BMP180_I2CADDR); 
#if (ARDUINO >= 100)
  Wire.write(a); 
#else
  Wire.send(a);
#endif
  Wire.endTransmission(); 
  
  Wire.beginTransmission(BMP180_I2CADDR); 
  Wire.requestFrom(BMP180_I2CADDR, 2);
#if (ARDUINO >= 100)
  ret = Wire.read(); 
  ret <<= 8;
  ret |= Wire.read();
#else
  ret = Wire.receive();
  ret <<= 8;
  ret |= Wire.receive();
#endif
  Wire.endTransmission(); 

  return ret;
}

void MePressureSensor::writeUInt8(uint8_t a, uint8_t d) {
  Wire.beginTransmission(BMP180_I2CADDR); 
#if (ARDUINO >= 100)
  Wire.write(a); 
  Wire.write(d);
#else
  Wire.send(a); 
  Wire.send(d); 
#endif
  Wire.endTransmission();
}
