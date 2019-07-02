#include "I2C.h"

void INIT_I2C(void){
    //enable I2C CLK
    SYSCTL_RCGCI2C_R |= SYSCTL_RCGCI2C_R0;
    while(!(SYSCTL_PRI2C_R & SYSCTL_RCGCI2C_R0));
    //enable PORT B CLK
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    while(!(SYSCTL_PRGPIO_R & SYSCTL_RCGCGPIO_R1));

    //configure GPIOs
    //I2C0SCL PB2 , I2C0SDA PB3
    GPIO_PORTB_AHB_PC_R   |= 1 << 2 | 1 << 3;                //PIN0 2mA
    GPIO_PORTB_AHB_DEN_R  |= 1 << 2 | 1 << 3;                //digital
    GPIO_PORTB_AHB_ODR_R  |= 1 << 3;                //open drain
    GPIO_PORTB_AHB_AFSEL_R |= 1 << 2 | 1 << 3;               //alternate function
    GPIO_PORTB_AHB_PCTL_R |= 2 << (4 * 2) | 2 << (4 * 3);    //SCL , SDA

    I2C0_MCR_R = 0x10;                                      //initialize the I2C
    I2C0_MTPR_R = 0x0C;                                     //Standard mode
}
