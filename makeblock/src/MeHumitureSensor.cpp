/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class MeHumiture
 * \brief   Driver for humiture sensor device.
 * @file    MeHumitureSensor.cpp
 * @author  MakeBlock
 * @version V1.0.1
 * @date    2015/11/18
 * @brief   Driver for humiture sensor device.
 *
 * \par Copyright
 * This software is Copyright (C), 2012-2016, MakeBlock. Use is subject to license \n
 * conditions. The main licensing options available are GPL V2 or Commercial: \n
 *
 * \par Open Source Licensing GPL V2
 * This is the appropriate option if you want to share the source code of your \n
 * application with everyone you distribute it to, and you also want to give them \n
 * the right to share who uses it. If you wish to use this software under Open \n
 * Source Licensing, you must contribute all your source code to the open source \n
 * community in accordance with the GPL Version 2 when your application is \n
 * distributed. See http://www.gnu.org/copyleft/gpl.html
 *
 * \par Description
 * This file is a drive for humiture sensor device, It supports humiture sensor
 * provided by the MakeBlock.
 *
 * \par Method List:
 *
 *    1. void MeHumiture::setpin(uint8_t port)
 *    2. void MeHumiture::update(void)
 *    3. uint8_t MeHumiture::getHumidity(void)
 *    4. uint8_t MeHumiture::getTemperature(void)
 *    5. uint8_t MeHumiture::getValue(uint8_t index)
 *    6. double MeHumiture::getFahrenheit(void)
 *    7. double MeHumiture::getKelvin(void)
 *    8. double MeHumiture::getdewPoint(void)
 *    9. double MeHumiture::getPointFast()
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/09/08     1.0.0            Rebuild the old lib.
 * forfish          2015/11/18     1.0.1            Add some functions.
 * </pre>
 *
 * @example MeHumitureSensorTest1.ino
 * @example MeHumitureSensorTest2.ino
 */
#include "MeHumitureSensor.h"

#ifdef ME_PORT_DEFINED
/**
 * Alternate Constructor which can call your own function to map the humiture sensor to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
MeHumiture::MeHumiture(void) : MePort(0)
{

}

/**
 * Alternate Constructor which can call your own function to map the temperature sensor to arduino port,
 * the slot2 pin will be used here since specify slot is not be set.
 * \param[in]
 *   port - RJ25 port from PORT_1 to M2
 */
MeHumiture::MeHumiture(uint8_t port) : MePort(port)
{

}
#else // ME_PORT_DEFINED
/**
 * \par Function
 *   setpin
 * \par Description
 *   Reset available PIN for temperature sensor by its arduino port.
 * \param[in]
 *   port - arduino port(should digital pin)
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
MeHumiture::MeHumiture(uint8_t port)
{
  _DataPin = port;
}
#endif // ME_PORT_DEFINED

/**
 * \par Function
 *   setpin
 * \par Description
 *   Reset available PIN for temperature sensor by its arduino port.
 * \param[in]
 *   port - arduino port(should digital pin)
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
void MeHumiture::setpin(uint8_t port)
{
  _DataPin = port;
  s2 = _DataPin;
}

/**
 * \par Function
 *   update
 * \par Description
 *   Use this function to update the sensor data
 * \par Output
 *   None
 * \return
 *   None
 * \par Others
 *   None
 */
void MeHumiture::update(void)
{
  uint8_t data[5] = {0};
  unsigned long Time, datatime;
  
#ifdef ME_PORT_DEFINED
  MePort::dWrite2(LOW);
  delay(20);
  MePort::dWrite2(HIGH);
  delayMicroseconds(40);
  MePort::dWrite2(LOW);
#else // ME_PORT_DEFINED
  pinMode(_DataPin,OUTPUT);
  digitalWrite(_DataPin,LOW);
  delay(20);
  digitalWrite(_DataPin,HIGH);
  delayMicroseconds(40);
  digitalWrite(_DataPin,LOW);
#endif // ME_PORT_DEFINED
  Time = millis();
#ifdef ME_PORT_DEFINED
  while(MePort::dRead2() != HIGH)
#else // ME_PORT_DEFINED
  pinMode(_DataPin,INPUT);
  while(digitalRead(_DataPin) != HIGH)
#endif // ME_PORT_DEFINED
  {
    if( ( millis() - Time ) > 2)
    {
      Humidity = 0;
      Temperature = 0;
      break;
    }
  }

  Time = millis();
  
#ifdef ME_PORT_DEFINED
  while(MePort::dRead2() != LOW)
#else // ME_PORT_DEFINED
  pinMode(_DataPin,INPUT);
  while(digitalRead(_DataPin) != LOW)
#endif // ME_PORT_DEFINED
  {
    if( ( millis() - Time ) > 2)
    {
      break;
    }
  }

  for(int16_t i=0;i<40;i++)
  {
  	Time = millis();
#ifdef ME_PORT_DEFINED
    while(MePort::dRead2() == LOW)
#else // ME_PORT_DEFINED
    pinMode(_DataPin,INPUT);
    while(digitalRead(_DataPin) == LOW)
#endif // ME_PORT_DEFINED
    {
      if( ( millis() - Time ) > 2)
      {
        break;
      }
    }

    datatime = micros();
    Time = millis();
#ifdef ME_PORT_DEFINED
    while(MePort::dRead2() == HIGH)
#else // ME_PORT_DEFINED
    pinMode(_DataPin,INPUT);
    while(digitalRead(_DataPin) == HIGH)
#endif // ME_PORT_DEFINED
    {
      if( ( millis() - Time ) > 2 )
      {
        break;
      }
    }

    if ( micros() - datatime > 40 )
    {
      data[i/8] <<= 1;
      data[i/8] |= 0x01;
    }
    else
    {
      data[i/8] <<= 1;
    }
  }
   
  if( (data[0] + data[2]) == data[4] )
  {
  	Humidity = data[0];
    Temperature = data[2];
  }
}

/**
 * \par Function
 *   getHumidity
 * \par Description
 *   Use this function to Get the Humidity data
 * \par Output
 *   None
 * \return
 *   The value of Humidity
 * \par Others
 *   None
 */
uint8_t MeHumiture::getHumidity(void)
{
  return Humidity;
}

/**
 * \par Function
 *   getTemperature
 * \par Description
 *   Use this function to Get the Temperature data
 * \par Output
 *   None
 * \return
 *   The value of Temperature
 * \par Others
 *   None
 */
uint8_t MeHumiture::getTemperature(void)
{
  return Temperature;
}

/**
 * \par Function
 *   getValue
 * \par Description
 *   Use this function to Get the Temperature data or Humidity data
 * \param[in]
 *   index - The value '0' means get the value of Humidity data and '1' used to get the
 *   Temperature data.
 * \par Output
 *   None
 * \return
 *   The value of Temperature
 * \par Others
 *   None
 */
uint8_t MeHumiture::getValue(uint8_t index)
{
  if(index == 0)
  {
    return Humidity;
  }
  else
  {
    return Temperature;
  }
}

/**
 * \par Function
 *    Fahrenheit
 * \par Description
 *    Change celsius degrees into Fahrenheit.
 * \param[in]
 *    celsius - The number of celsius degrees.
 * \par Output
 *    None
 * \par Return
 *    Return the number of Fahrenheit
 * \par Others
 *    None
 */
double MeHumiture::getFahrenheit(void)//Celsius degrees to Fahrenheit
{
  return 1.8 * Temperature + 32;
}


/**
 * \par Function
 *    Kelvin
 * \par Description
 *    Change celsius degrees into Kelvin.
 * \param[in]
 *    celsius - The number of celsius degrees.
 * \par Output
 *    None
 * \par Return
 *    Return the number of Kelvin temperature.
 * \par Others
 *    None
 */
double MeHumiture::getKelvin(void)
{
  return Temperature + 273.15;
}

/**
 * \par Function
 *    dewPoint
 * \par Description
 *    The dew-point temperature (Point at this temperature, the air is saturated and produce dew).
 * \param[in]
 *    celsius - The celsius degrees of air.
  * \param[in]
 *    humidity - The humidity of air.
 * \par Output
 *    None
 * \par Return
 *    Return the dew-point of air.
 * \par Others
 *    None
 */
double MeHumiture::getdewPoint(void)
{
  double A0= 373.15/(273.15 + Temperature);
  double SUM = -7.90298 * (A0-1);
  SUM += 5.02808 * log10(A0);
  SUM += -1.3816e-7 * (pow(10, (11.344*(1-1/A0)))-1) ;
  SUM += 8.1328e-3 * (pow(10,(-3.49149*(A0-1)))-1) ;
  SUM += log10(1013.246);
  double VP = pow(10, SUM-3) * Humidity;
  double T = log(VP/0.61078);   // temp var
  return (241.88 * T) / (17.558-T);
}

/**
 * \par Function
 *    dewPointFast
 * \par Description
 *    Fast calculating dew point, peed is 5 times to dewPoint().
 * \param[in]
 *    celsius - The celsius degrees of air.
  * \param[in]
 *    humidity - The humidity of air.
 * \par Output
 *    None
 * \par Return
 *    Return the Fast calculating dew point of air.
 * \par Others
 *    None
 */
double MeHumiture::getPointFast()
{
  double a = 17.271;
  double b = 237.7;
  double temp = (a * Temperature) / (b + Temperature) + log(Humidity/100);
  //double Td = (b * temp) / (a - temp);
  return ((b * temp) / (a - temp));
}

