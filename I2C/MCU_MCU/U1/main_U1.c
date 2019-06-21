//Master transmitter Mode.

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <compat/twi.h>
#define F_CPU 16000000ul
#include <util/delay.h>
volatile uint8_t twi_data;
volatile uint8_t stop = 0;

int main()
 { 
	_delay_ms(100);
	PORTC |= 1 << PINC4 | 1 << PINC5;	//enable pull up on SCL and SDA
	MCUCR &= ~(1 << PUD);
	sei();								//enable global interrupts
	TWCR = 0b01100101;					//ACK-enable-interrupt_enable-Start
	TWSR &= ~(1 << TWPS0 | 1 << TWPS1);
	TWBR = 72;
		
	twi_data = 0b01010101;
	TWCR |= 1 << TWINT;
	
	while (1);
   return 0;
 }

ISR(TWI_vect)
{
	switch(TW_STATUS){
    // All Master
    case TW_START:     // sent start condition
    case TW_REP_START: // sent repeated start condition
    TWDR = 0b01000001;
	TWCR |= 1 << TWINT;	//clear
	break; 
	
    // Master Transmitter
    case TW_MT_SLA_ACK:  // slave receiver acked address
    case TW_MT_DATA_ACK: // slave receiver acked data
    if(stop){
	TWCR |= 1 << TWSTO | 1 << TWINT;
	stop = 0;
	}
	else {
	TWDR = twi_data;
	TWCR |= 1 << TWINT;	//clear
	stop =1;
	}
	break;

    // Master Receiver
    case TW_MR_DATA_ACK: // data received, ack sent
    break;
    case TW_MR_DATA_NACK: // data received, nack sent
      // put final byte into buffer
    twi_data = TWDR;
	TWCR |= 1 << TWSTO | 1 << TWINT;
	break;
	case TW_MR_SLA_ACK:  // address sent, ack received
    TWCR &=~(1 << TWEA);
	TWCR |= 1 << TWINT;
	break;
	}
}