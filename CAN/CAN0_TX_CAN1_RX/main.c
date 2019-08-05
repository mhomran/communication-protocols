#include "main.h"


void main(void)
{
  //CAN0 variables
  tCANMsgObject sCANMessage_CAN0_1, sCANMessage_CAN0_2;
  uint32_t ui32MsgData_CAN0_1, ui32MsgData_CAN0_2;
  uint8_t *pui8MsgData_CAN0_1, * pui8MsgData_CAN0_2;
  //CAN1 variables
  tCANMsgObject sCANMessage_CAN1_1, sCANMessage_CAN1_2;
  uint8_t pui8MsgData_CAN1_1[1], pui8MsgData_CAN1_2[1];

  //initialize Clock
  uint32_t ui32SysClock;
  ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
			       SYSCTL_OSC_MAIN |
			       SYSCTL_USE_OSC)
			       ,25000000);


  pui8MsgData_CAN0_1 = (uint8_t *)&ui32MsgData_CAN0_1;
  pui8MsgData_CAN0_2 = (uint8_t *)&ui32MsgData_CAN0_2;

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
  CANBitRateSet(CAN0_BASE, ui32SysClock, 100000);
  CANBitRateSet(CAN1_BASE, ui32SysClock, 100000);

  //
  // Enable the CAN for operation.
  //

  CANEnable(CAN0_BASE);
  CANEnable(CAN1_BASE);

  //
  //messages transmitted
  //
  //message 1
  ui32MsgData_CAN0_1 = 0x55;
  sCANMessage_CAN0_1.ui32MsgID = 1;
  sCANMessage_CAN0_1.ui32MsgIDMask = 0;
  sCANMessage_CAN0_1.ui32Flags = 0;
  sCANMessage_CAN0_1.ui32MsgLen = 1;
  sCANMessage_CAN0_1.pui8MsgData = pui8MsgData_CAN0_1;
  //message 2
  ui32MsgData_CAN0_2 = 0x55;
  sCANMessage_CAN0_2.ui32MsgID = 2;
  sCANMessage_CAN0_2.ui32MsgIDMask = 0;
  sCANMessage_CAN0_2.ui32Flags = 0;
  sCANMessage_CAN0_2.ui32MsgLen = 1;
  sCANMessage_CAN0_2.pui8MsgData = pui8MsgData_CAN0_2;

  //received messages
  //message 1
  sCANMessage_CAN1_1.ui32MsgID = 1;
  sCANMessage_CAN1_1.ui32MsgIDMask = 0x1FFF; 				//use all 12 IDs for acceptance filtering
  sCANMessage_CAN1_1.ui32Flags = MSG_OBJ_USE_ID_FILTER | MSG_OBJ_RX_INT_ENABLE;
  sCANMessage_CAN1_1.ui32MsgLen = 1;

  //message 2
  sCANMessage_CAN1_2.ui32MsgID = 2;
  sCANMessage_CAN1_2.ui32MsgIDMask = 0x1FFF; 				//use all 12 IDs for acceptance filtering
  sCANMessage_CAN1_2.ui32Flags = MSG_OBJ_USE_ID_FILTER | MSG_OBJ_RX_INT_ENABLE;
  sCANMessage_CAN1_2.ui32MsgLen = 1;

  //set the message to be received
  CANMessageSet(CAN1_BASE, 1, &sCANMessage_CAN1_1, MSG_OBJ_TYPE_RX);
  CANMessageSet(CAN1_BASE, 2, &sCANMessage_CAN1_2, MSG_OBJ_TYPE_RX);

  //counter
  uint32_t i = 0;

  //initialize push button
  init_button_pm4();
  init_button_pm5();
  init_led_pn0();
  init_led_pn1();
  init_led_pf0();

  //LOOP FOR EVER
  while(1)
    {
      if (transmit_1)
	{
	  //transmit a message
	  CANMessageSet(CAN0_BASE, 1, &sCANMessage_CAN0_1, MSG_OBJ_TYPE_TX);
	  transmit_1 = 0;
	}
      if (transmit_2)
	{
	  //transmit a message
	  CANMessageSet(CAN0_BASE, 2, &sCANMessage_CAN0_2, MSG_OBJ_TYPE_TX);
	  transmit_2 = 0;
	}

      if (receive_1)
	{
	  sCANMessage_CAN1_1.pui8MsgData = pui8MsgData_CAN1_1;
	  CANMessageGet(CAN1_BASE, 1, &sCANMessage_CAN1_1, 0);
	  receive_1 = 0;
	}
      if (receive_2)
	{
	  sCANMessage_CAN1_2.pui8MsgData = pui8MsgData_CAN1_2;
	  CANMessageGet(CAN1_BASE, 2, &sCANMessage_CAN1_2, 0);
	  receive_2 = 0;
	}

      for (i = 0; i < 50000; i++)
	{
	  /* DO NOTHING */
	}
    }
}

