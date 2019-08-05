#include "CAN_ud/CAN_ud.h"

/* CAN driver */

void init_can(void)
  {
    //enable peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN1);

    //configure GPIOs
    //CAN0
    GPIOPinConfigure(GPIO_PA0_CAN0RX);
    GPIOPinConfigure(GPIO_PA1_CAN0TX);
    GPIOPinTypeCAN(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    //CAN1
    GPIOPinConfigure(GPIO_PB0_CAN1RX);
    GPIOPinConfigure(GPIO_PB1_CAN1TX);
    GPIOPinTypeCAN(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Initialize the CAN controller
    //
    CANInit(CAN0_BASE);
    CANInit(CAN1_BASE);

    //disable auto transmission
    CAN0_CTL_R |= 1 << 5;

    //enable interrupts
    CANIntEnable(CAN1_BASE, CAN_INT_MASTER);

    //
    // Enable the CAN interrupt on the processor (NVIC).
    //
    IntEnable(INT_CAN1);

    //bit rate-> 100 Kb/s
    CANBitRateSet(CAN0_BASE, F_CPU, 100000);
    CANBitRateSet(CAN1_BASE, F_CPU, 100000);

    //
    // Enable the CAN for operation.
    //

    CANEnable(CAN0_BASE);
    CANEnable(CAN1_BASE);
  }
