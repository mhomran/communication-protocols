
#include <avr/io.h>

#include "LCD_I2C.h"

int main()
 { 
	INIT_LCD_I2C(0x27);
	LCD_PRINT("I did it");
	
	while (1)
      ;
   return 0;
 }