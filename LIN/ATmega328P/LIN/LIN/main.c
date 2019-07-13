#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "USART.h"

volatile int DATA_LEN;
volatile int data_bytes_TX[9];

void generate_checksum(void);
void send_response(void);

int main()
 { 
	INIT_UART(2000);
	uint8_t id = 0x02;
	DATA_LEN = 3;
	
	uint8_t P0 = 0 , P1 =0;
	//calculate P0 , P1 
	P0 = (id & 0x1) ^ (id & 0x2) ^ (id & 0x4) ^ (id  & 0x8) ^ (id & 0x10);
	P1 = !((id & 0x2) ^ (id & 0x4) ^ (id  & 0x8) ^ (id & 0x10) ^ (id & 0x20));
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
	
	data_bytes_TX[0] = 2;
	data_bytes_TX[1] = 3;
	send_response();
	
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
	data_bytes_TX[0] = 4;
	data_bytes_TX[1] = 5;
	send_response();
	
   return 0;
 }

void send_response()
{	
	generate_checksum();
		
	int i = 0;
	
	while(i < DATA_LEN)
	{
		USART_TX(data_bytes_TX[i]);
		i++;
	}
}

void generate_checksum(void){
	int sum = 0, i = 0;
	while(i < (DATA_LEN-1) )
	{
		sum += data_bytes_TX[i];
		if(sum >= 256)
		{
			sum &= 0xFF;
			sum ++;
		}
		i++;
	}

	sum = ~sum;
	sum &= 0xFF;
	
	data_bytes_TX[DATA_LEN-1] = sum;
}