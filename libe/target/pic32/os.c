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

int os_gpio_set(uint8_t pin, bool state)
{
	return 0;
}
