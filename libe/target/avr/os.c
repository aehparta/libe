/*
 * libe cross-platform library: operating system/target specific routines
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include <util/delay.h>
#include <libe/libe.h>


int os_init(void)
{
	/* every pin as input as default */
#ifdef DDRA
	DDRA = 0x00;
#endif
#ifdef DDRB
	DDRB = 0x00;
#endif
#ifdef DDRC
	DDRC = 0x00;
#endif
#ifdef DDRD
	DDRD = 0x00;
#endif
#ifdef DDRE
	DDRE = 0x00;
#endif
#ifdef DDRF
	DDRF = 0x00;
#endif

	return 0;
}

time_t os_timei(void)
{
	WARN_MSG("not implemented");
	return 0;
}

os_time_t os_timef(void)
{
	WARN_MSG("not implemented");
	return 0;
}

void os_sleepi(time_t t)
{
	for (time_t i = 0; i < t; i++) {
		os_delay_ms(1000);
	}
}

void os_sleepf(os_time_t t)
{
	for (int i = (int)(t * 1000); i > 0; i--) {
		os_delay_us(999);
	}
}

void os_gpio_enable_callable(uint8_t pin, bool direction)
{
	return os_gpio_enable_inline(pin, direction);
}

void os_gpio_set_callable(uint8_t pin, bool state)
{
	return os_gpio_set_inline(pin, state);
}

uint8_t os_gpio_read_callable(uint8_t pin)
{
	return os_gpio_read_inline(pin);
}
