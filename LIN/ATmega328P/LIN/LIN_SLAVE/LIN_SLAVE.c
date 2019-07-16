#include "LIN_SLAVE.h"


void INIT_LIN_SLAVE(int BAUD_fn	){
	BAUD = BAUD_fn;
	BAUD_13 = BAUD/13;
	
	UBRRL = (F_CPU/16/BAUD)-1;
	UCSRB |= 1 << RXCIE | 1 << TXEN;					//enabling the transmitter causing problems.
	UCSRB &= ~(1 << UCSZ2);
	UCSRC |= 1 << UCSZ0 | 1 << UCSZ1 | 1 << URSEL;
	sei();
	
	init_Timer1();
	
	//initialize data
	WAIT_SYNC_BREAK = 1;
	WAIT_SYNC_FIELD = 0;
	WAIT_ID, RECEIVE = 0;
	DATA_LEN = 0;
	DATA_INDEX = 0;
	VALID_ID = 0;
	VALID_DATA = 0;		//assume that the data isn't valid
	slave_id = 0x01;
	WAIT_CHECKSUM = 0;
	TRANSMIT = 0;

	DATA_LEN_TX = 3;
	data_bytes_TX[0] = 0x11;
	data_bytes_TX[1] = 0x22;
	generate_checksum();
}

void USART_TX(char data )
{
	// Wait for empty transmit buffer 
	while ( !( UCSRA & (1<<UDRE)) )
	;
	/* Put data into buffer, sends the data */
	UDR = data;
}

void USART_SET_BAUD(int BAUD_FN){
UBRRL = (F_CPU/16/BAUD_FN)-1;
}

ISR(USART_RXC_vect){
	data_rx = UDR;
						
	if (WAIT_SYNC_FIELD){
			
		if(data_rx == 0x55){
			WAIT_ID = 1;
		}
		else
		{	//RETURN BACK
			//disable RXEN and enable CAPT interrupt
			UCSRB &= ~(1 << RXEN);
			
			TIFR  |= 1 << ICF1;
			TIMSK |= (1 << TICIE1);
		}
		WAIT_SYNC_FIELD = 0;
	}
	
	else if (WAIT_ID){		//data + 1(checksum) 
		VALID_ID = 1;
		
		if(check_parity(data_rx)){
			data_rx = data_rx & 0x3F;
			
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
		} else {VALID_ID = 0;}
			
		if (VALID_ID){		
			if(data_rx == slave_id)			//transmit
			{	
							
				TRANSMIT = 1;
					
				USART_TX( data_bytes_TX[0]);
				DATA_INDEX = 1;
			}
			else {				
				RECEIVE = 1;
				DATA_INDEX = 0;
				WAIT_CHECKSUM = 0;
			}
		}
		else{
				//disable RXEN and enable CAPT interrupt
				UCSRB &= ~(1 << RXEN);
			
				TIFR  |= 1 << ICF1;
				TIMSK |= (1 << TICIE1);
		}
		WAIT_ID = 0;	
	}
	
	else if (RECEIVE) 
	{
		data_bytes_RX[DATA_INDEX] = data_rx;
		DATA_INDEX++;
	
		if(DATA_INDEX == DATA_LEN)
		{
			RECEIVE = 0;
			//disable RXEN and enable CAPT interrupt
			UCSRB &= ~(1 << RXEN);
			
			TIFR  |= 1 << ICF1;
			TIMSK |= (1 << TICIE1);
			
			WAIT_CHECKSUM = 1;
		}	
	}
	else if(TRANSMIT)
	{
		PORTD  ^= 1 << PIND4;
		
		UDR = data_bytes_TX[DATA_INDEX];
		DATA_INDEX++;
		
		if(DATA_INDEX == DATA_LEN_TX)
		{
			TRANSMIT = 0;
			
			UCSRB &= ~(1 << RXEN);
			
			TIFR  |= 1 << ICF1;
			TIMSK |= (1 << TICIE1);
		}
	} 
}

ISR(TIMER1_CAPT_vect){
				
	if( !(TCCR1B & (1 << ICES1)) ){			//falling ?
		
		TCCR1B |= (1 << ICES1); //triggers rising edge
		TCNT1 = 0;				//clear timer's value
	}
	else
	{
		 		
		if ( ( (ICR1L | (ICR1H << 8)) >= (int)(F_CPU/(BAUD_13/PS)) ) || (TIFR & (1 << TOV1)) ){		//F_CPU/(1/(13/19200))
				
			WAIT_SYNC_FIELD = 1;
			//enable RXEN and disable CAPT interrupt
			UCSRB |= 1 << RXEN;
			TIMSK &= ~(1 << TICIE1);
		}
		TCCR1B &= ~(1 << ICES1); //triggers falling edge
	}
	TIFR |= 1 << TOV1;		//clear overflow flag
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
void init_Timer1(void){
	TCCR1B &= ~(1 << ICES1); //triggers falling edge
	PS = 1;
	TIMSK |= 1 << TICIE1;
	TCCR1B |= 1 << CS10 ; // prescaler = 1
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

uint8_t check_parity(uint8_t id){
	uint8_t P0 = 0 , P1 =0;
	//calculate P0 , P1
	P0 = ((id & 0x1) >> 0) ^ ((id & 0x2) >> 1) ^ ((id & 0x4) >> 2) ^ ((id  & 0x8) >> 3) ^ ((id & 0x10) >> 4);
	P1 = !( ((id & 0x2) >> 1) ^ ((id & 0x4) >> 2) ^ ((id  & 0x8) >> 3) ^ ((id & 0x10) >> 4) ^ ((id & 0x20) >> 5) );
	if( ( P1 == ( (id & 0x80) >> 7 ) ) && ( P0 == ( (id & 0x40) >> 6 ) )){
		return 1;
	}
	else {
		return 0;
	}
}
