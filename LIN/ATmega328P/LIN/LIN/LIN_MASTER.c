#include "LIN_MASTER.h"


void INIT_UART(int BAUD){
	UBRR0 = (F_CPU/16/BAUD)-1;
	UCSR0B |= 1 << TXEN0 | 1 << RXEN0 | 1 << RXCIE0;
	UCSR0B &= ~(1 << UCSZ02);
	UCSR0C |= 1 << UCSZ00 | 1 << UCSZ01;
	sei();

	//initialize data
	WAIT_SYNC_FIELD = 0;
	WAIT_SYNC_BREAK = 0;
	WAIT_ID = 0;
	RECEIVE = 0;
	TRANSMIT = 0;
	DATA_LEN = 0;
	DATA_INDEX = 0;
	VALID_ID = 0;
	VALID_DATA = 0;		//assume that the data isn't valid
	slave_id = 0x2;
	WAIT_CHECKSUM = 0;

	MASTER_TASK = 0;
	M_WAIT_SYNC_FIELD = 0;
	M_WAIT_ID = 0;
	M_ID_INDEX = 0;
	M_ID_LEN = 2;
	M_ID[0] = generate_parity(0x02);
	M_ID[1] = generate_parity(0x01);

	DATA_LEN_TX = 3;
	data_bytes_TX[0] = 0x1;
	data_bytes_TX[1] = 0x1;
	generate_checksum();

}

uint8_t generate_parity(uint8_t id){
	uint8_t P0 = 0 , P1 =0;
	//calculate P0 , P1
	P0 = (id & 0x1) ^ (id & 0x2) ^ (id & 0x4) ^ (id  & 0x8) ^ (id & 0x10);
	P1 = !((id & 0x2) ^ (id & 0x4) ^ (id  & 0x8) ^ (id & 0x10) ^ (id & 0x20));	
	id = id | (P1 << 8) | (P0 << 7);
	return id;
}

void USART_TX(char data )
{
	/*//you don't need it anymore
	// Wait for empty transmit buffer 
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	*/
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

void USART_SET_BAUD(int BAUD_FN){
UBRR0 = (F_CPU/16/BAUD_FN)-1;
}
/*******************************************ISRs***********************************/
ISR(USART_RX_vect){
				
	data_rx = UDR0;
	if (WAIT_SYNC_BREAK)
	{
		WAIT_SYNC_FIELD = 1;
		WAIT_SYNC_BREAK = 0;	
	}											
	else if (WAIT_SYNC_FIELD){
		if(data_rx == 0x55){
			WAIT_SYNC_FIELD = 0;
			WAIT_ID = 1;
		}
		else
		{	//RETURN BACK						
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
				TRANSMIT = 1;
						
				UDR0 = data_bytes_TX[0];
				DATA_INDEX = 1;
			}
			else{
				RECEIVE = 1;
				DATA_INDEX = 0;
				WAIT_CHECKSUM = 0;
			}
		}
		
		WAIT_ID = 0;
	}
	else if (RECEIVE) 
	{
		PORTD ^= 1 << PIND7;
		
		data_bytes_RX[DATA_INDEX] = data_rx;
		DATA_INDEX++;
	
		if(DATA_INDEX == DATA_LEN)
		{
			RECEIVE = 0;		
			WAIT_CHECKSUM = 1;
		}	
	}
	else if(TRANSMIT)
	{
		UDR0 = data_bytes_TX[DATA_INDEX];
		DATA_INDEX++;
			
		if(DATA_INDEX == DATA_LEN_TX)
		{
			TRANSMIT = 0;
		}	
	}
	
	
	if(MASTER_TASK)
	{	
		if(M_WAIT_SYNC_FIELD)
		{
			USART_SET_BAUD(19200);
			USART_TX(0x55);
			
			M_WAIT_ID = 1;
			M_WAIT_SYNC_FIELD = 0;	
		}
		else if (M_WAIT_ID)
		{
			USART_TX(M_ID[M_ID_INDEX]);
			M_ID_INDEX++;

			if(M_ID_INDEX == M_ID_LEN)
				M_ID_INDEX = 0;
			
			M_WAIT_ID = 0;
			MASTER_TASK = 0;
		}
	}
}


ISR(TIMER0_OVF_vect){
	USART_SET_BAUD(9600);
	USART_TX(0x00);
	MASTER_TASK = 1;
	M_WAIT_SYNC_FIELD = 1;
	WAIT_SYNC_BREAK = 1;
	RECEIVE = 0;				//in case didn't receive
}
/**************************************************************************************/

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

void generate_checksum(void){
	int sum = 0, i = 0;
	while(i < (DATA_LEN_TX-1) )
	{
		sum += data_bytes_TX[i];
		if(sum >= 256)
		{
			sum &= 0xFF;
			sum ++;
		}
		i++;
	}

	sum = ~sum;
	sum &= 0xFF;
	
	data_bytes_TX[DATA_LEN_TX-1] = sum;
}

void init_Timer0(void){	//FAST PWM OVF set on TOP -> OCR0A
	TCCR0A |= 1 << WGM01 | 1 << WGM00;
	TCCR0B |= 1 << WGM02;
	OCR0A = 57;
	TIMSK0 |= 1 << TOIE0;				//timer0 overflow
	TCCR0B |= 1 << CS02 | 1 << CS00;	//prescaler 1024
}
