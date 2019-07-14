#ifndef _LIN_SLAVE_H
#define _LIN_SLAVE_H

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000ul

volatile uint8_t WAIT_SYNC_BREAK, WAIT_SYNC_FIELD, WAIT_ID, RECEIVE,TRANSMIT, DATA_LEN, DATA_INDEX,VALID_ID , slave_id;
volatile uint8_t VALID_DATA;
volatile uint8_t data_bytes_TX[9],DATA_LEN_TX;
volatile uint8_t data_bytes_RX[9];
volatile uint8_t WAIT_CHECKSUM;
volatile uint8_t data_rx;

//relates to MASTER
volatile uint8_t MASTER_TASK;
volatile uint8_t M_WAIT_SYNC_FIELD, M_WAIT_ID;
#define M_ID_LEN_DEF 2
volatile uint8_t M_ID[M_ID_LEN_DEF], M_ID_INDEX, M_ID_LEN;

void checksum(void);
void generate_checksum(void);
void init_Timer0(void); 		//for scheduling
void INIT_UART(int);
void USART_TX(char);
void USART_SET_BAUD(int);
uint8_t generate_parity(uint8_t);

#endif

