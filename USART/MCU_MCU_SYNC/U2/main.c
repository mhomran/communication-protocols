#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "USART.h"

int main()
 { 
	INIT_USART();
	while (1)
      ;
   return 0;
 }