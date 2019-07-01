#ifndef _SPI_H
#define _SPI_H

#include "tm4c1294ncpdt.h"
#include <inttypes.h>

void INIT_SPI(void);
void SPI_MasterTransmit(uint8_t byte);

#endif
