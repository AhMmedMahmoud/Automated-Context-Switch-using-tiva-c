#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "bsp.h"
#include "MIROS.h"


/********** global variables ********/
extern uint32_t tickCount;


/*********** tasks ****************/
// task 1
void main_blinky1()
{
	int i = 0;
	for(;;)
	{
		led_on(GREEN_LED);
		for(i = 0; i < 16000; i++);		// delay
		led_off(GREEN_LED);
	}
}

// task 2
void main_blinky2()
{
	int i = 0;
	for(;;)
	{
		led_on(BLUE_LED);
		for(i = 0; i < 16000; i++);			// delay
		led_off(BLUE_LED);
	}
}


/*********** stacks ****************/
// stack array for task 1
uint32_t stack_blinky1[40];

// stack array for task 2
uint32_t stack_blinky2[40];

// container contains stack pointer of task 1
OSThread blinky1;

// container contains stack pointer of task 1
OSThread blinky2;


/****** systick timer handler ******/
void SysTick_Handler()
{
	tickCount++;
	
	__disable_irq();
	OS_sched();
	__enable_irq();
}


/********* PendSV handler **********/
__asm
void PendSV_Handler(void)
{
	IMPORT OS_curr
	IMPORT OS_next
	
	
	CPSID I								 // disable global interrupts 
	
	LDR r1,=OS_curr        // if(OS_curr != (OSThread *)0) 
	LDR r1,[r1,#0x00]
	CBZ r1,PendSV_restore
	
	PUSH {r4-r11}  				 		// push registers r4-r11 on the stack/
	
	LDR r1,=OS_curr					  // OS_curr-*>sp = sp (R13)
	LDR r1,[r1,#0x00]
	STR r13,[r1,#0x00]
	
PendSV_restore
	LDR r1,=OS_next        // sp = OS_next->sp
	LDR r1,[r1,#0x00]
	LDR r13,[r1,#0x00]
	
	LDR r1,=OS_next        // OS_curr = OS_next
	LDR r1,[r1,#0x00]
	LDR r2,=OS_curr
	STR r1,[r2,#0x00]
	
	POP {r4-r11}           // pop registers r4-r11
	
	CPSIE I                // enable global interrupts
	
	BX lr			             // retunr to the next thread	
}


/************ main function *******/
int main()
{
	/* initialize portF, leds, systick timer, enable global interrupt*/
	BSP_init();
	
	/* set priority of PENDSV to lowest priority */
	OS_init();
	
	/* fabricate Cortex-M ISR stack frame for task 1 */
	OSThread_start(&blinky1,              /* container contains sp of task 1 */
								&main_blinky1,          /* task 1 function */
							  stack_blinky1,          /* stack array of task 1 */
								sizeof(stack_blinky1)   /* size of stack array of task 1 */
								);

	/* fabricate Cortex-M ISR stack frame for task 1 */
	OSThread_start(&blinky2,              /* container contains sp of task 2 */
								&main_blinky2,          /* task 2 function */
							  stack_blinky2,          /* stack array of task 2 */
								sizeof(stack_blinky2)   /* size of stack array of task 2 */
								);
	
	OS_start(&blinky1);								
	for (;;);
}






/*
instruction memory
---------
instruction1   0x01
instruction2   0x02
instruction3   0x03

stack
---------
0x00
0x04
0x08
0x0c
..

to understand memory in memory view 
------------------------------------
0x12345678 is stored as 78563412

interrupt
--------------
- in arm after executing each instruction, it check if there is interrupt or not

- when interrupt occurs, processor internally push current values of R0, R1, R2, R3, R12, LR, RC+1, PSR
*/
