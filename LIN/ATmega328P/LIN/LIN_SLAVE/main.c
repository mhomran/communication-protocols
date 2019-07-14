#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "LIN_SLAVE.h"


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



