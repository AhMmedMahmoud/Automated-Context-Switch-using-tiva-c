#include "bsp.h"
#include "ports.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>


/********* systick timer ***********/
void SysTict_init()
{
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = 16000000-1;
	NVIC_ST_CURRENT_R = 0; 
	NVIC_ST_CTRL_R = 0x07;     //activate enable,interrupt,freq bus
}

/*************** BSP *************/
void BSP_init()
{
	portF_init();         // initialize port f
	led_init(GREEN_LED);  // initialize green led
	led_off(GREEN_LED);    // turn on green led
	led_init(BLUE_LED);   // initialize blue led
	led_off(BLUE_LED);     // turn on blue led
	
	SysTict_init();       // initialize systick timer
	NVIC_SYS_PRI3_R &= ~((1<<29 | 1<<30 | 1<<31));  // set Systick interrupt priority (highest 0)
	
		__enable_irq();			// enable global interrupt
}