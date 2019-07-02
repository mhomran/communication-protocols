#ifndef _ADXL345_H
#define _ADXL345_H

char X_AXIS(void);
char Y_AXIS(void);
char Z_AXIS(void);

#include "tm4c1294ncpdt.h"
#include "inttypes.h"
#include "I2C.h"

void writeTo(uint8_t address, uint8_t val);
char readFrom(uint8_t address);

void INIT_ADXL(void);

#define POWER_CTL 0x2D //Power Control Register
#define DATA_FORMAT 0x31
#define DATAX0 0x32 //X-Axis Data 0
#define DATAX1 0x33 //X-Axis Data 1
#define DATAY0 0x34
#define DATAY1 0x35
#define DATAZ0 0x36
#define DATAZ1 0x37

#define DEVICE_ADDR_W 0xA6
#define DEVICE_ADDR_R 0xA7


void writeTo(uint8_t address, uint8_t val) ;
char readFrom(uint8_t address);

#endif
