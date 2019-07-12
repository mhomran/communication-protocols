#include "USART.h"


void INIT_UART(int BAUD){
	UBRR0 = (F_CPU/16/BAUD)-1;
	UCSR0B |= 1 << TXEN0 | 1 << RXEN0 | 1 << RXCIE0;
	UCSR0B &= ~(1 << UCSZ02);
	UCSR0C |= 1 << UCSZ00 | 1 << UCSZ01;
	sei();

	//initialize data
	WAIT_SYNC_BREAK = 1;
	WAIT_SYNC_FIELD = 0;
	WAIT_ID, RECEIVE = 0;
	DATA_LEN = 0;
	DATA_INDEX = 0;
	VALID_ID = 0;
	slave_id = 0x1;
}

void USART_TX(char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

void USART_PRINTF(const char *str)
{
	int x =0;
	while (str[x]){
	USART_TX(str[x]);
	x++;
	}
}

void USART_PRINTN(uint8_t ch){
	char str[4];
	
	str[3] = '\0';
	int i;
	for(i = 0; i < 3; i++){
		switch (i){
			case 0 :
			{
				str[2-i] = (ch % 10) + 48;
			}
			break;
			case 1 :
			{
				str[2-i] = ((ch / 10) % 10 )+ 48;
			}
			break;
			case 2 :
			{
				str[2-i] = ((ch/100) % 10) + 48;
			}
			break;
		}
	}
		
	USART_PRINTF(str);
}

void USART_SET_BAUD(int BAUD_FN){
UBRR0 = (F_CPU/16/BAUD_FN)-1;
}

ISR(USART_RX_vect){		//((1/9600)*(13+2)/(8+2))^-1 ->  //Baud rate necessary to cover 13 bit position in Normal speed 
														//after that time the bus will be in "break" condition
	data_rx = UDR0;										//the receiver will read 0x00 and a frame error flag will be set
														//the receiver will start reading another frame only if start transition happens
	if (WAIT_SYNC_BREAK && data_rx == 0){				//from high (idle) to low.
	USART_SET_BAUD(6400);							
	WAIT_SYNC_BREAK = 0;								//baud speed is chosen according to the speed of processor to handle the ISR 
	WAIT_SYNC_FIELD = 1;									
	}
	else if (WAIT_SYNC_FIELD){
		if(data_rx == 0x55){
			WAIT_SYNC_FIELD = 0;
			WAIT_ID = 1;
		}
		else{	//RETURN BACK
			USART_SET_BAUD(9600);
			WAIT_SYNC_BREAK = 1;
			WAIT_SYNC_FIELD = 0;
		}
	}
	else if (WAIT_ID){		//data + 1(checksum) 
		VALID_ID = 1;
		if(data_rx >= 0 && data_rx < 32)
		{
			DATA_LEN = 3;
		}
		else if(data_rx >= 32 && data_rx < 48)
		{
			DATA_LEN = 5;
		}
		else if(data_rx >= 48 && data_rx < 64)
		{
			DATA_LEN = 9;
		}
		else 
		{
			VALID_ID = 0;
		}
		if (VALID_ID){		
			if(data_rx == slave_id)			//transmit
			{
				RECEIVE = 0;
				UDR0 = data_bytes[0];
				DATA_INDEX = 1;
			}
			else{
				RECEIVE = 1;
				DATA_INDEX = 0;
			}
			WAIT_ID = 0;
		}
	}
	else if (RECEIVE) 
	{
		if(DATA_INDEX == DATA_LEN)
		{
			USART_SET_BAUD(9600);
			WAIT_SYNC_BREAK = 1;
		}
		else
		{
		data_bytes[0] = data_rx;
		DATA_INDEX++;
		}
	}
	else 
	{
		if(DATA_INDEX == DATA_LEN)
		{
			USART_SET_BAUD(9600);
			WAIT_SYNC_BREAK = 1;
		}
		else
		{
			UDR0 = data_bytes[0];
			DATA_INDEX++;
		}
	} 
}