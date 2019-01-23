/*
 * libe cross-platform library: operating system/target specific routines
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include <libe/os.h>
#include <libe/log.h>

#define _BV(_PIN)           (1 << (_PIN))

int os_init(void)
{
	return 0;
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

int8_t os_gpio_enable(uint8_t pin, bool direction)
{
	switch (pin >> 3) {
#ifdef TRISA
	case 0:
		TRISA = direction ? (TRISA & ~_BV(pin & 7)) : (TRISA | _BV(pin & 7));
		return 0;
#endif
#ifdef TRISB
	case 1:
		TRISB = direction ? (TRISB & ~_BV(pin & 7)) : (TRISB | _BV(pin & 7));
		return 0;
#endif
#ifdef TRISC
	case 2:
		TRISC = direction ? (TRISC & ~_BV(pin & 7)) : (TRISC | _BV(pin & 7));
		return 0;
#endif
#ifdef TRISD
	case 3:
		TRISD = direction ? (TRISD & ~_BV(pin & 7)) : (TRISD | _BV(pin & 7));
		return 0;
#endif
#ifdef TRISE
	case 4:
		TRISE = direction ? (TRISE & ~_BV(pin & 7)) : (TRISE | _BV(pin & 7));
		return 0;
#endif
#ifdef TRISF
	case 5:
		TRISF = direction ? (TRISF & ~_BV(pin & 7)) : (TRISF | _BV(pin & 7));
		return 0;
#endif
	}
	return -1;
}

int8_t os_gpio_set(uint8_t pin, bool state)
{
	return -1;
}

int8_t os_gpio_read(uint8_t pin)
{
	return -1;
}
