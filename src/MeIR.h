/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MeIR
 * \brief   Driver for Me IR module.
 * @file    MeIR.h
 * @author  MakeBlock
 * @version V1.0.4
 * @date    2015/11/16
 * @brief   Header for MeIR.cpp module
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
 
#ifndef MeIR_h
#define MeIR_h

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>
#include "MeConfig.h"
#ifdef ME_PORT_DEFINED
#include "MePort.h"
#endif // ME_PORT_DEFINED

#ifndef __AVR_ATmega32U4__
#define MARK  0
#define SPACE 1
#define NEC_BITS 32

#define USECPERTICK 50  // microseconds per clock interrupt tick
#define RAWBUF 100 // Length of raw duration buffer

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

#define NEC_HDR_MARK	9000
#define NEC_HDR_SPACE	4500
#define NEC_BIT_MARK	560
#define NEC_ONE_SPACE	1600
#define NEC_ZERO_SPACE	560
#define NEC_RPT_SPACE	2250
#define NEC_RPT_PERIOD	110000


// #define TOLERANCE 25  // percent tolerance in measurements
// #define LTOL (1.0 - TOLERANCE/100.) 
// #define UTOL (1.0 + TOLERANCE/100.) 

#define _GAP 5000 // Minimum map between transmissions
// #define GAP_TICKS 5000//(_GAP/USECPERTICK)

// #define TICKS_LOW(us) (int) (((us)*LTOL/USECPERTICK))
// #define TICKS_HIGH(us) (int) (((us)*UTOL/USECPERTICK + 1))

// receiver states
#define STATE_IDLE     2
#define STATE_MARK     3
#define STATE_SPACE    4
#define STATE_STOP     5


// Values for decode_type
#define NEC 1
#define SONY 2
#define RC5 3
#define RC6 4
#define DISH 5
#define SHARP 6
#define PANASONIC 7
#define JVC 8
#define SANYO 9
#define MITSUBISHI 10
#define SAMSUNG 11
#define LG 12
#define UNKNOWN -1

#define TOPBIT 0x80000000


#ifdef F_CPU
#define SYSCLOCK F_CPU     // main Arduino clock
#else
#define SYSCLOCK 16000000  // main Arduino clock
#endif


#define _GAP 5000 // Minimum map between transmissions
#define GAP_TICKS (_GAP/USECPERTICK)


#define TIMER_DISABLE_INTR   (TIMSK2 = 0)
#define TIMER_ENABLE_PWM     (TCCR2A |= _BV(COM2B1))
#define TIMER_DISABLE_PWM    (TCCR2A &= ~(_BV(COM2B1)))
#define TIMER_ENABLE_INTR    (TIMSK2 = _BV(OCIE2A))
#define TIMER_DISABLE_INTR   (TIMSK2 = 0)
#define TIMER_INTR_NAME      TIMER2_COMPA_vect
#define TIMER_CONFIG_KHZ(val) ({ \
  const uint8_t pwmval = F_CPU / 2000 / (val); \
  TCCR2A = _BV(WGM20); \
  TCCR2B = _BV(WGM22) | _BV(CS20); \
  OCR2A = pwmval; \
  OCR2B = pwmval / 3; \
})

#define TIMER_COUNT_TOP      (SYSCLOCK * USECPERTICK / 1000000)
#if (TIMER_COUNT_TOP < 256)
#define TIMER_CONFIG_NORMAL() ({ \
  TCCR2A = _BV(WGM21); \
  TCCR2B = _BV(CS20); \
  OCR2A = TIMER_COUNT_TOP; \
  TCNT2 = 0; \
})
#else
#define TIMER_CONFIG_NORMAL() ({ \
  TCCR2A = _BV(WGM21); \
  TCCR2B = _BV(CS21); \
  OCR2A = TIMER_COUNT_TOP / 8; \
  TCNT2 = 0; \
})
#endif

// information for the interrupt handler
typedef struct {
  uint8_t recvpin;           // pin for IR data from detector
  volatile uint8_t rcvstate;          // state machine
  volatile uint32_t lastTime;
  unsigned int timer;     // 
  volatile uint8_t rawbuf[RAWBUF]; // raw data
  volatile uint8_t rawlen;         // counter of entries in rawbuf
} 
irparams_t;

// main class for receiving IR
/**
 * Class: MeIR
 * \par Description
 * Declaration of Class MeIR.
 */
class MeIR
{
public:
/**
 * Alternate Constructor which can call your own function to map the IR to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
*/
  MeIR();

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
  ErrorStatus decode();
  
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
  void begin();

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
  void end();
  
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
  void loop();
  
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
  boolean keyPressed(unsigned char r);
  // void resume();

  int8_t decode_type; // NEC, SONY, RC5, UNKNOWN
  unsigned long value; // Decoded value
  uint8_t bits; // Number of bits in decoded value
  volatile uint8_t *rawbuf; // Raw intervals in .5 us ticks
  int rawlen; // Number of records in rawbuf.
  
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
  String getString();
  
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
  unsigned char getCode();
  
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
  void sendString(String s);
  
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
  void sendString(float v);
  
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
  void sendNEC(unsigned long data, int nbits);
  
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
  void sendRaw(unsigned int buf[], int len, uint8_t hz);
  
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
  void enableIROut(uint8_t khz);
  
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
  void enableIRIn();
  
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
  void mark(uint16_t us);
  
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
  void space(uint16_t us);

private:
  // These are called by decode
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
  ErrorStatus decodeNEC();
  
  int16_t irIndex;	
  char irRead;
  char floatString[5];
  boolean irReady;
  boolean irPressed;
  String irBuffer;
  String Pre_Str;
  double irDelayTime;
};
#endif // !__AVR_ATmega32U4__
#endif

