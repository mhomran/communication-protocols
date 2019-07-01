#include "SPI.h"

void INIT_SPI(void){
    //enable SSI2 CLK
    SYSCTL_RCGCSSI_R |= SYSCTL_RCGCSSI_R2;
    while(!(SYSCTL_PRSSI_R & SYSCTL_RCGCSSI_R2));

    //enable GPIO Port D
    SYSCTL_RCGCGPIO_R  |= SYSCTL_RCGCGPIO_R3;
    while(!(SYSCTL_PRGPIO_R & SYSCTL_RCGCGPIO_R3));

    //configure the SPI pins
    //MISO-> INPUT
    GPIO_PORTD_AHB_DIR_R  &= ~(1 << 0);               //INPUT PD0
    GPIO_PORTD_AHB_PC_R     |= 1 << 0;                //PIN0 2mA
    GPIO_PORTD_AHB_PUR_R  &= ~(1 << 0);               //disable pull-up resistor
    GPIO_PORTD_AHB_DEN_R    |= 1 << 0;                //digital
    GPIO_PORTD_AHB_AFSEL_R  |= 1 << 0;                //Alternate function enable
    GPIO_PORTD_AHB_PCTL_R   |= 15 << (4 * 0);       //SSI2Rx

    //MOSI -> OUTPUT
    GPIO_PORTD_AHB_DIR_R    |= 1 << 1;                       //OUTPUT PD1
    GPIO_PORTD_AHB_PC_R     |= 1 << 1;                       //PIN1 2mA
    GPIO_PORTD_AHB_PUR_R  &= ~(1 << 1);                      //disable pull-up
    GPIO_PORTD_AHB_DEN_R    |= 1 << 1;                       //digital
    GPIO_PORTD_AHB_AFSEL_R  |= 1 << 1;                       //Alternate function enable
    GPIO_PORTD_AHB_PCTL_R   |= 15 << (4 * 1);              //SSI2Tx

    //SS
    GPIO_PORTD_AHB_DIR_R    |= 1 << 2;                       //OUTPUT PD2
    GPIO_PORTD_AHB_PC_R     |= 1 << 2;                       //PIN2 2mA
    GPIO_PORTD_AHB_PUR_R  &= ~(1 << 2);                      //disable pull-up
    GPIO_PORTD_AHB_DEN_R    |= 1 << 2;                       //digital
    (*((volatile uint32_t *)0x4005B010)) &= ~(1 << 2);

    //CLK
    GPIO_PORTD_AHB_DIR_R    |= 1 << 3;                       //OUTPUT PD3
    GPIO_PORTD_AHB_PC_R     |= 1 << 3;                       //PIN3 2mA
    GPIO_PORTD_AHB_PUR_R   |= (1 << 3);                      //enable pull-up must for IDLE HIGH CLK
    GPIO_PORTD_AHB_DEN_R    |= 1 << 3;                       //digital
    GPIO_PORTD_AHB_AFSEL_R  |= 1 << 3;                       //Alternate function enable
    GPIO_PORTD_AHB_PCTL_R   |= 15 << (4 * 3);              //SSI2Clk

    //legacy mode
    //configure Frame
    SSI2_CR1_R             &= ~(1 << 1);                     //ensure module is disabled
    SSI2_CC_R              =   0x5;                          //CLK source is PIOSC
    SSI2_CPSR_R            = 16;                             //16MHz/16=1MHz SSI2Clk
    SSI2_CR0_R             |= 1 << 7 | 1 << 6 | 7 << 0;      //IDLE HIGH +sampling on rising edge+ 8-bit frame
    SSI2_CR1_R             |= (1 << 1);                      //enable module
}

void SPI_MasterTransmit(uint8_t byte){
    SSI2_DR_R = byte;
    while(!(SSI2_RIS_R & (1 << 6)));
    SSI2_ICR_R |= 1 << 6;
}
