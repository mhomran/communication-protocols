#ifndef _GPIO_H
#define _GPIO_H

#include <stdint.h>
#include "inc/tm4c1294ncpdt.h"

void init_button_pm4(void);
void init_button_pm5(void);
void init_led_pn0(void);
void init_led_pn1(void);
void init_led_pf0(void);

#endif
