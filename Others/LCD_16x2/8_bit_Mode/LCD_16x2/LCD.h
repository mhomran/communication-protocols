/*
 * LCD.h
 *
 * Created: 5/26/2019 1:38:54 AM
 *  Author: Mohamed_Hassanin
 */ 


#ifndef LCD_H_
#define LCD_H_


#include <avr/io.h>

#ifndef	F_CPU
#define F_CPU 16000000UL
#endif	

#include <util/delay.h>
#include <avr/interrupt.h>

#define LCD_DATA_DDR	DDRD
#define LCD_DATA		PORTD
#define LCD_CONTROL_DDR	DDRC
#define LCD_CONTROL		PORTC
#define LIGHT_SWITCH	5
#define RW				4
#define	RS				3

void SEND_A_COMMAND(unsigned char command);
void SEND_A_CHARACTER(unsigned char character);
void LCD_CLEAR(void);
void init_LCD(void);
#define max 32


#endif /* LCD_H_ */