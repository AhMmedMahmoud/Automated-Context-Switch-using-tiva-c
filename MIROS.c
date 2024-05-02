#include "MIROS.h"
#include "tm4c123gh6pm.h"


OSThread *OS_curr;
OSThread *OS_next;
uint32_t tickCount = 0;
uint8_t flag = 0;
OSThread *startingTask;


/****************** OS *************/
void OS_init()
{

	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & ~((1<<21 | 1<<22 | 1<<23))) | ((1<<21 | 1<<22 | 1<<23));  // set PendSV interrupt priority (lowest 7) 
}

void OS_sched()
{
	/* OS_next = .... */
	if((tickCount%1 == 0) && flag == 0)
	{
		OS_next = startingTask;
		flag = 1;
	}
	else if((tickCount%1 == 0) && flag == 1)
	{
		OS_next = ((OSThread *)startingTask)+1;
		flag = 0;
	}
	
	// force context switching
	if (OS_next != OS_curr)
	{
		NVIC_INT_CTRL_R |= (1u << 28);	// trigger PendSV interrupt
	}
}

void OSThread_start( 
	OSThread *me,
	OSThreadHandler threadHandler,
	void *stkSto,
	uint32_t stkSize)
{
	uint32_t *sp = (uint32_t *) ((((uint32_t)stkSto + stkSize) / 8) *8);
	uint32_t *stk_limit;
	
	*(--sp) = (1u << 24);   				  	 /* xPSR = 00000001   */
	*(--sp) = (uint32_t)threadHandler;   /* PC   = 38040000 + 01000000 = 39040000   */
	*(--sp) = 0x0000000EU;    					 /* LR   = 0E000000   */
	*(--sp) = 0x0000000CU;     					 /* R12  = 0C000000   */
	*(--sp) = 0x00000003U;    					 /* R3   = 03000000   */
	*(--sp) = 0x00000002U;    					 /* R2   = 02000000   */
	*(--sp) = 0x00000001U;     					 /* R1   = 01000000   */
	*(--sp) = 0x00000000U;               /* R0   = 00000000   */
	/* additionality, fake registers R4-R11 */
	*(--sp) = 0x0000000BU;   				  	 /* R11  = 0B00000000  */
	*(--sp) = 0x0000000AU;  						 /* R10  = 0A00000000  */
	*(--sp) = 0x00000009U;    					 /* R9   = 0900000000  */
	*(--sp) = 0x00000008U;     					 /* R8   = 0800000000  */
	*(--sp) = 0x00000007U;    					 /* R7   = 0700000000  */
	*(--sp) = 0x00000006U;    					 /* R6   = 0600000000  */
	*(--sp) = 0x00000005U;     					 /* R5   = 0500000000  */
	*(--sp) = 0x00000004U;               /* R4   = 0400000000  */
	
	/*
	task 1        task 2
	04000000      04000000
	05000000      05000000
	06000000      06000000
	07000000      07000000
	08000000      08000000
	09000000      09000000
	0A000000      0A000000
	0B000000      0B000000
	00000000      00000000
	01000000      01000000
	02000000      02000000
	03000000      03000000
	0C000000      0C000000
	0E000000      0E000000
	75030000			93030000
	00000001      00000001
	*/
	/* save the top of the stack in the thread's attribure */
	me->sp = sp;
	stk_limit = (uint32_t *)(((((uint32_t)stkSto - 1U) / 8) + 1U) * 8);
	/* pre-fill the unused part of the stack with 0xDEADBEEF */
	for (sp = sp - 1U; sp >= stk_limit; --sp)
	{
		*sp = 0xDEADBEEFU;
	}
}

void OS_start(void *containers)
{
	startingTask = (OSThread *)containers;
}
		