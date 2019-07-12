#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "USART.h"

int main()
 { 
	INIT_UART(6400);
	uint8_t id = 0x02;
	uint8_t P0 = 0 , P1 =0;
	//calculate P0 , P1 
	P0 = (id & 0x1) ^ (id & 0x2) ^ (id & 0x4) ^ (id  & 0x8) ^ (id & 0x10);
	P1 = ~((id & 0x2) ^ (id & 0x4) ^ (id  & 0x8) ^ (id & 0x10) ^ (id & 0x20));
	id = id | (P1 << 8) | (P0 << 7);
	
	//8-bit frame - 0 parity - 1 start - 1 stop 
	//sync break
	USART_SET_BAUD(6400);			//must be less than or equal to 6400
	USART_TX(0x00);
	
	//8-bit frame - 0 parity - 1 start - 1 stop 
	//sync field
	USART_SET_BAUD(9600);
	USART_TX(0x55);

	//8-bit frame - 0 parity - 1 start - 1 stop 
	//identifier
	USART_TX(id);
	
	while (1)
      ;
   return 0;
 }