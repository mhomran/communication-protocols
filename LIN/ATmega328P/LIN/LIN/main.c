#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "LIN_MASTER.h"


int main()
 { 
	INIT_UART(9600);

	init_Timer1();
	
	init_Timer0();

	sei();

	while(1);

	return 0;
 }

