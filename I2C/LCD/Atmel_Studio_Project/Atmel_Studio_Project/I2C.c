
#include "I2C.h"


void INIT_I2C(uint8_t ADDR_fn){
	PORTC |= 1 << PINC4 | 1 << PINC5;	//enable pull up on SCL and SDA
	MCUCR &= ~(1 << PUD);
	sei();								//enable global interrupts
	TWCR = 0b01000101;					//ACK-enable-interrupt_En
	TWSR &= ~(1 << TWPS0 | 1 << TWPS1);	//prescaler 1
	TWBR = 72;
	ADDR = ADDR_fn << 1;
	stop =0;
}

void I2C_SEND(uint8_t data){
	twi_data = data;
	TWCR |= 1 << TWINT | 1 << TWSTA;
}

ISR(TWI_vect)
{
	switch(TW_STATUS){
    // All Master
    case TW_START:     // sent start condition
    case TW_REP_START: // sent repeated start condition
    TWDR = ADDR;
	TWCR |= 1 << TWINT;	//clear
	break; 
	
    // Master Transmitter
    case TW_MT_SLA_ACK:  // slave receiver acked address
    case TW_MT_DATA_ACK: // slave receiver acked data

	if(stop){
	TWCR &= ~(1 << TWSTA);
	TWCR |= 1 << TWSTO | 1 << TWINT;
	stop = 0;
	}
	else {
	TWDR = twi_data;
	TWCR |= 1 << TWINT;	//clear
	stop =1;
	}
	break;
	}
}