#ifndef MIROS_H
#define MIROS_H

#include <stdint.h>


/************** structures  **************/
/* Thread Control Block (TCB) */
typedef struct
{
	void *sp;
	/* other attributes associated with a thread */
} OSThread;

typedef void (*OSThreadHandler)();


/************ function prototypes ********/
void OS_init();

void OS_sched(void);

void OS_start(void *containers);

void OSThread_start( 
	OSThread *me,
	OSThreadHandler threadHandler,
	void *stkSto,
	uint32_t stkSize);

#endif

