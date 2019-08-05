#include "main.h"

/* send and receive two can messages. Control the transmission of the messages with push buttons */

void main(void)
{
  //initialize Clock
  SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
			       SYSCTL_OSC_MAIN |
			       SYSCTL_USE_OSC)
			       ,25000000);

  //initialize CAN protocol
  init_can();
  //initialize messages
  init_can_messages();

  //delay counter
  uint32_t i;

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
	  CANMessageGet(CAN1_BASE, 1, &sCANMessage_CAN1_1, 0);
	  receive_1 = 0;
	}
      if (receive_2)
	{
	  CANMessageGet(CAN1_BASE, 2, &sCANMessage_CAN1_2, 0);
	  receive_2 = 0;
	}

      for (i = 0; i < 50000; i++)
	{
	  /* DO NOTHING */
	}
    }
}

void
init_can_messages(void)
{
  //messages transmitted

  pui8MsgData_CAN0_1 = (uint8_t *)&ui32MsgData_CAN0_1;
  pui8MsgData_CAN0_2 = (uint8_t *)&ui32MsgData_CAN0_2;

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

  sCANMessage_CAN1_1.pui8MsgData = pui8MsgData_CAN1_1;
  sCANMessage_CAN1_2.pui8MsgData = pui8MsgData_CAN1_2;

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
}
