#ifndef _lin_slave_h
#define _lin_slave_h

/* header file for LIN_MASTER.c */

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

/* Keywords extended by the compiler 
#todo: change them according to your target 
*/
#define ISR ISR																 //interrupt service routine function name
#define uart_rx_vector USART_RX_vect					 //UART receiver vector
#define timer0_overflow_vector TIMER0_OVF_vect 
  
#define F_CPU 8000000UL			//System Clock
#define M_ID_LEN_DEF 2			//number of IDs to be sent

//Slave flags
static volatile uint8_t wait_sync_break, wait_sync_field, wait_id, valid_id, receive, transmit, wait_checksum;

//slave id
static volatile uint8_t slave_id;

//data received
static volatile uint8_t data_rx, data_bytes_rx[9], data_len, data_index;

//data sent
static volatile uint8_t valid_data, data_bytes_tx[9], data_len_tx;

//Baud
static volatile uint16_t baud, baud_13;		//BAUD_13 is the baud of the 13 bit

//Master flags
static volatile uint8_t master_task, m_wait_sync_field, m_wait_id;

//IDs sent
static volatile uint8_t m_id[M_ID_LEN_DEF], m_id_index, m_id_len;

//checksum
void check_checksum (void);
void generate_checksum (void);

//LIN initialization
void init_lin_master (const int);
static void init_timer0 (void); 		//for scheduling
static void init_uart (void);
static void uart_tx (char);
static void uart_set_baud (int);

//parity bits
uint8_t generate_parity (uint8_t);
uint8_t check_parity (const uint8_t);
 
#endif	/* not _lin_slave_h */

