/*
 * LCD.c
 *
 * Edited: 6/20/2019 10:27 PM
 *  Author: Mohamed_Hassanin
 */ 
//this version of LCD driver has 0 delay. So it increases the performance of the CPU and reduces its load.
//trade-off: 4-bit timer usage + 2 interrupts.
#include "LCD.h"
volatile int i = 0;
volatile int N = 0;
volatile int char_idx = 0;
struct LCD_INST{
	int Type;					//1 for command, 0 for data
	unsigned char data;
};

volatile struct LCD_INST LCD_INSTs[max];


void SEND_A_COMMAND(unsigned char command){
	
	if(TCCR0B & 1 << CS01){	
	TCCR0B &=~( 1 << CS01);
	}
	LCD_INSTs[N].data = command; LCD_INSTs[N].Type = 1; N++;
	LCD_INSTs[N].data = (command << 4); LCD_INSTs[N].Type = 1; N++;
	TCCR0B |= 1 << CS01;
}

void LCD_PRINT(char *str)
{
	
	if(TCCR0B & 1 << CS01){
	TCCR0B &=~( 1 << CS01);
	}
	int x = 0;
	while(str[x]!=0)  /* send each char of string till the NULL */
	{
		LCD_INSTs[N].data = str[x]; LCD_INSTs[N].Type = 0; N++;
		LCD_INSTs[N].data = (str[x] << 4); LCD_INSTs[N].Type = 0; N++;
		x++;
	}
	TCCR0B |= 1 << CS01;	
}
void LCD_CLEAR(void){
	SEND_A_COMMAND(0x01);
}

void init_LCD(void){
	LCD_CONTROL_DDR |= 1 << LIGHT_SWITCH | 1 << RW | 1 << RS;					//declare as output
	LCD_DATA_DDR = 0xFF;
	sei();
	TCCR0A |= 1 << WGM01;
	TIMSK0 |= 1 << OCIE0A | 1 << OCIE0B;
	OCR0A  = 200;
	OCR0B  = 100;

	LCD_INSTs[N].data = 0x20; LCD_INSTs[N].Type = 1; N++;		//is a must for 4-bit mode
	SEND_A_COMMAND(0x28);
	SEND_A_COMMAND(0x0F);
}

ISR(TIMER0_COMPA_vect){
	LCD_CONTROL &= ~ (1 << LIGHT_SWITCH);
	LCD_DATA = 0x00;
}
ISR(TIMER0_COMPB_vect){
	if(i >= N){
		TCCR0B &= ~(1 << CS01);		//finish
		i = 0;
		N = 0;
	}
	else if(LCD_INSTs[i].Type)	//command
	{
		LCD_DATA = LCD_INSTs[i].data;
		LCD_CONTROL &= ~ (1 << RW | 1 << RS);

		LCD_CONTROL |= 1 << LIGHT_SWITCH;
		i++;
	}
	else{
		if(char_idx == 16){
			
			char_idx = 0;
		}
		LCD_DATA = LCD_INSTs[i].data;	//data
		LCD_CONTROL |= 1 << RS;
		LCD_CONTROL &= ~ (1 << RW);
		
		LCD_CONTROL |= 1 << LIGHT_SWITCH;
		i++;
		char_idx++;
	}
}
void LCD_SET_CURSOR(uint8_t row, uint8_t clm){
	if(clm)
	SEND_A_COMMAND(0b11000000);			//get to the second row
	else
	SEND_A_COMMAND(0b10000000);			//get to the second row
	
	int i;
	for (i=0; i < (row % 16); i ++)
	{
		SEND_A_COMMAND(0b00010100);			//get to the first row
	}
}