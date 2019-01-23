/*
 * libe cross-platform library: operating system/target specific routines
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include <math.h>
#include <libe/os.h>
#include <libe/debug.h>


int os_init(void)
{
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
	switch (pin >> 3) {
#ifdef LATA
	case 0:
		LATA = state ? (LATA | _BV(pin & 7)) : (LATA & ~_BV(pin & 7));
		return 0;
#endif
#ifdef LATB
	case 1:
		LATB = state ? (LATB | _BV(pin & 7)) : (LATB & ~_BV(pin & 7));
		return 0;
#endif
#ifdef LATC
	case 2:
		LATC = state ? (LATC | _BV(pin & 7)) : (LATC & ~_BV(pin & 7));
		return 0;
#endif
#ifdef LATD
	case 3:
		LATD = state ? (LATD | _BV(pin & 7)) : (LATD & ~_BV(pin & 7));
		return 0;
#endif
#ifdef LATE
	case 4:
		LATE = state ? (LATE | _BV(pin & 7)) : (LATE & ~_BV(pin & 7));
		return 0;
#endif
#ifdef LATF
	case 5:
		LATF = state ? (LATF | _BV(pin & 7)) : (LATF & ~_BV(pin & 7));
		return 0;
#endif
	}
	return -1;
}


int8_t os_gpio_read(uint8_t pin)
{
	switch (pin >> 3) {
#ifdef LATA
	case 0:
		return LATA & _BV(pin & 7) ? 1 : 0;
#endif
#ifdef LATB
	case 1:
		return LATB & _BV(pin & 7) ? 1 : 0;
#endif
#ifdef LATC
	case 2:
		return LATC & _BV(pin & 7) ? 1 : 0;
#endif
#ifdef LATD
	case 3:
		return LATD & _BV(pin & 7) ? 1 : 0;
#endif
#ifdef LATE
	case 4:
		return LATE & _BV(pin & 7) ? 1 : 0;
#endif
#ifdef LATF
	case 5:
		return LATF & _BV(pin & 7) ? 1 : 0;
#endif
	}
	return -1;
}
