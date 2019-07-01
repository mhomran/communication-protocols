#include "ADXL345.h"

void INIT_ADXL(){
    INIT_SPI();

    (*((volatile uint32_t *)0x4005B010)) &= ~(1 << 2);
    SPI_MasterTransmit(0b00111111 & DATA_FORMAT);
    SPI_MasterTransmit(0x01);
    (*((volatile uint32_t *)0x4005B010)) |= 1 << 2;

    (*((volatile uint32_t *)0x4005B010)) &= ~(1 << 2);
    SPI_MasterTransmit(0b00111111 & POWER_CTL);
    SPI_MasterTransmit(0x08);
    (*((volatile uint32_t *)0x4005B010)) |= 1 << 2;
}

char X_AXIS(void){
    (*((volatile uint32_t *)0x4005B010)) &= ~(1 << 2);
    SPI_MasterTransmit(DATAX0 | 0x80);
    SPI_MasterTransmit(0xFF);           //dummy
    (*((volatile uint32_t *)0x4005B010)) |= 1 << 2;

    while(!(SSI2_SR_R & (1 << 1)));
    SSI2_ICR_R |= 1 << 1;

    return SSI2_DR_R;
}

char Y_AXIS(void){

    (*((volatile uint32_t *)0x4005B010)) &= ~(1 << 2);
    SPI_MasterTransmit(DATAY0 | 0x80);
    SPI_MasterTransmit(0xFF);           //dummy
    (*((volatile uint32_t *)0x4005B010)) |= 1 << 2;

    while(!(SSI2_SR_R & (1 << 1)));
     SSI2_ICR_R |= 1 << 1;

    return SSI2_DR_R;
}

char Z_AXIS(void){

    (*((volatile uint32_t *)0x4005B010)) &= ~(1 << 2);
    SPI_MasterTransmit(DATAZ0 | 0x80);
    SPI_MasterTransmit(0xFF);           //dummy
    (*((volatile uint32_t *)0x4005B010)) |= 1 << 2;

    while(!(SSI2_RIS_R & (1 << 1)));
    SSI2_ICR_R |= 1 << 1;

    return SSI2_DR_R;
}
