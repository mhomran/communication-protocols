#include "ISR_ud.h"

/* interrupt service routines */

void
CANIntHandler(void)
{
  switch (CAN1_INT_R & 0xFFFF)
    {
      case 0x1	:
	{
	  CANIntClear(CAN1_BASE, 1);
	  GPIO_PORTN_DATA_R ^= 1 << 0;	//D2
	  receive_1 = 1;
	}
	break;
      case 0x2	:
	{
	  CANIntClear(CAN1_BASE, 2);
	  GPIO_PORTN_DATA_R ^= 1 << 1;	//D1
	  receive_2 = 1;
	}
    }
}

void
port_m_handler (void)
{
  GPIO_PORTM_ICR_R |= 1 << 4 | 1 << 5; //clear interrupt flag
  if ((GPIO_PORTM_DATA_R & (1 << 4)) == 0)
    {
      transmit_1 = 1;
    }
  else if ((GPIO_PORTM_DATA_R & (1 << 5)) == 0)
    {
      transmit_2 = 1;
    }
  else
    {
      /* NOT REACHABLE */
    }
}
