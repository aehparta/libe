/*
 * libe cross-platform library: operating system/target specific routines
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include <libe/os.h>
#include <libe/debug.h>


int os_init(void)
{
	return 0;
}

os_time_t os_time(void)
{
	return 0;
}

void os_sleep(os_time_t t)
{
}

int os_gpio_enable(uint8_t pin, bool direction)
{
	return 0;
}

int os_gpio_output(uint8_t pin)
{
	return os_gpio_enable(pin, OS_GPIO_OUTPUT);
}

int os_gpio_input(uint8_t pin)
{
	return os_gpio_enable(pin, OS_GPIO_INPUT);
}

int os_gpio_set(uint8_t pin, bool state)
{
	return 0;
}

int os_gpio_high(uint8_t pin)
{
	return os_gpio_set(pin, true);
}

int os_gpio_low(uint8_t pin)
{
	return os_gpio_set(pin, false);
}
