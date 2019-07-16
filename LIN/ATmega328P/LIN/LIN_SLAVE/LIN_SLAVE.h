#ifndef _LIN_SLAVE_H
#define _LIN_SLAVE_H

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 8000000

volatile uint8_t WAIT_SYNC_BREAK, WAIT_SYNC_FIELD, WAIT_ID, RECEIVE, DATA_LEN, DATA_INDEX,VALID_ID , slave_id;
volatile uint8_t VALID_DATA;
volatile uint8_t data_bytes_TX[9],DATA_LEN_TX;
volatile uint8_t data_bytes_RX[9];
volatile uint8_t WAIT_CHECKSUM;
volatile uint8_t TRANSMIT;
volatile int BAUD, BAUD_13; //BAUD_13 is the baud of the 13 bit
volatile int PS;			//prescaler

void generate_checksum(void);
void checksum(void);
void init_Timer1(void);

uint8_t check_parity(uint8_t id);

volatile uint8_t data_rx;
void INIT_LIN_SLAVE(int);
void USART_TX(char ch);
void USART_SET_BAUD(int);

#endif

