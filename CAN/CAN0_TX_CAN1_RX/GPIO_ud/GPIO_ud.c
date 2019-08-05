#include "GPIO_ud/GPIO_ud.h"

/* Initialize GPIOs*/

void
init_button_pm4(void)
{
  //PM4 INPUT interrupt enabled
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R11;         //clock for PORT M
  GPIO_PORTM_DIR_R    &= ~(1 << 4);             //INPUT PM4
  GPIO_PORTM_PC_R     |= 1 << 4;                //PIN4 2mA
  GPIO_PORTM_PUR_R    &= ~(1 << 4);              //disable pull-up
  GPIO_PORTM_DEN_R    |= 1 << 4;                //digital

  GPIO_PORTM_IM_R   |= 1 << 4;                //enable interrupt-> default falling edge
  NVIC_EN2_R        |= 1 << (72-64);          //enable interrupt number 51
}

void
init_button_pm5(void)
{
  //PM4 INPUT interrupt enabled
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R11;         //clock for PORT M
  GPIO_PORTM_DIR_R    &= ~(1 << 5);             //INPUT PM4
  GPIO_PORTM_PC_R     |= 1 << 5;                //PIN4 2mA
  GPIO_PORTM_PUR_R    &= ~(1 << 5);              //disable pull-up
  GPIO_PORTM_DEN_R    |= 1 << 5;                //digital

  GPIO_PORTM_IM_R   |= 1 << 5;                //enable interrupt-> default falling edge
  NVIC_EN2_R        |= 1 << (72-64);          //enable interrupt number 51
}

void
init_led_pn0(void)
{
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R12;   //clock for PORTN
  while(!(SYSCTL_PRGPIO_R & SYSCTL_RCGCGPIO_R12));

  GPIO_PORTN_DIR_R  |= 1 << 0;                //OUTPUT PIN0
  GPIO_PORTN_PC_R   |= 1 << 0;                //PIN0 2mA
  GPIO_PORTN_PUR_R  &= ~(1 << 0);             //disable pull-up
  GPIO_PORTN_DEN_R  |= 1 << 0;                //digital
}

void
init_led_pn1(void)
{
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R12;   //clock for PORTN
  while(!(SYSCTL_PRGPIO_R & SYSCTL_RCGCGPIO_R12));

  GPIO_PORTN_DIR_R  |= 1 << 1;                //OUTPUT PIN0
  GPIO_PORTN_PC_R   |= 1 << 1;                //PIN0 2mA
  GPIO_PORTN_PUR_R  &= ~(1 << 1);             //disable pull-up
  GPIO_PORTN_DEN_R  |= 1 << 1;                //digital
}

void
init_led_pf0(void){
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;   //clock for PORTN
  while(!(SYSCTL_PRGPIO_R & SYSCTL_RCGCGPIO_R5));

  GPIO_PORTF_AHB_DIR_R  |= 1 << 0;                //OUTPUT PIN0
  GPIO_PORTF_AHB_PC_R   |= 1 << 0;                //PIN0 2mA
  GPIO_PORTF_AHB_PUR_R  &= ~(1 << 0);             //disable pull-up
  GPIO_PORTF_AHB_DEN_R  |= 1 << 0;                //digital
}
