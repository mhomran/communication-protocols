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
	
	/*************************************************************/
	//frame with short sync break
	//8-bit frame - 0 parity - 1 start - 1 stop 
	//sync break
	USART_SET_BAUD(19200);			//must be less than or equal 11810 ~= 13 bit
	USART_TX(0x00);
	
	//8-bit frame - 0 parity - 1 start - 1 stop 
	//sync field
	USART_SET_BAUD(19200);
	USART_TX(0x55);
	
	//8-bit frame - 0 parity - 1 start - 1 stop 
	//identifier
	USART_TX(id);
	
	/*************************************************************/
	//frame with short sync break
	//8-bit frame - 0 parity - 1 start - 1 stop  
	//sync break
	USART_SET_BAUD(9600);			//must be less than or equal 11810 ~= 13 bit
	USART_TX(0x00);
	
	//8-bit frame - 0 parity - 1 start - 1 stop 
	//sync field
	USART_SET_BAUD(19200);
	USART_TX(0x22);
	
	//8-bit frame - 0 parity - 1 start - 1 stop 
	//identifier
	USART_TX(id);

	/*************************************************************/
	//correct header 
	//8-bit frame - 0 parity - 1 start - 1 stop 
	//sync break
	USART_SET_BAUD(9600);			//must be less than or equal 11810 ~= 13 bit
	USART_TX(0x00);
	
	//8-bit frame - 0 parity - 1 start - 1 stop 
	//sync field
	USART_SET_BAUD(19200);
	USART_TX(0x55);
	
	//8-bit frame - 0 parity - 1 start - 1 stop 
	//identifier
	USART_TX(id);

	//response
	
	//data(1)
	USART_TX(1);
	USART_TX(2);
	USART_TX(3);

	/*************************************************************/
	//another correct header 
	//8-bit frame - 0 parity - 1 start - 1 stop 
	//sync break
	USART_SET_BAUD(9600);			//must be less than or equal 11810 ~= 13 bit
	USART_TX(0x00);
	
	//8-bit frame - 0 parity - 1 start - 1 stop 
	//sync field
	USART_SET_BAUD(19200);
	USART_TX(0x55);
	
	//8-bit frame - 0 parity - 1 start - 1 stop 
	//identifier
	USART_TX(id);

	//another response
	
	//data(2)
	USART_TX(4);
	USART_TX(5);
	USART_TX(6);

	
   return 0;
 }