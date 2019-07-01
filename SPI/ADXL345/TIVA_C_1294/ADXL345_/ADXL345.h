#ifndef _ADXL345_H
#define _ADXL345_H

#include "SPI.h"
#include "tm4c1294ncpdt.h"

#define POWER_CTL 0x2D //Power Control Register
#define DATA_FORMAT 0x31
#define DATAX0 0x32 //X-Axis Data 0
#define DATAX1 0x33 //X-Axis Data 1
#define DATAY0 0x34
#define DATAY1 0x35
#define DATAZ0 0x36
#define DATAZ1 0x37

char X_AXIS(void);
char Y_AXIS(void);
char Z_AXIS(void);

void INIT_ADXL(void);

#endif
