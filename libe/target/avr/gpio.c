/*
 * AVR specific gpio.
 */

#ifdef USE_GPIO

#include <libe/libe.h>


void gpio_enable_callable(uint8_t pin, bool direction)
{
	return gpio_enable_inline(pin, direction);
}

void gpio_set_callable(uint8_t pin, uint8_t state)
{
	return gpio_set_inline(pin, state);
}

uint8_t gpio_read_callable(uint8_t pin)
{
	return gpio_read_inline(pin);
}

#endif
