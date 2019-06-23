#include "USART.h"


void INIT_USART(void){
	UBRR0 = 207;	
	DDRD &= ~(1 << PIND4);
	UCSR0B |= 1 << TXEN0 | 1 << RXEN0;
	UCSR0B &= ~(1 << UCSZ02);
	UCSR0C |= 1 << UCSZ00 | 1 << UCSZ01 | 1 << UMSEL00;
	sei();
}

void USART_TX(char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

void USART_PRINTF(const char *str)
{
	int x =0;
	while (str[x]){
	USART_TX(str[x]);
	x++;
	}
}

