/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    MeTFT.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/11/10
 * @brief   Description: this file is sample program for TFT LCD.
 *
 * Function List:
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/11/10     1.0.0                   
 * </pre>
 */
#include "MeOrion.h"
#include <SoftwareSerial.h>

MeSerial mySerial(PORT_5);

//-------------------------------------  64 colors of c  --------------------------------------------//
// These 64 colors are 'c' value of command
// with this sample code, you can see the color in your LCD.


//-----------------------------------  Screen coordinates  ------------------------------------------//
// The upper left corner is coordinate point(0,0)
// Screen resolution is 320 * 240
// transverse direction is the X direction, 
// the vertical direction is the Y direction;


//-----------------------------------  Command Description ------------------------------------------//
// Command                 Description                               sample code         
// CLC(c);                 clear screen with c color                 CLS(0);
// SCC(c,n)                customizing colors for c                  SCC(15,65535);
// SBC(c)                  Set Background Color                      SBC(1)
// PS(x,y,c)               draw a point at (x, y) with color c       PS(100,100,1);
// PL(x1,y1,x2,y2,c);      draw line from (x1,y1) to (x2,y2)         PL(0,0,100,0,2);
// BOX(x1,y1,x2,y2,c);     draw rectangle,Left corner(x1,y1)         BOX(0,0,100,100,2);
//                         Bottom right corner(x2,y2)    
// BOXF(x1,y1,x2,y2,c);    draw Solid rectangle,Left corner(x1,y1)   BOXF(0,0,100,100,2);
// PIC(x,y,n)              painted figure n at(x,y)                  PIC(0,0,1);
// DS12(x,y,'String',c);   display character 12 dot matrix characters
// DS16(x,y,'String',c);   display character 16 dot matrix characters
// DS24(x,y,'String',c);   display character 24 dot matrix characters
// DS32(x,y,'String',c);   display character 32 dot matrix characters
// DS64(x,y,'String',c);   display character 64 dot matrix characters
// DRn                     Set the screen orientation; n is (0-3), 
//                         corresponding to four directions screen,   DR0: horizontal screen display
//                         you can use this to adjust the horizontal  DR1: vertical screen display
//                         and vertical screen display; This command  DR2: horizontal screen upside down
//                         will not clear screen                      DR3: vertical screen upside down
// Others: see the doc   

#define MAX_STRING_LEN  128
String show_buffer = "";
char str_buffer[MAX_STRING_LEN];

const char String_color[][MAX_STRING_LEN]  PROGMEM =
{
  "CLS(0);",
  "DS16(0,0,'Common color:',15);",
  "PL(0,18,320,18,15);",
  "DS12(4,24,'0- ',4);BOXF(20,24,40,36,0);",
  "DS12(54,24,'1- ',4);BOXF(70,24,90,36,1);",
  "DS12(104,24,'2- ',4);BOXF(120,24,140,36,2);",
  "DS12(154,24,'3- ',4);BOXF(170,24,190,36,3);",
  "DS12(204,24,'4- ',4);BOXF(220,24,240,36,4);",
  "DS12(254,24,'5- ',4);BOXF(270,24,290,36,5);",

  "DS12(4,44,'6- ',4);BOXF(20,44,40,56,6);",
  "DS12(54,44,'7-',4);BOXF(70,44,90,56,7);",
  "DS12(104,44,'8- ',4);BOXF(120,44,140,56,8);",
  "DS12(154,44,'9- ',4);BOXF(170,44,190,56,9);",
  "DS12(204,44,'10- ',4);BOXF(220,44,240,56,10);",
  "DS12(254,44,'11-',4);BOXF(270,44,290,56,11);",

  "DS12(4,64,'12- ',4);BOXF(20,64,40,76,12);",
  "DS12(54,64,'13-',4);BOXF(70,64,90,76,13);",
  "DS12(104,64,'14- ',4);BOXF(120,64,140,76,14);",
  "DS12(154,64,'15- ',4);BOXF(170,64,190,76,15);",
  "DS12(204,64,'16- ',4);BOXF(220,64,240,76,16);",
  "DS12(254,64,'17-',4);BOXF(270,64,290,76,17);",

  "DS12(4,128,'We can use < SCC(c,n) > command to set the color',15);",
  "DS12(4,140,'c - index of color  <0-63>',15);",
  "DS12(4,152,'n - 64K color val   <0-65536>',15);\r\n",
};

//pgm_read_byte
void read_data_from_string_color(uint8_t index)
{
  uint8_t read_data;
  for(uint8_t num=0; num < MAX_STRING_LEN; num++)
  {
    read_data = pgm_read_byte(&String_color[index][num]);
    str_buffer[num] = read_data;
    if(read_data == '\0')
    {
      show_buffer = String(str_buffer);
      break;
    }
  }
}

void setup() 
{
  mySerial.begin(9600);
}

void print_string(void)
{
  mySerial.print("CLS(0);"); // clear the screen with c color 
  mySerial.print("DR0;");    // the screen displays in upright way
  mySerial.println("DS64(64,104,'Hello world!',4);");  
}

void print_para(void)
{
  float tmp = 28.90;
  float humidity = 45.7;
  mySerial.print("CLS(0);"); // clear the screen with c color 
  mySerial.print("DR0;");    // the screen displays in upright way
  mySerial.print("DS64(0,0,'Temperature: ");
  mySerial.print(tmp);
  mySerial.print("',4);");
  mySerial.print("DS64(0,64,'Humidity:       ");
  mySerial.print(humidity);
  mySerial.println("%',4);");
}

void draw_circle(void)
{
  mySerial.print("CLS(0);"); // clear the screen with c color 
  mySerial.print("DR0;");    // the screen displays in upright way
  mySerial.println("CIR(160,120,80,4);");  
}

void draw_rectangle(void)
{
  mySerial.print("CLS(0);"); // clear the screen with c color 
  mySerial.print("DR0;");    // the screen displays in upright way
  mySerial.print("DS24(112,112,'1234567890',4);");  
  mySerial.println("BOX(85,45,235,195,4);");
}

void draw_solid_rectangle(void)
{
  mySerial.print("CLS(0);"); // clear the screen with c color 
  mySerial.print("DR0;");    // the screen displays in upright way
  mySerial.println("BOXF(85,45,235,195,4);");  
}

void loop() 
{
  for(int i = 0; i < 25; i++)
  {
    read_data_from_string_color(i);
    mySerial.print(show_buffer);
  }
  delay(3000);// wait

  print_string();
  delay(1000);// wait

  print_para();
  delay(1000);// wait

  draw_circle();
  delay(1000);

  draw_rectangle();
  delay(1000);
  
  draw_solid_rectangle();
  delay(000);
}
