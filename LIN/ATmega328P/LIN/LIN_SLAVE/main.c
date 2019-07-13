#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "USART.h"

void init_Timer1(void);

int VALID_DATA;
void checksum(void);

int main()
 { 
	VALID_DATA = 0;		//assume that the data isn't valid

	INIT_UART(19200);
	
	init_Timer1();
	
	sei();
	
	while (1)
	{
		checksum();
	}
   return 0;
 }

void init_Timer1(void){
  TCCR1B &= ~(1 << ICES1); //triggers falling edge
  TIMSK1 |= 1 << ICIE1;
  TCCR1B |= 1 << CS10 ; // prescaler = 1
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