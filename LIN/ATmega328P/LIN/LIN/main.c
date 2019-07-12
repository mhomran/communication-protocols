/*
 * LIN.c
 *
 * Created: 7/12/2019 3:17:23 AM
 * Author : Mohamed_Hassanin
 */ 

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "USART.h"

int main()
{

	INIT_UART(19200);
	uint8_t id = 0x02;
	uint8_t P0 = 0 , P1 =0;
	//calculate P0 , P1
	P0 = (id & 0x1) ^ (id & 0x2) ^ (id & 0x4) ^ (id  & 0x8) ^ (id & 0x10);
	P1 = ~((id & 0x2) ^ (id & 0x4) ^ (id  & 0x8) ^ (id & 0x10) ^ (id & 0x20));
	id = id | (P1 << 8) | (P0 << 7);
	
	//8-bit frame - 0 parity - 1 start - 1 stop - BR 19200
	//sync break
	USART_SET_BAUD(19200);
	USART_TX(0x00);
	
	//8-bit frame - 0 parity - 1 start - 1 stop - BR 9600
	//sync field
	USART_SET_BAUD(9600);
	USART_TX(0x55);

	//8-bit frame - 0 parity - 1 start - 1 stop - BR 9600
	//identifier
	USART_TX(id);
	
	while (1)
	;
	return 0;
}