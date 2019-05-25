/*
 * LCD.c
 *
 * Created: 5/26/2019 1:39:22 AM
 *  Author: Mohamed_Hassanin
 */ 
#include "LCD.h"
volatile int i = 0;
volatile int N = 0;
volatile struct LCD_INST{
	int Type;					//1 for command, 0 for data
	unsigned char data;
};

volatile struct LCD_INST LCD_INSTs[max];


void SEND_A_COMMAND(unsigned char command){
LCD_INSTs[N].data = command; LCD_INSTs[N].Type = 1; N++;
	TCCR0B |= 1 << CS00;	
}

void LCD_PRINT(char *str)
{
	int x;
	for(x=0;str[x]!=0;x++)  /* send each char of string till the NULL */
	{
	LCD_INSTs[N].data = str[x]; LCD_INSTs[N].Type = 0; N++;	
	}
	TCCR0B |= 1 << CS00;
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
	OCR0A  = 32;
	OCR0B  = 16;
	
	//delay by ticks for just initialization
	_delay_ms(15);
	SEND_A_COMMAND(0x01);														//clear screen
	_delay_ms(2);
	
	LCD_INSTs[N].data = 0x38; LCD_INSTs[N].Type = 1; N++;
	LCD_INSTs[N].data = 0b00001111; LCD_INSTs[N].Type = 1; N++;
	TCCR0B |= 1 << CS00;

	/*		//avoid accessing the data while timer is working
			//because you try to access the data while it may be accessed at the same time by an interrupt.
			//so there must be #polling for CS00 to determine if the first print is done.
	LCD_PRINT("lol");
	*/
}

ISR(TIMER0_COMPA_vect){
	LCD_CONTROL &= ~ (1 << LIGHT_SWITCH);
	LCD_DATA = 0x00;
}
ISR(TIMER0_COMPB_vect){
	if(i >= N){
		TCCR0B &= ~(1 << CS00);		//finish
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
		LCD_DATA = LCD_INSTs[i].data;
		LCD_CONTROL |= 1 << RS;
		LCD_CONTROL &= ~ (1 << RW);
		
		LCD_CONTROL |= 1 << LIGHT_SWITCH;
		i++;
	}
}
