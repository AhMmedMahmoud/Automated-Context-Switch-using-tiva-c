#include "ports.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

/*********************** functions ****************/
void portF_init(void)
{
	/* enable clock for port F */
	SYSCTL_RCGCGPIO_R |= PORT_F;  
}
