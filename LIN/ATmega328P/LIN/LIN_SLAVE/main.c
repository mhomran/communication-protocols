#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "LIN_SLAVE.h"


int main()
 { 
	
	INIT_LIN_SLAVE(19200);
	 
	while (1)
	{
		//waiting for interruption
	}
   return 0;
 }



