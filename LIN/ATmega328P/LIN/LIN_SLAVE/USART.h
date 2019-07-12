#ifndef _USART_H
#define _USART_H

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000ul

volatile uint8_t WAIT_SYNC_BREAK, WAIT_SYNC_FIELD, WAIT_ID, RECEIVE, DATA_LEN, DATA_INDEX,VALID_ID , slave_id;
volatile uint8_t data_bytes[9];

volatile uint8_t data_rx;
void INIT_UART(int);
void USART_TX(char ch);
void USART_SET_BAUD(int);
void USART_PRINTF(const char *str);
void USART_PRINTN(uint8_t ch);

#endif

