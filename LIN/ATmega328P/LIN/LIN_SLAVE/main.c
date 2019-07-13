#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "USART.h"

void init_Timer1(void);

int main()
{
	INIT_UART(19200);
	
	init_Timer1();
	
	sei();
	
	while (1)
	;
	return 0;
}

void init_Timer1(void){
	TCCR1B &= ~(1 << ICES1); //triggers falling edge
	TIMSK1 |= 1 << ICIE1;
	TCCR1B |= 1 << CS10 ; // prescaler = 1
}

volatile int x;

ISR(TIMER1_CAPT_vect){

	TIFR1 |= 1 << TOV1;		//clear overflow flag

	if( !(TCCR1B & (1 << ICES1)) ){
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
	}
}
