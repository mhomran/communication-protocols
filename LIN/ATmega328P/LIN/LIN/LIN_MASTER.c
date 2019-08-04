/* this is a driver for a lin communication as a master */

#include "LIN_MASTER.h"

/*
Description : initialize UART peripheral 
Inputs		  : None
Outputs	    : None
 
*/

static void 
init_uart(void)
{
	UCSR0B |= (uint8_t) (1 << TXEN0) | (uint8_t) (1 << RXEN0) | (uint8_t) (1 << RXCIE0);
	UCSR0B &= (uint8_t) (~ ((uint8_t) (1 << UCSZ02)));
	UCSR0C |= (uint8_t) (1 << UCSZ00) | (uint8_t) (1 << UCSZ01);
	sei ();
}

/*
Description : initialize LIN communication as a master.	
Inputs			: baud_fn: the baud rate of the communication.
Outputs			: None


*/

void 
init_lin_master (const int baud_fn)
{
	baud = baud_fn;
	baud_13 = baud / 2;			//traditionally it's the half speed of the normal speed according to vector company
	
	init_uart ();
		
	init_timer0 ();

	//initialize data
	wait_sync_field = 0;
	wait_sync_break = 0;
	wait_id = 0;
	receive = 0;
	transmit = 0;
	data_len = 0;
	data_index = 0;
	valid_id = 0;
	valid_data = 0;		//assume that the data isn't valid
	slave_id = 0x2;
	wait_checksum = 0;

	master_task = 0;
	m_wait_sync_field = 0;
	m_wait_id = 0;
	m_id_index = 0;
	m_id_len = 2;
	m_id[0] = generate_parity (0x02);
	m_id[1] = generate_parity (0x01);

	data_len_tx = 3;
	data_bytes_tx[0] = 0x1;
	data_bytes_tx[1] = 0x1;
	generate_checksum ();
}

/*
Description : Generate the two parity bits of IDs
Inputs      : id: the id to attach parity bits to.
Outputs     : id: the id after attaching the parity bits.


*/

uint8_t 
generate_parity (uint8_t id)
{
	uint8_t P0, P1;
	P0 = P1 = 0;
	
	//calculate P0 , P1
	P0 = ((id & 0x1) >> 0) ^ ((id & 0x2) >> 1) ^ ((id & 0x4) >> 2) ^ ((id  & 0x8) >> 3) ^ ((id & 0x10) >> 4);
	P1 = !(((id & 0x2) >> 1) ^ ((id & 0x4) >> 2) ^ ((id  & 0x8) >> 3) ^ ((id & 0x10) >> 4) ^ ((id & 0x20) >> 5));
	id = id | (uint8_t) (P1 << 7) | (uint8_t) (P0 << 6);
	
	return id;
}

/*
Description : check the incoming parity bits
Inputs      : id: the incoming id
Outputs     :	flag: the condition of validity of the id. '1' means valid. '0' mean invalid.


*/

uint8_t 
check_parity (const uint8_t id)
{
	uint8_t P0, P1;
	P0 = P1 = 1;
	
	//calculate P0 , P1
	P0 = ((id & 0x1) >> 0) ^ ((id & 0x2) >> 1) ^ ((id & 0x4) >> 2) ^ ((id  & 0x8) >> 3) ^ ((id & 0x10) >> 4);
	P1 = !(((id & 0x2) >> 1) ^ ((id & 0x4) >> 2) ^ ((id  & 0x8) >> 3) ^ ((id & 0x10) >> 4) ^ ((id & 0x20) >> 5));
	
	uint8_t flag = 0;
	if ((P1 == ((id & 0x80) >> 7)) && (P0 == ((id & 0x40) >> 6)))
		{
			flag = 1;
		}
	
	return flag;
}

/*
Description : send a byte using UART
Inputs      : data: data to be sent
Outputs     : None


*/

static void 
uart_tx (char data )
{
	UDR0 = data;
}

/*
Description : set the baud rate of the UART
Inputs      : baud_fn: the baud to be set
Outputs     : None


*/

static void 
uart_set_baud (int baud_fn)
{
	UBRR0 = (F_CPU/16/baud_fn) - 1;
}

/*******************************************ISRs***********************************/
/*
Description : the uart receiver ISR


*/

ISR (uart_rx_vector)
{				
	data_rx = UDR0;
	if (wait_sync_break != 0)
		{
		wait_sync_field = 1;
		wait_sync_break = 0;	
		}											
	else if (wait_sync_field != 0)
		{
			if(data_rx == 0x55)
				{
					wait_sync_field = 0;
					wait_id = 1;
				}
			else
				{	//RETURN BACK						
					wait_sync_field = 0;		
				}
		}
	else if (wait_id != 0)
		{		 
			valid_id = 1;
			if (check_parity (data_rx))
				{
					data_rx = data_rx & 0x3F;
		
					if (data_rx >= 0 && data_rx < 32)
						{
							data_len = 3;
						}
					else if (data_rx >= 32 && data_rx < 48)
						{
							data_len = 5;
						}
					else if (data_rx >= 48 && data_rx < 64)
						{
							data_len = 9;
						}
					else 
						{
							valid_id = 0;
						}
				}
			else 
				{
					valid_id = 0;
				}
			
			if (valid_id != 0)
				{		
					if (data_rx == slave_id)			//transmit
						{
							transmit = 1;
						
							UDR0 = data_bytes_tx[0];
							data_index = 1;
						}
					else
						{
							receive = 1;
							data_index = 0;
							wait_checksum = 0;
						}
				}
		
		wait_id = 0;
	}
	else if (receive != 0) 
		{
			data_bytes_rx[data_index] = data_rx;
			data_index++;
	
			if (data_index == data_len)
			{
				receive = 0;		
				wait_checksum = 1;
			}	
		}
	else if (transmit != 0)
		{
			UDR0 = data_bytes_tx[data_index];
			data_index++;
			
			if (data_index == data_len_tx)
				{
					transmit = 0;
				}	
		}
	else 
		{
			/* NOT REACHED */
		}
	
	if (master_task != 0)
		{	
			if (m_wait_sync_field != 0)
				{
					uart_set_baud (baud);
					uart_tx (0x55);
			
					m_wait_id = 1;
					m_wait_sync_field = 0;	
				}
			else if (m_wait_id != 0)
				{
					uart_tx (m_id[m_id_index]);
					m_id_index++;

					if (m_id_index == m_id_len)
						m_id_index = 0;
			
					m_wait_id = 0;
					master_task = 0;
			}
	}
}

/*
Description : Timer0 overflow ISR


*/

ISR (timer0_overflow_vector)
{
	uart_set_baud (baud_13);
	uart_tx (0x00);
	master_task = 1;
	m_wait_sync_field = 1;
	wait_sync_break = 1;
	receive = 0;				//in case didn't receive
}
/**************************************************************************************/

/*
Description : check the received checksum
Inputs      : None
Outputs     :	None


*/

void 
check_checksum (void)
{
	uint8_t i = 0;
	int sum = 0;
	if (wait_checksum)
	{
		while (i < (data_len-1) )
		{
			sum += (int)data_bytes_rx[i];
			if (sum >= (int) (256))
			{
				sum &= (int)0xFF;
				sum ++;
			}
			i++;
		}

		sum = ~sum;
		sum &= 0xFF;

		if (sum == data_bytes_rx[data_len-1])
		{
			valid_data = 1;		
		}
		else
		{
			valid_data = 0;		
		}

		wait_checksum = 0;
	}
}

/*
Description : Generate a checksum to the data sent
Inputs      : None
Outputs     :	None


*/
	
void
generate_checksum (void)
{
	int sum = 0;
	uint8_t i = 0;
	while (i < (data_len_tx-1))
	{
		sum += (int)data_bytes_tx[i];
		if (sum >= (int)256)
		{
			sum &= 0xFF;
			sum ++;
		}
		i++;
	}

	sum = ~sum;
	sum &= 0xFF;
	
	data_bytes_tx[data_len_tx-1] = (uint8_t)sum;
}

/*
Description : initialize timer0
Inputs      : None
Outputs     : None


*/

static void 
init_timer0 (void)
{	
	//FAST PWM OVF set on TOP -> OCR0A
	TCCR0A |= (uint8_t) (1 << WGM01) | (uint8_t) (1 << WGM00);
	TCCR0B |= (uint8_t) (1 << WGM02);
	int prescaler = 1024;
	uint8_t frame_width = (uint8_t)ceil (F_CPU / prescaler / (((unsigned long) (baud) * baud_13)
																			/ ((unsigned long) (50) * baud_13 + 10 * baud))); 
	OCR0A = frame_width;
	TIMSK0 |= (uint8_t) (1 << TOIE0);				//timer0 overflow
	TCCR0B |= (uint8_t) (1 << CS02) | (uint8_t) (1 << CS00);	//prescaler 1024
}

