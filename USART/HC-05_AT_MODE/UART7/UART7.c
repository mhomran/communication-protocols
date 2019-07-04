#include "UART7.h"

void INIT_UART7(void){
    //enable UART7
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R7;
    while(!(SYSCTL_PRUART_R & SYSCTL_RCGCUART_R7));
    //enable PORTC
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R2;
    while(!(SYSCTL_PRGPIO_R & SYSCTL_RCGCGPIO_R2));

    //PC5-> U7Tx
    GPIO_PORTC_AHB_DIR_R    |= 1 << 5;                       //OUTPUT
    GPIO_PORTC_AHB_PC_R     |= 1 << 5;                      //PIN5 2mA
    GPIO_PORTC_AHB_PUR_R    &= ~(1 << 5);                    //disable pull-up
    GPIO_PORTC_AHB_DEN_R    |= 1 << 5;                      //digital

    GPIO_PORTC_AHB_AFSEL_R  |= 1 << 5;                //Alternate function enable
    GPIO_PORTC_AHB_PCTL_R   |= 0x1 << (4 * 5);           //U0TX->PA1

    //PC4-> U7Rx
    GPIO_PORTC_AHB_DIR_R    &= ~(1 << 4);             //INPUT PA0
    GPIO_PORTC_AHB_PC_R     |= 1 << 4;                //PIN0 2mA
    GPIO_PORTC_AHB_PUR_R    |= (1 << 4);              //enable pull-up
    GPIO_PORTC_AHB_DEN_R    |= 1 << 4;                //digital

    GPIO_PORTC_AHB_AFSEL_R  |= 1 << 4;             //Alternate function enable
    GPIO_PORTC_AHB_PCTL_R   |= 0x1 << (4 * 4);               //U0RX->PA0

    //disable UART2
    UART7_CTL_R     &= ~(1);
    //setting the BAUD->25MHz/(16*38400)
    UART7_IBRD_R = 40;
    UART7_FBRD_R = 44;

    //configure the frame 8-bit + 1 stop bit + FIFOs disabled
    UART7_LCRH_R = 0x60;

    //enable receive interrupt

    UART7_IM_R      |= 1 << 4;                  //mask  receiver
    NVIC_EN1_R      |= 1 << (60 - 32);          //enable interrupt number 60

    //enable UART2
    UART7_CTL_R     |= 1 ;
}

void UART7_TX(char ch){
    while(!(UART7_FR_R & (1 << 7)));
    UART7_DR_R = ch;
}

void UART0_TX(char ch){
    while(!(UART0_FR_R & (1 << 7)));
    UART0_DR_R = ch;
}

void UART7IntHandler(void){
    UART7_ICR_R |= 1 << 4;       //clear
    UART0_DR_R = UART7_DR_R;        //read FIFO receiver of UART7 and send it to UART0
}
