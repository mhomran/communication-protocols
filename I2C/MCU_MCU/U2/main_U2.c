//Slave receiver
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <compat/twi.h>

volatile uint8_t twi_data;

int main()
 { 
   // Write your code here
	
	PORTC |= 1 << PINC4 | 1 << PINC5;	//enable pull up on SCL and SDA
	MCUCR &= ~(1 << PUD);
	sei();								//enable global interrupts
	
	TWAR = 0b01000001;					//slave address + general call response
	TWCR = 0b01000101;					//enable-ac	ks-enable_interrupts
	
   while (1);
   return 0;
 }


ISR(TWI_vect)
{
	switch(TW_STATUS){
    // Slave Receiver
    case TW_SR_SLA_ACK:   // addressed, returned ack
    case TW_SR_GCALL_ACK: // addressed generally, returned ack
    case TW_SR_ARB_LOST_SLA_ACK:   // lost arbitration, returned ack
    case TW_SR_ARB_LOST_GCALL_ACK: // lost arbitration, returned ack
      // enter slave receiver mode
      //twi_state = TWI_SRX;
    
	TWCR |= 1 << TWINT;  
	break;
    case TW_SR_DATA_ACK:       // data received, returned ack
    case TW_SR_GCALL_DATA_ACK: // data received generally, returned ack
    twi_data = TWDR;
    TWCR |= 1 << TWINT;
    break;
    case TW_SR_STOP: 
	// stop or repeated start condition received
      // ack future responses and leave slave receiver state
    TWCR |= 1 << TWINT;  
	break;
    case TW_SR_DATA_NACK:       // data received, returned nack
    case TW_SR_GCALL_DATA_NACK: // data received generally, returned nack
      // nack back at master
    break;

      
    // Slave Transmitter
    case TW_ST_SLA_ACK:          // addressed, returned ack
    case TW_ST_ARB_LOST_SLA_ACK: // arbitration lost, returned ack
    // enter slave transmitter mode
	TWDR = 0b00100100;
	TWCR |= 1 << TWINT;	
	// transmit first byte from buffer, fall
    case TW_ST_DATA_ACK: // byte sent, ack returned
      // copy data to output register
	TWCR |= 1 << TWINT;
      break;
    case TW_ST_DATA_NACK: // received nack, we are done 
    case TW_ST_LAST_DATA: // received ack, but we are done already!
	// ack future responses
	TWCR |= 1 << TWINT; 
	// leave slave receiver state
      break;

    // All
    case TW_NO_INFO:   // no state information
      break;
    case TW_BUS_ERROR: // bus error, illegal stop/start
      break;
  }
}
