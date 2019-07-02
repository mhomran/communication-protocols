#include "ADXL345.h"

void writeTo(uint8_t address, uint8_t val){
    I2C0_MSA_R  = DEVICE_ADDR_W;                           //device address + W
    I2C0_MDR_R = address;
    I2C0_MCS_R = 0b000011;

    while(!(I2C0_MRIS_R & (1 << 5)));    //start polling
    I2C0_MICR_R |= 1 << 5;
    while(!(I2C0_MRIS_R & (1 << 0)));    //transaction polling
    I2C0_MICR_R |= 1 << 0;

    I2C0_MDR_R = val;
    I2C0_MCS_R = 0b000101;

    while(!(I2C0_MRIS_R & (1 << 0)));    //transaction polling
    I2C0_MICR_R |= 1 << 0;
    while(!(I2C0_MRIS_R & (1 << 6)));    //stop polling
    I2C0_MICR_R |= 1 << 6;
}

char readFrom(uint8_t address){
    I2C0_MSA_R  = DEVICE_ADDR_W;                           //device address + W
    I2C0_MDR_R = address;
    I2C0_MCS_R = 0b000111;

    while(!(I2C0_MRIS_R & (1 << 5)));    //start polling
    I2C0_MICR_R |= 1 << 5;
    while(!(I2C0_MRIS_R & (1 << 0)));    //transaction polling
    I2C0_MICR_R |= 1 << 0;
    while(!(I2C0_MRIS_R & (1 << 6)));    //stop polling
    I2C0_MICR_R |= 1 << 6;

    I2C0_MSA_R  = DEVICE_ADDR_R;                           //device address + R
    I2C0_MCS_R= 0b000111;

    while(!(I2C0_MRIS_R & (1 << 5)));    //start polling
    I2C0_MICR_R |= 1 << 5;
    while(!(I2C0_MRIS_R & (1 << 0)));    //transaction polling
    I2C0_MICR_R |= 1 << 0;
    while(!(I2C0_MRIS_R & (1 << 6)));    //stop polling
    I2C0_MICR_R |= 1 << 6;

    return I2C0_MDR_R;
}

void INIT_ADXL(void){
    //Initiate I2C0
    INIT_I2C();

    writeTo(DATA_FORMAT, 0x01);
    //Put the ADXL345 into Measurement Mode by writing 0x08 to the POWER_CTL register.
    writeTo(POWER_CTL, 0x08);
}

char X_AXIS(void){
    return readFrom(DATAX0);
}

char Y_AXIS(void){
    return readFrom(DATAY0);
}

char Z_AXIS(void){
    return readFrom(DATAZ0);
}
