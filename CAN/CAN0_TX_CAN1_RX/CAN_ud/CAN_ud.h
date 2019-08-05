#ifndef _CAN_H
#define _CAN_H

#define F_CPU (unsigned long)(25000000) 	//the system CLK

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_can.h"
#include "inc/hw_memmap.h"
#include "driverlib/can.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "inc/tm4c1294ncpdt.h"

extern volatile uint32_t transmit_1;
extern volatile uint32_t transmit_2;
extern volatile uint32_t receive_1;
extern volatile uint32_t receive_2;

void init_can(void);

#endif
