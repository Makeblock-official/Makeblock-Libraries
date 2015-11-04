void setup() 
{
  Serial.begin(9600);
}
void loop() 
{
Serial.print("CLS(0);"); // clear the screen with c color 
Serial.print("DR0;");    // the screen displays in upright way
Serial.print("DS32(150,150,'makeblock',4);"); // display hello world with 32 dot matrix at the position of coordinate (150, 150)
delay(3000);// wait 
Serial.print("CLS(0);"); // clear the screen with c color 
Serial.print("DR0;");    // the screen displays in upright way
Serial.println("DS32(150,150,'mBot',4);"); // display hello world with 32 dot matrix at the position of coordinate (150, 150)
delay(3000);// wait 
Serial.print("CLS(0);"); // clear the screen with c color 
Serial.print("DR0;");    // the screen displays in upright way
Serial.println("DS32(150,150,'mBlock',4);"); // display hello world with 32 dot matrix at the position of coordinate (150, 150)
delay(3000);// wait
Serial.print("CLS(0);"); // clear the screen with c color 
Serial.print("DR0;");    // the screen displays in upright way
Serial.println("DS32(150,150,'Hello world!',4);"); // display hello world with 32 dot matrix at the position of coordinate (150, 150)
delay(3000);// wait 
Serial.print("CLS(0);"); // clear the screen with c color
Serial.print("DR2;");    // the screen displays in upside-down way 
Serial.print("CIR(30,100,20,3);");// display a circle of 20 radius at the position of coordinate (30, 100)
Serial.print("CIR(110,110,80,6);"); // display a circle of 20 radius at the position of coordinate (110, 120)
Serial.print("PL(10,10,200,200,4);");// display a straight line from coordinate (10,10) to coordinate (200,200)
Serial.print("PL(280,10,30,200,5);"); // display a straight line from coordinate (280,10) to coordinate (300,200)
Serial.print("BOX(50,20,230,150,2);");// display a rectangle from coordinate (50,20) to coordinate (230,150) 
Serial.println("BOXF(250,170,300,220,1);"); // display a solid rectangle from coordinate (250,170) to coordinate (300,220)  
delay(3000);
}
