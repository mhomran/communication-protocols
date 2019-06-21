#include "LCD_I2C.h"


void SEND_A_COMMAND(unsigned char command){
	if(TCCR0B & 1 << CS01 | 1 << CS00){	
	TCCR0B &=~( 1 << CS01 | 1 << CS00);
	}
	LCD_INSTs[N].data = command; LCD_INSTs[N].Type = 1; N++;
	LCD_INSTs[N].data = (command << 4); LCD_INSTs[N].Type = 1; N++;
	TCCR0B |= 1 << CS01 | 1 << CS00;
}
void LCD_PRINT(char *str)
{
	
	if(TCCR0B & 1 << CS01 | 1 << CS00){
	TCCR0B &=~( 1 << CS01 | 1 << CS00);
	}
	int x = 0;
	while(str[x]!=0)  /* send each char of string till the NULL */
	{
		LCD_INSTs[N].data = str[x]; LCD_INSTs[N].Type = 0; N++;
		LCD_INSTs[N].data = (str[x] << 4); LCD_INSTs[N].Type = 0; N++;
		x++;
	}
	TCCR0B |= 1 << CS01 | 1 << CS00;	
}


void INIT_LCD_I2C(uint8_t ADDR_fn){
	N=0;
	i = 0;
	char_idx =0;
	INIT_I2C(ADDR_fn);
	TCCR0A |= 1 << WGM01;
	TIMSK0 |= 1 << OCIE0A | 1 << OCIE0B;
	OCR0A  = 200;
	OCR0B  = 100;

	LCD_INSTs[N].data = 0x20; LCD_INSTs[N].Type = 1; N++;		//is a must for 4-bit mode
	SEND_A_COMMAND(0x28);
	SEND_A_COMMAND(0x0F);
}

ISR(TIMER0_COMPA_vect){
	last_data = (last_data & 0b11111011) ;
	I2C_SEND (last_data);
}
ISR(TIMER0_COMPB_vect){
	if(i >= N){
		TCCR0B &= ~(1 << CS01 | 1 << CS00);		//finish
		i = 0;
		N = 0;
	}
	else if(LCD_INSTs[i].Type)	//command
	{
		last_data = (LCD_INSTs[i].data & 0xF0) | 0b00000100; 
		I2C_SEND(last_data);
		i++;
	}
	else{
		if(char_idx == 16){
			
			char_idx = 0;
		}
		last_data = (LCD_INSTs[i].data & 0xF0) | 0b00000101;
		I2C_SEND(last_data);
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