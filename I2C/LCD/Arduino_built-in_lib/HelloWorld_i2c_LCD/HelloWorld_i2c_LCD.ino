
/*-----( Import needed libraries )-----*/
#include <Wire.h>  // Comes with Arduino IDE
// Get the LCD I2C Library here: 
// https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move any other LCD libraries to another folder or delete them
// See Library "Docs" folder for possible commands etc.
#include <LiquidCrystal_I2C.h>
 
/*-----( Declare Constants )-----*/
/*-----( Declare objects )-----*/
// set the LCD address to 0x27 for a 20 chars 4 line display
// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
byte smile1[] = {
   B00000,
  B00000,
  B11011,
  B11011,
  B00000,
  B10001,
  B01010,
  B00100
};
byte smile2[] = {
  B00000,
  B00000,
  B11011,
  B11011,
  B00000,
  B11111,
  B00000,
  B00000
};
byte smile3[] = {
  B00000,
  B00000,
  B11011,
  B11011,
  B00000,
  B00100,
  B01010,   
  B10001
};
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
 
void setup()
{
  // initialize library
  lcd.begin(16,2);
  // set cursor to positon x=0, y=0
 
  lcd.print("l");

}
 
 
void loop()
{ 
}
