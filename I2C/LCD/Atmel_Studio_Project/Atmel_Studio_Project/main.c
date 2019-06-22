
#include <avr/io.h>
#include "LCD_I2C.h"

int main()
 { 
	INIT_LCD_I2C();
	LCD_PRINT("I did it");
	LCD_SET_CURSOR(1,0);
 	LCD_PRINT("thanks God");
 	LCD_SET_CURSOR(0,11);
 	LCD_PRINT("007");
 	while (1)
      ;
   return 0;
 }