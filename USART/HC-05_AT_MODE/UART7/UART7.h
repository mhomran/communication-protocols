#ifndef _UART7_H
#define _UART7_H
//PC4 Rx, PC5 Tx
#include <inttypes.h>
#include "tm4c1294ncpdt.h"

void INIT_UART7(void);
void UART7_TX(char ch);
void UART0_TX(char ch);

#endif
