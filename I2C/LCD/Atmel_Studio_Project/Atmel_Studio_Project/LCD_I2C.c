#include "LCD_I2C.h"
/*
PCF8574 pins:
-pins 7,6,5,4 is connected to D7,D6,D5,D4 respectively
-pins 0,1,2 is connected to RS,RW,EN repectively
-pin 3 enables the backlight of the LCD
 */

void SEND_A_COMMAND(unsigned char command){
	if(TCCR0B & 1 << CS02 | 1 << CS00){	
	TCCR0B &=~( 1 << CS02 | 1 << CS00);
	}
	LCD_INSTs[N].data = command; LCD_INSTs[N].Type = 1; N++;
	//LCD_INSTs[N].data = (command << 4); LCD_INSTs[N].Type = 1; N++;
	TCCR0B |= 1 << CS02 | 1 << CS00;
}
void LCD_PRINT(char *str)
{
	
	if(TCCR0B & 1 << CS02 | 1 << CS00){
	TCCR0B &=~( 1 << CS02 | 1 << CS00);
	}
	int x = 0;
	while(str[x]!=0)  /* send each char of string till the NULL */
	{
		LCD_INSTs[N].data = str[x]; LCD_INSTs[N].Type = 0; N++;
		//LCD_INSTs[N].data = (str[x] << 4); LCD_INSTs[N].Type = 0; N++;
		x++;
	}
	TCCR0B |= 1 << CS02 | 1 << CS00;	
}


void INIT_LCD_I2C(){
	sei();			 //enable interrupts.
	beginAsMaster(); // join i2c bus (address optional for master)
	
	beginTransmission(DEVICE); // start transmission to device
	endTransmissionThenStop(); // end transmission
	
	beginTransmission(DEVICE); // start transmission to device
	write(0x00);
	endTransmissionThenStop(); // end transmission
	
	_delay_ms(100);
	
	N=0;
	i = 0;
	MSB_nibble = 0;			//for 0x20 command in one nibble only, so that LCD works
	TCCR0A |= 1 << WGM01;	//-> to execute it but 0x02 in the buffer and make MSB_nibble flag 0
	TIMSK0 |= 1 << OCIE0A | 1 << OCIE0B;
	OCR0A  = 62;		//250 Hz freq - 4 ms period
	OCR0B  = 31;
	
	LCD_INSTs[N].data = 0x02; LCD_INSTs[N].Type = 1; N++;
	SEND_A_COMMAND(0x28);
	SEND_A_COMMAND(0x0F);
	LCD_CLR();
	
}

ISR(TIMER0_COMPA_vect){
	sei();					//enable nested interrupts for TWI because it needs to handle the bus as soon as the TWINT is set.
	last_data = (last_data & 0b11111011) ;
	
	beginTransmission(DEVICE); // start transmission to device
	write(last_data);
	endTransmissionThenStop(); // end transmission
}

ISR(TIMER0_COMPB_vect){
	sei();					//enable nested interrupts for TWI because it needs to handle the bus as soon as the TWINT is set.
	if(i >= N){
		TCCR0B &= ~(1 << CS02 | 1 << CS00);		//finish
		i = 0;
		N = 0;
	}
	else if(LCD_INSTs[i].Type)	//command
	{
		if(MSB_nibble){
		last_data = (LCD_INSTs[i].data & 0xF0) | 0b00001100; 
		MSB_nibble = 0;
		}
		else {
		last_data = (LCD_INSTs[i].data << 4) | 0b00001100;	
		MSB_nibble = 1;
		i++;
		}
		beginTransmission(DEVICE); // start transmission to device
		write(last_data);
		endTransmissionThenStop(); // end transmission
//		i++;
	}
	else{
		
		if(MSB_nibble){	
		last_data = (LCD_INSTs[i].data & 0xF0) | 0b00001101;
		MSB_nibble = 0;
		}
		else{
		last_data = (LCD_INSTs[i].data << 4)  | 0b00001101;	
		MSB_nibble = 1;
		i++;
		}
		
		beginTransmission(DEVICE); // start transmission to device
		write(last_data);
		endTransmissionThenStop(); // end transmission
//		i++;
	}
}
void LCD_SET_CURSOR(uint8_t row, uint8_t clm){
	if(row)
	SEND_A_COMMAND(0b11000000);			//get to the second row
	else
	SEND_A_COMMAND(0b10000000);			//get to the second row
	
	int i;
	for (i=0; i < (clm % 16); i ++)
	{
		SEND_A_COMMAND(0b00010100);			//get to the first row
	}
}
void LCD_CLR(void){
	SEND_A_COMMAND(0x01);	
}