/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MeIR
 * \brief   Driver for Me IR module.
 * @file    MeIR.cpp
 * @author  MakeBlock
 * @version V1.0.4
 * @date    2015/11/16
 * @brief   Driver for Me IR module.
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
 * This file is a drive for Me IR device, The IR inherited the 
 * MeSerial class from SoftwareSerial.
 *
 * \par Method List:
 *
 *    1. ErrorStatus MeIR::decode();
 *    2. void MeIR::begin();
 *    3. void MeIR::end();
 *    4. void MeIR::loop();
 *    5. boolean MeIR::keyPressed(unsigned char r);
 *    6. String MeIR::getString();
 *    7. unsigned char MeIR::getCode()
 *    8. void MeIR::sendString(String s);
 *    9. void MeIR::sendString(float v);
 *    10. void MeIR::sendNEC(unsigned long data, int nbits);
 *    11. void MeIR::sendRaw(unsigned int buf[], int len, uint8_t hz);
 *    12. void MeIR::enableIROut(uint8_t khz);
 *    13. void MeIR::enableIRIn();
 *    14. void MeIR::mark(uint16_t us);
 *    15. void MeIR::space(uint16_t us);
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan        2015/10/09     1.0.0            Bulid the new.
 * Mark Yan        2015/10/29     1.0.1            Fix minor errors on format.
 * Mark Yan        2015/11/02     1.0.2            Fix bug that IRsend and IRreceive can't work at the same time.
 * forfish         2015/11/09     1.0.3            Add description.
 * Mark Yan        2015/11/16     1.0.4            add data recovery when timeout.
 * </pre>
 *
 */
 
#include "MeIR.h"

// Provides ISR
#ifndef __AVR_ATmega32U4__
#include <avr/interrupt.h>

volatile irparams_t irparams;
bool MATCH(uint8_t measured_ticks, uint8_t desired_us)
{
  // Serial.print(measured_ticks);Serial.print(",");Serial.println(desired_us);
  return(measured_ticks >= desired_us - (desired_us>>2)-1 && measured_ticks <= desired_us + (desired_us>>2)+1);//判断前后25%的误差
}

ISR(TIMER_INTR_NAME)
{
  uint8_t irdata = (uint8_t)digitalRead(2);
  // uint32_t new_time = micros();
  // uint8_t timer = (new_time - irparams.lastTime)>>6;
  irparams.timer++; // One more 50us tick
  if (irparams.rawlen >= RAWBUF)
  {
    // Buffer overflow
    irparams.rcvstate = STATE_STOP;
  }
  switch(irparams.rcvstate)
  {
    case STATE_IDLE: // In the middle of a gap
      if (irdata == MARK)
      {
        irparams.rawlen = 0;
        irparams.timer = 0;
        irparams.rcvstate = STATE_MARK;
      }
      break;
    case STATE_MARK: // timing MARK
      if (irdata == SPACE)
      {
        // MARK ended, record time
        irparams.rawbuf[irparams.rawlen++] = irparams.timer;
        irparams.timer = 0;
        irparams.rcvstate = STATE_SPACE;
      }
      break;
    case STATE_SPACE: // timing SPACE
      if (irdata == MARK)
      {
        // SPACE just ended, record it
        irparams.rawbuf[irparams.rawlen++] = irparams.timer;
        irparams.timer = 0;
        irparams.rcvstate = STATE_MARK;
      }
      else
      { // SPACE
        if (irparams.timer > GAP_TICKS)
        {
          // big SPACE, indicates gap between codes
          // Mark current code as ready for processing
          // Switch to STOP
          // Don't reset timer; keep counting space width
          irparams.rcvstate = STATE_STOP;
		  irparams.lastTime = millis();
        } 
      }
      break;
    case STATE_STOP: // waiting, measuring gap
      if(millis() - irparams.lastTime > 200)
      {
        irparams.rawlen = 0;
        irparams.timer = 0;
        irparams.rcvstate = STATE_IDLE;
      }
      else if (irdata == MARK)
      { 
        // reset gap timer
        irparams.timer = 0;
      }
      break;
  }
  // irparams.lastTime = new_time;
}

/**
 * Alternate Constructor which can call your own function to map the IR to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
MeIR::MeIR()
{
  pinMode(2,INPUT);
  irparams.recvpin = 2;
  // attachInterrupt(INT0, irISR, CHANGE);
  
  irDelayTime = 0;
  irIndex = 0;
  irRead = 0;
  irReady = false;
  irBuffer = "";
  irPressed = false;
  begin();
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW); // When not sending PWM, we want it low
}

/**
 * \par Function
 *    begin
 * \par Description
 *    Initialize interrupt.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeIR::begin()
{
  cli();
  // setup pulse clock timer interrupt
  //Prescale /8 (16M/8 = 0.5 microseconds per tick)
  // Therefore, the timer interval can range from 0.5 to 128 microseconds
  // depending on the reset value (255 to 0)
  TIMER_CONFIG_NORMAL();

  //Timer2 Overflow Interrupt Enable
  TIMER_ENABLE_INTR;

  // TIMER_RESET;

  sei();  // enable interrupts

  // initialize state machine variables
  irparams.rawlen = 0;
  irparams.rcvstate = STATE_IDLE;

  // set pin modes
  // pinMode(2, INPUT);
  // pinMode(irparams.recvpin, INPUT);
}

/**
 * \par Function
 *    end
 * \par Description
 *    Close the interrupt.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeIR::end()
{
  EIMSK &= ~(1 << INT0);
}

/**
 * \par Function
 *    decode
 * \par Description
 *    Decodes the received IR message.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    Returns 0 if no data ready, 1 if data ready.
 * \par Others
 *    Results of decoding are stored in results.
 */
ErrorStatus MeIR::decode()
{
  rawbuf = irparams.rawbuf;
  rawlen = irparams.rawlen;
  if (irparams.rcvstate != STATE_STOP)
  {
    return ERROR;
  }

  if (decodeNEC())
  {
    begin();
    return SUCCESS;
  }
  begin();
  return ERROR;
}

/**
 * \par Function
 *    decodeNEC
 * \par Description
 *    Decodes NEC the received IR message.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    Returns ERROR if decode NEC no done, SUCCESS if decode NEC done.
 * \par Others
 *    Results of decode NEC.
 */
// NECs have a repeat only 4 items long
ErrorStatus MeIR::decodeNEC()
{
  uint32_t data = 0;
  int offset = 0; // Skip first space
  // Initial mark
  if (!MATCH(rawbuf[offset], NEC_HDR_MARK/50)) 
  {
    return ERROR;
  }
  offset++;
  // Check for repeat
  if (rawlen == 3 &&
    MATCH(rawbuf[offset], NEC_RPT_SPACE/50) &&
    MATCH(rawbuf[offset+1], NEC_BIT_MARK/50)) 
  {
    bits = 0;
    // results->value = REPEAT;
	// Serial.println("REPEAT");
	decode_type = NEC;
    return SUCCESS;
  }
  if (rawlen < (2 * NEC_BITS + 3)) 
  {
    return ERROR;
  }
  // Initial space  
  if (!MATCH(rawbuf[offset], NEC_HDR_SPACE/50)) 
  {
    return ERROR;
  }
  offset++;
  for (int i = 0; i < NEC_BITS; i++)
  {
    if (!MATCH(rawbuf[offset], NEC_BIT_MARK/50)) 
	{
      return ERROR;
    }
    offset++;
    if (MATCH(rawbuf[offset], NEC_ONE_SPACE/50))
	{
      //data = (data << 1) | 1;
      data = (data >> 1) | 0x80000000;
    } 
    else if (MATCH(rawbuf[offset], NEC_ZERO_SPACE/50))
	{
      //data <<= 1;
      data >>= 1;
    } 
    else 
	{
      return ERROR;
    }
    offset++;
  }
  // Success
  bits = NEC_BITS;
  value = data;
  decode_type = NEC;
  return SUCCESS;
}

/**
 * \par Function
 *    mark
 * \par Description
 *    Sends an IR mark for the specified number of microseconds.
 * \param[in]
 *    us - THe time of a PWM.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeIR::mark(uint16_t us)
{
  // Sends an IR mark for the specified number of microseconds.
  // The mark output is modulated at the PWM frequency.
  TIMER_ENABLE_PWM; // Enable pin 3 PWM output
  delayMicroseconds(us);
}

/**
 * \par Function
 *    space
 * \par Description
 *    Sends an IR mark for the specified number of microseconds.
 * \param[in]
 *    us - THe time of a PWM.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
/* Leave pin off for time (given in microseconds) */
void MeIR::space(uint16_t us)
{
  // Sends an IR space for the specified number of microseconds.
  // A space is no output, so the PWM output is disabled.
  TIMER_DISABLE_PWM; // Disable pin 3 PWM output
  delayMicroseconds(us);
}

/**
 * \par Function
 *    enableIROut
 * \par Description
 *    Enable an IR for the specified number of khz.
 * \param[in]
 *    us - THe time of a INTR.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeIR::enableIROut(uint8_t khz)
{
  TIMER_DISABLE_INTR; //Timer2 disable Interrupt
  TIMER_CONFIG_KHZ(khz);
}

/**
 * \par Function
 *    enableIRIn
 * \par Description
 *    Enable an IR to write in.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
// initialization
void MeIR::enableIRIn() {
  cli();
  // setup pulse clock timer interrupt
  //Prescale /8 (16M/8 = 0.5 microseconds per tick)
  // Therefore, the timer interval can range from 0.5 to 128 microseconds
  // depending on the reset value (255 to 0)
  TIMER_CONFIG_NORMAL();

  //Timer2 Overflow Interrupt Enable
  TIMER_ENABLE_INTR;

  //TIMER_RESET;

  sei();  // enable interrupts

  // initialize state machine variables
  irparams.rcvstate = STATE_IDLE;
  irparams.rawlen = 0;

  // set pin modes
  pinMode(irparams.recvpin, INPUT);
}

/**
 * \par Function
 *    sendRaw
 * \par Description
 *    Send the length of data with hz.
 * \param[in]
 *    buf[] - The data's buffer.
  * \param[in]
 *    len - The data's length.
  * \param[in]
 *    hz - The hz for sending data.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeIR::sendRaw(unsigned int buf[], int len, uint8_t hz)
{
  enableIROut(hz);
  for (int i = 0; i < len; i++)
  {
    if (i & 1)
    {
      space(buf[i]);
    } 
    else
    {
      mark(buf[i]);
    }
  }
  space(0); // Just to be sure
}

/**
 * \par Function
 *    getString
 * \par Description
 *    Get string in a INTR.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    Return the result in a IRQ.
 * \par Others
 *    None
 */
String MeIR::getString()
{
  if(decode())
  {
    irRead = ((value >> 8) >> 8) & 0xff;
    if(irRead == 0xa || irRead == 0xd)
    {
      irIndex = 0;
      irReady = true;
    }
    else
    {
      irBuffer += irRead; 
      irIndex++;
    }
    irDelayTime = millis();
  }
  else
  {
    if(irRead > 0)
    {
      if(millis() - irDelayTime > 100)
      {
        irPressed = false;
        irRead = 0;
        irDelayTime = millis();
		Pre_Str = "";
      }
    }
  }
  if(irReady)
  {
    irReady = false;
    String s = String(irBuffer);
	Pre_Str = s;
    irBuffer = "";
    return s;
  }
  return Pre_Str;
}

/**
 * \par Function
 *    getCode
 * \par Description
 *    Get the reading code.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    Return the result of reading.
 * \par Others
 *    None
 */
unsigned char MeIR::getCode(){
  irIndex = 0;
  loop();
  return irRead;
}

/**
 * \par Function
 *    sendString
 * \par Description
 *    Send data.
 * \param[in]
 *    s - The string you want to send.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeIR::sendString(String s)
{
  unsigned long l;
  uint8_t data;
  s.concat('\n');
  for(int i = 0;i < s.length();i++)
  {
    data = s.charAt(i);
    l = 0x0000ffff & (uint8_t)(~data);
	l = l << 8;
    l = l + ((uint8_t)data);
    l = l << 16;
	l = l | 0x000000ff;
    sendNEC(l,32);
    delay(20);
  }
  enableIRIn();
}

/**
 * \par Function
 *    sendString
 * \par Description
 *    Send data.
 * \param[in]
 *    v - The string you want to send.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeIR::sendString(float v)
{
  dtostrf(v,5, 2, floatString);
  sendString(floatString);
}

/**
 * \par Function
 *    sendNEC
 * \par Description
 *    Send NEC.
 * \param[in]
 *    data - The data you want to send.
  * \param[in]
 *    nbits - The data bit you want to send.
 * \par Output
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeIR::sendNEC(unsigned long data, int nbits)
{
  enableIROut(38);
  mark(NEC_HDR_MARK);
  space(NEC_HDR_SPACE);
  for (int i = 0; i < nbits; i++)
  {
    if (data & 1) 
    {
      mark(NEC_BIT_MARK);
      space(NEC_ONE_SPACE);
    } 
    else 
    {
      mark(NEC_BIT_MARK);
      space(NEC_ZERO_SPACE);
    }
    data >>= 1;
  }
  mark(NEC_BIT_MARK);
  space(0);
}

/**
 * \par Function
 *    loop
 * \par Description
 *    A circle of operation.
 * \param[in]
 *    None
 * \par Output0
 *    None
 * \par Return
 *    None
 * \par Others
 *    None
 */
void MeIR::loop()
{
  if(decode())
  {
    irRead = ((value >> 8) >> 8) & 0xff;
    irPressed = true;
    if(irRead == 0xa || irRead == 0xd)
    {
      irIndex = 0;
      irReady = true;
    }
    else
    {
      irBuffer += irRead; 
      irIndex++;
      if(irIndex > 64)
      {
        irIndex = 0;
        irBuffer = "";
      }
    }
    irDelayTime =  millis();
  }
  else
  {
    if(irRead > 0)
    {
      if(millis() - irDelayTime > 200)
      {
        irPressed = false;
        irRead = 0;
        irDelayTime = millis();
      }
    }
  }
}

/**
 * \par Function
 *    keyPressed
 * \par Description
 *    Press key.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    Return you the pressed key or not.
 * \par Others
 *    None
 */
boolean MeIR::keyPressed(unsigned char r)
{
  irIndex = 0;
  loop();
  return irRead == r;
}
#endif // !defined(__AVR_ATmega32U4__)
