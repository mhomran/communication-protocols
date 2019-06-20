/*
 * LCD_16x2.c
 *
 * Created: 5/25/2019 7:26:20 PM
 * Author : Mohamed_Hassanin
 */ 
//The target of this driver is to make use of the Timer and interrupt peripherals and drive the LCD without any #polling.
//if the program print 32 char every or more than ~520 clock cycle, the printing will hardly use the CPU, that makes the program focus
//on what it does without any polling coming from LCD. In the Other Hand, If the program printing interval is less than
//that number of cycles, then this program will be useless and the traditional way of the CPU delay will be ok to drive the LCD.

#include <avr/io.h>
#define F_CPU 16000000UL
#include "LCD.h"

int main(void)
{
	init_LCD();
	LCD_PRINT("lol");
	LCD_PRINT("pop");
	LCD_SET_CURSOR(0,0);
	while (1) 
    {
    }
}
