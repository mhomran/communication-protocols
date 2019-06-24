
/*
 * SPI.c
 *
 * Created: 6/24/2019 1:41:14 PM
 *  Author: Mohamed_Hassanin
 */ 
#include "SPI.h"

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>


void SPI_MasterInit(void)
{
	/* Set MOSI and SCK output, all others input */
	DDRB |= 1 << SS | 1 << PINB3 | 1 << PINB5;
	DDRD &= ~(1 << PINB4);			//MISO
	PORTB |= 1 << SS;	//pull SS HIGH
	/* Enable SPI, Master, set clock rate fck/16 */
	//idle->HIGH , data is sample at the rising edge.
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0) | 1 << CPOL | 1 << CPHA;
}

void SPI_MasterTransmit(char cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
}

