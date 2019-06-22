
#ifndef LCD_I2C_H_
#define LCD_I2C_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

#include "Wire.h"

#define F_CPU 16000000UL

#include <util/delay.h>

#define DEVICE 0x27		//device address

volatile int i ;
volatile int N ;
volatile int char_idx ;
volatile int last_data;
struct LCD_INST{
	int Type;					//1 for command, 0 for data
	unsigned char data;
};

#define max 64
volatile struct LCD_INST LCD_INSTs[max];

void INIT_LCD_I2C();
void SEND_A_COMMAND(unsigned char command);
void LCD_PRINT(char *str);
void LCD_SET_CURSOR(uint8_t row, uint8_t clm);
void LCD_CLR(void);
#endif /* LCD_I2C_H_ */