#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "LIN_MASTER.h"


int main()
 { 
	
	INIT_LIN_MASTER(19200);

	while(1);

	return 0;
 }

