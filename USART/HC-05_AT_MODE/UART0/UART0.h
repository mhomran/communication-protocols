#ifndef _UART0_H
#define _UART0_H

#include <inttypes.h>
#include "tm4c1294ncpdt.h"

void INIT_UART0(void);
void UART0_TX(char ch);

volatile uint32_t UART0_data;
#endif
