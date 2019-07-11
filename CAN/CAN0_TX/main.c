#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/can.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "inc/tm4c1294ncpdt.h"


void main(void)
{
    uint32_t ui32SysClock;
    tCANMsgObject sCANMessage;
    uint32_t ui32MsgData;
    uint8_t *pui8MsgData;

    pui8MsgData = (uint8_t *)&ui32MsgData;

    ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                       SYSCTL_OSC_MAIN |
                                       SYSCTL_USE_OSC)
                                       ,25000000);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_CAN0RX);
    GPIOPinConfigure(GPIO_PA1_CAN0TX);

    GPIOPinTypeCAN(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);

    //
    // Initialize the CAN controller
    //
    CANInit(CAN0_BASE);

    //bit rate-> 100 Kb/s
    CANBitRateSet(CAN0_BASE, ui32SysClock, 100000);

    //
    // Enable the CAN for operation.
    //

    CANEnable(CAN0_BASE);

    //
    // Initialize the message object that will be used for sending CAN
    // messages.  The message will be a byte.

    ui32MsgData = 0x55;
    sCANMessage.ui32MsgID = 1;
    sCANMessage.ui32MsgIDMask = 0;
    sCANMessage.ui32Flags = 0;
    sCANMessage.ui32MsgLen = 1;
    sCANMessage.pui8MsgData = pui8MsgData;

    CANMessageSet(CAN0_BASE, 1, &sCANMessage, MSG_OBJ_TYPE_TX);

}

