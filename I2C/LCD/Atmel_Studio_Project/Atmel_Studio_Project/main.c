
#include <avr/io.h>
#include "LCD_I2C.h"

int main()
 { 
	INIT_LCD_I2C();
	LCD_PRINT("I did it!");
	LCD_SET_CURSOR(1,0);
	LCD_PRINT("thanks God");
	
	while (1)
      ;
   return 0;
 }