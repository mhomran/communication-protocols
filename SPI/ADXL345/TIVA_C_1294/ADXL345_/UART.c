#include "UART.h"

void INIT_UART(void){
    //enable UART0
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R0;
    while(!(SYSCTL_PRUART_R & SYSCTL_RCGCUART_R0));
    //enable PORTD
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0;
    while(!(SYSCTL_PRGPIO_R & SYSCTL_RCGCGPIO_R0));

    //PA1-> U0Tx
    GPIO_PORTA_AHB_DIR_R    |= 1 << 1;                       //OUTPUT PA1
    GPIO_PORTA_AHB_PC_R     |= 1 << 1;                      //PIN5 2mA
    GPIO_PORTA_AHB_PUR_R    &= ~(1 << 1);                    //disable pull-up
    GPIO_PORTA_AHB_DEN_R    |= 1 << 1;                      //digital

    GPIO_PORTA_AHB_AFSEL_R  |= 1 << 1;                //Alternate function enable
    GPIO_PORTA_AHB_PCTL_R   |= 0x1 << 4;           //U0TX->PA1

    //PA0-> U0Rx
    GPIO_PORTA_AHB_DIR_R    &= ~(1 << 0);             //INPUT PA0
    GPIO_PORTA_AHB_PC_R     |= 1 << 0;                //PIN0 2mA
    GPIO_PORTA_AHB_PUR_R    |= (1 << 0);              //enable pull-up
    GPIO_PORTA_AHB_DEN_R    |= 1 << 0;                //digital

    GPIO_PORTA_AHB_AFSEL_R  |= 1 << 0;             //Alternate function enable
    GPIO_PORTA_AHB_PCTL_R   |= 0x1;               //U0RX->PA0

    //disable UART2
    UART0_CTL_R     &= ~(1);
    //setting the BAUD->25MHz/(16*9600)
    UART0_IBRD_R = 162;
    UART0_FBRD_R = 49;

    //configure the frame 8-bit + 1 stop bit + FIFOs disabled
    UART0_LCRH_R = 0x60;

    //enable receive interrupt

    UART0_IM_R      |= 1 << 4;          //mask
    NVIC_EN0_R      |= 1 << 5;          //enable interrupt number 5

    //enable UART2
    UART0_CTL_R     |= 1 ;
}

void UART_TX(char ch){
    while(!(UART0_FR_R & (1 << 7)));
    UART0_DR_R = ch;
}

void UART_PRINTF(const char *str)
{
    int x =0;
    while (str[x]){
    UART_TX(str[x]);
    x++;
    }
}

void UART_PRINTN(uint8_t ch){
char str[4];

    str[3] = '\0';
    int i;
    for(i = 0; i < 3; i++){
        switch (i){
            case 0 :
            {
                str[2-i] = (ch % 10) + 48;
            }
            break;
            case 1 :
            {
                str[2-i] = ((ch / 10) % 10 )+ 48;
            }
            break;
            case 2 :
            {
                str[2-i] = ((ch/100) % 10) + 48;
            }
            break;
        }
    }

    UART_PRINTF(str);
}
