#ifndef I2C_H_
#define I2C_H_

#include <inttypes.h>
#include <compat/twi.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void INIT_I2C(uint8_t ADDR_fn);
void I2C_SEND(uint8_t data);

volatile uint8_t twi_data;
volatile uint8_t stop;
volatile uint8_t ADDR;

#endif /* I2C_H_ */