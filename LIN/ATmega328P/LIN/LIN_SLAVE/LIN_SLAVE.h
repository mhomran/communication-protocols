#ifndef _LIN_SLAVE_H
#define _LIN_SLAVE_H

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000ul

volatile uint8_t WAIT_SYNC_BREAK, WAIT_SYNC_FIELD, WAIT_ID, RECEIVE, DATA_LEN, DATA_INDEX,VALID_ID , slave_id;
volatile uint8_t VALID_DATA;

void checksum(void);

volatile uint8_t data_bytes_TX[9];
volatile uint8_t data_bytes_RX[9];

volatile uint8_t WAIT_CHECKSUM;

volatile uint8_t data_rx;
void INIT_UART(int);
void USART_TX(char ch);
void USART_SET_BAUD(int);

#endif

