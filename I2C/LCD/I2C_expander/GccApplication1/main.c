/*
 * GccApplication1.c
 *
 * Created: 5/22/2019 5:36:03 PM
 * Author : Mohamed_Hassanin
 */ #include "Wire.h"
 #include "USART.h"
 #define F_CPU 16000000UL
 #include <avr/io.h>
 #include <util/delay.h>
 #include <avr/interrupt.h>
 #define DEVICE (0b0100111) // Device address as specified in data sheet

int main(void)
{	
	PORTC &= ~(1 << PINC4 | 1 << PINC5);
	beginAsMaster();
	sei();
    initUSART();
	
	beginTransmission(DEVICE);
	write(0b01111001);
	endTransmissionThenStop();
	while (1) 
    {
	}
}

