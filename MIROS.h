#ifndef MIROS_H
#define MIROS_H

#include <stdint.h>

/*********** macros ****************/
#define MAXIMUM_NO_OF_TASKS 2


/************** structures  **************/
/* Thread Control Block (TCB) */
typedef struct
{
	void *sp;
	/* other attributes associated with a thread */
} OSThread;

typedef void (*OSThreadHandler)();


/************ function prototypes ********/
void OS_init(void);

void OS_sched(void);

void OS_taskStart( 
	OSThread *me,
	OSThreadHandler threadHandler,
	void *stkSto,
	uint32_t stkSize);

#endif

