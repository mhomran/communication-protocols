/*
 * ADXL345.c
 *
 * Created: 6/24/2019 1:35:29 PM
 * Author : Mohamed_Hassanin
 */ 

#include <avr/io.h>
#define F_CPU 16000000ul
#include <util/delay.h>
#include "USART.h"
#include "ADXL345.h"

int main(void)
{
    INIT_UART();	
	INIT_ADXL();
	
	while (1) 
    {

	USART_PRINTN(X_AXIS());
	USART_TX(' ');
	USART_PRINTN(Y_AXIS());
	USART_TX(' ');
	USART_PRINTN(Z_AXIS());
	USART_TX('\n');
 	_delay_ms(100);
    }
}

