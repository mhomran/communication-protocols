#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "USART.h"


int main()
{
	DDRD |= 1 << PIND1;
	PORTD = 1 << PIND1;
	INIT_UART(2000);
	uint8_t id = 0x02;
	uint8_t P0 = 0 , P1 =0;
	//calculate P0 , P1
	P0 = (id & 0x1) ^ (id & 0x2) ^ (id & 0x4) ^ (id  & 0x8) ^ (id & 0x10);
	P1 = ~((id & 0x2) ^ (id & 0x4) ^ (id  & 0x8) ^ (id & 0x10) ^ (id & 0x20));
	id = id | (P1 << 8) | (P0 << 7);
	
	//8-bit frame - 0 parity - 1 start - 2 stop -
	//sync break
	USART_SET_BAUD(5000);			//must be less than or equal 11810 ~= 13 bit
	USART_TX(0x00);
	
	//8-bit frame - 0 parity - 1 start - 1 stop - BR 9600
	//sync field
	USART_SET_BAUD(19200);
	USART_TX(0x55);
	
	//8-bit frame - 0 parity - 1 start - 1 stop - BR 9600
	//identifier
	USART_TX(id);
	
	while (1)
	;
	return 0;
}