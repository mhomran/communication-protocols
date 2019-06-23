#ifndef _USART_H
#define _USART_H

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t data_rx;

void INIT_USART(void);
void USART_TX(char ch);
void USART_PRINTF(const char *str);
#endif

