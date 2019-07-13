#include "LIN_SLAVE.h"


void INIT_UART(int BAUD){
	UBRR0 = (F_CPU/16/BAUD)-1;
	UCSR0B |= 1 << RXCIE0 | 1 << TXCIE0;
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
	VALID_DATA = 0;		//assume that the data isn't valid
	slave_id = 0x1;
	WAIT_CHECKSUM = 0;
	//data_bytes[0] = 0x1;
	//data_bytes[1] = 0x1;
	//data_bytes[2] = (0xff - 0x2); //checksum
}

void USART_TX(char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

void USART_SET_BAUD(int BAUD_FN){
UBRR0 = (F_CPU/16/BAUD_FN)-1;
}

ISR(USART_RX_vect){		
															
	data_rx = UDR0;											
	if (WAIT_SYNC_FIELD){
		if(data_rx == 0x55){
			WAIT_SYNC_FIELD = 0;
			WAIT_ID = 1;
		}
		else
		{	//RETURN BACK
			//disable RXEN and enable CAPT interrupt
			UCSR0B &= ~(1 << RXEN0);
			TIMSK1 |= (1 << ICIE1);
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
				//disable RXEN and disable CAPT interrupt - enable transmitter
				UCSR0B &= ~(1 << RXEN0);
				UCSR0B |=   1 << TXEN0;
				TIMSK1 &= ~(1 << ICIE1);
							
				UDR0 = data_bytes_TX[0];
				DATA_INDEX = 1;
			}
			else{
				RECEIVE = 1;
				DATA_INDEX = 0;
				WAIT_CHECKSUM = 0;
			}
			WAIT_ID = 0;
		}
	}
	else 
	{
		data_bytes_RX[DATA_INDEX] = data_rx;
		DATA_INDEX++;
	
		if(DATA_INDEX == DATA_LEN)
		{
			//disable RXEN and enable CAPT interrupt
			UCSR0B &= ~(1 << RXEN0);
			TIMSK1 |= (1 << ICIE1);
			WAIT_CHECKSUM = 1;
		}	
	} 
}

volatile int x;

ISR(TIMER1_CAPT_vect){

	if( !(TCCR1B & (1 << ICES1)) ){			//falling ?
		TCCR1B |= (1 << ICES1); //triggers rising edge
		TCNT1 = 0;				//clear timer's value
	}
	else
	{
		x = ICR1;									//10840 is the time to cover 13 bit
		if ( x >= 10840  || (TIFR1 & (1 << TOV1)) ){
			WAIT_SYNC_FIELD = 1;
			//enable RXEN and disable CAPT interrupt
			UCSR0B |= 1 << RXEN0;
			TIMSK1 &= ~(1 << ICIE1);
		}
		else
		{
			TCCR1B &= ~(1 << ICES1); //triggers falling edge
		}
	}
	TIFR1 |= 1 << TOV1;		//clear overflow flag
}

void checksum(void){
	int i = 0;
	int sum = 0;
	if(WAIT_CHECKSUM)
	{
		while(i < (DATA_LEN-1) )
		{
			sum += data_bytes_RX[i];
			if(sum >= 256)
			{
				sum &= 0xFF;
				sum ++;
			}
			i++;
		}

		sum = ~sum;
		sum &= 0xFF;

		if(sum == data_bytes_RX[DATA_LEN-1])
		VALID_DATA = 1;
		else
		VALID_DATA = 0;

		WAIT_CHECKSUM = 0;
	}
}
ISR(USART_TX_vect)
{
	UDR0 = data_bytes_TX[DATA_INDEX];
	DATA_INDEX++;
	
	if(DATA_INDEX == DATA_LEN)
	{
		//disable RXEN and enable CAPT interrupt - disable transmitter
		UCSR0B &= ~(1 << RXEN0 | 1 << TXEN0);
		TIMSK1 |=  (1 << ICIE1);
	}
}