#ifndef LEDS_H
#define LEDS_H

#include <stdint.h>


/************ led macros *************/
#define RED_LED 0x02
#define BLUE_LED 0x04
#define GREEN_LED 0x08

/********** functions prototypes *********/
void led_init(uint8_t _led);

void led_on(uint8_t _led);

void led_off(uint8_t _led);

void led_toggle(uint8_t _led);

#endif