/*
 * LCD_16x2.c
 *
 * Created: 5/25/2019 7:26:20 PM
 * Author : Mohamed_Hassanin
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

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
void CHECK_IF_BUSY(void);
void init_LCD(void);

void SEND_A_COMMAND(unsigned char command){
	CHECK_IF_BUSY();
	LCD_DATA = command;
	LCD_CONTROL &= ~ (1 << RW | 1 << RS);
	
	LCD_CONTROL |= 1 << LIGHT_SWITCH;
	_delay_us(1);											
	LCD_CONTROL &= ~ 1 << LIGHT_SWITCH;

	LCD_DATA = 0x00;
}
void SEND_A_CHARACTER(unsigned char character){
	
	LCD_DATA = character;
	LCD_CONTROL |= 1 << RS;
	LCD_CONTROL &= ~ (1 << RW);
	
	LCD_CONTROL |= 1 << LIGHT_SWITCH;
	_delay_us(1);											
	LCD_CONTROL &= ~ (1 << LIGHT_SWITCH);
	LCD_DATA = 0x00;	
}
void LCD_CLEAR(void){
	SEND_A_COMMAND(0x01);
}
void CHECK_IF_BUSY(void){
	LCD_DATA_DDR = 0;
	LCD_CONTROL |= 1 << RW;
	LCD_CONTROL &= ~ (1 << RS);
	while(LCD_DATA >= 0x80){	//busy if pin 7 is 1, and vice versa	0b10000000
		LCD_CONTROL |= 1 << LIGHT_SWITCH;
		_delay_us(1);											//Important
		LCD_CONTROL &= ~ 1 << LIGHT_SWITCH;
	}
	LCD_DATA_DDR = 0xFF;	 
}

void init_LCD(void){
	LCD_CONTROL_DDR |= 1 << LIGHT_SWITCH | 1 << RW | 1 << RS;					//declare as output
	_delay_ms(15);
	SEND_A_COMMAND(0x01);														//clear screen
	_delay_ms(2);
	SEND_A_COMMAND(0x38);														//8 bit mode
	_delay_us(50);
	SEND_A_COMMAND(0b00001111);													//0b0000|1|Display on or off|cursor on or off|cursor blinking
	_delay_us(50);
	
	
	SEND_A_CHARACTER('A');
}
int main(void)
{
	init_LCD();

	while (1) 
    {
    }
}

