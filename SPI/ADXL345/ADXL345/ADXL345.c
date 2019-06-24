
/*
 * ADXL345.c
 *
 * Created: 6/24/2019 4:17:12 PM
 *  Author: Mohamed_Hassanin
 */ 
#include "ADXL345.h"

void INIT_ADXL(){
	SPI_MasterInit();
	PORTB |= 1 << SS;
	DDRB |= 1 << SS;

	PORTB &= ~(1 << SS);
	SPI_MasterTransmit(0b00111111 & DATA_FORMAT);
	SPI_MasterTransmit(0x01);
	PORTB |= 1 << SS;

	PORTB &= ~(1 << SS);
	SPI_MasterTransmit(0b00111111 & POWER_CTL);
	SPI_MasterTransmit(0x08);
	PORTB |= 1 << SS;
}

char X_AXIS(void){
	PORTB &= ~(1 << SS);
	SPI_MasterTransmit(DATAX0 | 0x80);
	SPI_MasterTransmit(0xFF);			//dummy
	PORTB |= 1 << SS;

	return SPDR;
}

char Y_AXIS(void){
	PORTB &= ~(1 << SS);
	SPI_MasterTransmit(DATAY0 | 0x80);
	SPI_MasterTransmit(0xFF);			//dummy
	PORTB |= 1 << SS;
	uint8_t data = SPDR;
	
	return SPDR;	
}

char Z_AXIS(void){
	PORTB &= ~(1 << SS);
	SPI_MasterTransmit(DATAZ0 | 0x80);
	SPI_MasterTransmit(0xFF);			//dummy
	PORTB |= 1 << SS;

	return SPDR;
}