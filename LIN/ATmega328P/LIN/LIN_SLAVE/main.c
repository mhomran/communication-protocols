#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "LIN_SLAVE.h"

void init_Timer1(void);


int main()
 { 

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

