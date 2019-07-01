
/*
 * SPI.h
 *
 * Created: 6/24/2019 1:41:26 PM
 *  Author: Mohamed_Hassanin
 */ 

#ifndef _SPI_H
#define _SPI_H

#define SS PINB2

#include <avr/io.h>

void SPI_MasterTransmit(char cData);
void SPI_MasterInit(void);

#endif