#include "leds.h"
#include "tm4c123gh6pm.h"


/*********************** functions ****************/
void led_init(uint8_t _led)
{
	if( (_led == RED_LED) || (_led == BLUE_LED) || (_led == GREEN_LED))
	{
		/* unlock port F */
		GPIO_PORTF_LOCK_R = 0x4C4F434B;
		

		GPIO_PORTF_CR_R |= _led;
		
		/* config PF1,PF2,PF3 as digitial */
		GPIO_PORTF_DIR_R |= _led;
		
		/* config PF1,PF2,PF3 as output */
		GPIO_PORTF_DEN_R |= _led;
		
		GPIO_PORTF_AMSEL_R &= ~_led;
		GPIO_PORTF_AFSEL_R &= ~_led;
		GPIO_PORTF_PCTL_R  &= ~0x0000FFF0;  
		
		/*write 0 in PF1,PF2,PF3 */
		GPIO_PORTF_DATA_R &= ~_led;
	}
}

void led_on(uint8_t _led) { GPIO_PORTF_DATA_R |= (GPIO_PORTF_DATA_R & ~0x0E ) | _led ; }

void led_off(uint8_t _led) { GPIO_PORTF_DATA_R &= ~_led; }

void led_toggle(uint8_t _led) { GPIO_PORTF_DATA_R ^= _led; }