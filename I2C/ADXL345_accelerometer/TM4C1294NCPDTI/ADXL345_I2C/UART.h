#ifndef _UART_H
#define _UART_H

#include "tm4c1294ncpdt.h"
#include <inttypes.h>

void INIT_UART(void);
void UART_TX(char ch);
void UART_PRINTN(uint8_t ch);
void UART_PRINTF(const char *str);

#endif
