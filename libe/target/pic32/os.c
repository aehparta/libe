/*
 * libe cross-platform library: operating system/target specific routines
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include <libe/os.h>
#include <libe/log.h>


int os_init(void)
{
	return 0;
}

void os_delay_ms(uint32_t ms)
{
}

void os_delay_us(uint32_t us)
{
}

time_t os_timei(void)
{
	return 0;
}

os_time_t os_timef(void)
{
	return 0;
}

void os_sleepi(time_t t)
{
}

void os_sleepf(os_time_t t)
{
}
