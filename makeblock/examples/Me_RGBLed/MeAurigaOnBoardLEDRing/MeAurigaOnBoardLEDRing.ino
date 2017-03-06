
/**
 * \par Copyright (C), 2017, MakeBlock
 * @file    MeAurigaOnBoardLEDRing.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2017/03/01
 * @brief   Description: this file is sample code for the Auriga on-board 12 LED ring device.
 *
 * Function List:
 * 1. bool MeRGBLed::setColor(uint8_t index, uint8_t red, uint8_t green, uint8_t blue)
 * 2. void MeRGBLed::show()
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Gregory Toto 2017/03/01    1.0.0          First version example code
 * </pre>
 *
 */
 
 #include <MeAuriga.h>

#define AURIGARINGLEDNUM  12
#define RINGALLLEDS        0


#ifdef MeAuriga_H
// on-board LED ring, at PORT0 (onboard), with 12 LEDs
MeRGBLed led_ring( 0, 12 );
#endif


void setup()
{
    Serial.begin( 9600 );

#ifdef MeAuriga_H
    // 12 LED Ring controller is on Auriga D44/PWM
    led_ring.setpin( 44 );
#endif

}

void loop()
{
  int i;

    // cycle the RING LEDS through RED/GREEN/BLUE at increasing brightnesses
    for ( i = 50; i <= 150; i+=50 ) 
    {

      // led_ring.setColor( 0...12, 0...255, 0...255, 0...255 )
      // turn on all 12 leds, RED, incrementing brightness by 50 each
      // time through the loop
      Serial.print("RED ");
      led_ring.setColor( RINGALLLEDS, i, 0, 0 ) ;
      led_ring.show();
      delay( 500 );
  
      Serial.print("GREEN ");
      led_ring.setColor( RINGALLLEDS, 0, i, 0 );
      led_ring.show();
      delay( 500 );
  
      Serial.println("BLUE ");
      led_ring.setColor( RINGALLLEDS, 0, 0, i );
      led_ring.show();
      delay( 500 );
    }

    // all LEDs off
    led_ring.setColor( RINGALLLEDS, 0, 0, 0 );
    led_ring.show();
    delay( 500 );
        
    for ( i = 1; i <= AURIGARINGLEDNUM; i++ )
    {
        // Turn on  LEDs one at a time with some interesting color
        Serial.print(i);
        led_ring.setColor( i, 40, 10, 40);
        led_ring.show();
        delay( 200 );
    }
    Serial.println("");
}

