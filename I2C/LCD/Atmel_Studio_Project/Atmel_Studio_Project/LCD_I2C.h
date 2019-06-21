
#ifndef LCD_I2C_H_
#define LCD_I2C_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#include "I2C.h"

#ifndef	F_CPU
#define F_CPU 16000000UL
#endif	
#include <util/delay.h>

volatile int i ;
volatile int N ;
volatile int char_idx ;
volatile int last_data;
struct LCD_INST{
	int Type;					//1 for command, 0 for data
	unsigned char data;
};

#define max 32			
volatile struct LCD_INST LCD_INSTs[max];

void INIT_LCD_I2C(uint8_t ADDR_fn);


#endif /* LCD_I2C_H_ */