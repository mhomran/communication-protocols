#ifndef _MAIN_H
#define _MAIN_H

#include <stdint.h>
#include "CAN_ud/CAN_ud.h"
#include "GPIO_ud/GPIO_ud.h"

void init_can_messages(void);


/* CAN messages */
//
//messages transmitted
//
tCANMsgObject sCANMessage_CAN0_1, sCANMessage_CAN0_2;
uint32_t ui32MsgData_CAN0_1, ui32MsgData_CAN0_2;
uint8_t *pui8MsgData_CAN0_1, * pui8MsgData_CAN0_2;

//received messages
tCANMsgObject sCANMessage_CAN1_1, sCANMessage_CAN1_2;
uint8_t pui8MsgData_CAN1_1[1], pui8MsgData_CAN1_2[1];


#endif
