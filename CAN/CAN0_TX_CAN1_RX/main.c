#include "main.h"


void main(void)
{
	//CAN0 variables
	tCANMsgObject sCANMessage_CAN0;
	uint32_t ui32MsgData_CAN0;
	uint8_t *pui8MsgData_CAN0;
	//CAN1 variables
	tCANMsgObject sCANMessage_CAN1;
    uint8_t pui8MsgData_CAN1[1];

	//initialize Clock
	uint32_t ui32SysClock;
    ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                       SYSCTL_OSC_MAIN |
                                       SYSCTL_USE_OSC)
                                       ,25000000);


    pui8MsgData_CAN0 = (uint8_t *)&ui32MsgData_CAN0;

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

    CAN0_CTL_R |= 1 << 5;

    //bit rate-> 100 Kb/s
    CANBitRateSet(CAN0_BASE, ui32SysClock, 100000);
    CANBitRateSet(CAN1_BASE, ui32SysClock, 100000);

    //
    // Enable the CAN for operation.
    //

    CANEnable(CAN0_BASE);
    CANEnable(CAN1_BASE);

    //
    //message transmitted
    //

    ui32MsgData_CAN0 = 0x55;
    sCANMessage_CAN0.ui32MsgID = 1;
    sCANMessage_CAN0.ui32MsgIDMask = 0;
    sCANMessage_CAN0.ui32Flags = 0;
    sCANMessage_CAN0.ui32MsgLen = 1;
    sCANMessage_CAN0.pui8MsgData = pui8MsgData_CAN0;

	//received message
    sCANMessage_CAN1.ui32MsgID = 1;
    sCANMessage_CAN1.ui32MsgIDMask = 0x1FFF; 				//use all 12 IDs for acceptance filtering
    sCANMessage_CAN1.ui32Flags = MSG_OBJ_USE_ID_FILTER;
    sCANMessage_CAN1.ui32MsgLen = 1;

   CANMessageSet(CAN1_BASE, 1, &sCANMessage_CAN1, MSG_OBJ_TYPE_RX);

   uint32_t i = 0;

   //LOOP FOR EVER
   while(1){
	   //transmit a message
	   CANMessageSet(CAN0_BASE, 1, &sCANMessage_CAN0, MSG_OBJ_TYPE_TX);

       sCANMessage_CAN1.pui8MsgData = pui8MsgData_CAN1;

	   CANMessageGet(CAN1_BASE, 1, &sCANMessage_CAN1, 0);

	   for (i = 0; i < 50000; i++){
	   }
   }
}

