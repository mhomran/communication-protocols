#ifndef _ISR_H
#define _ISR_H

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"
#include "inc/tm4c1294ncpdt.h"
#include "driverlib/can.h"
#include "inc/hw_memmap.h"

volatile uint32_t transmit_1 = 0;
volatile uint32_t transmit_2 = 0;
volatile uint32_t receive_1 = 0;
volatile uint32_t receive_2 = 0;

#endif
