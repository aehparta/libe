/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#include <libe/os.h>
#include <libe/log.h>

int os_init(void)
{
	/* every pin as input as default */
#ifdef P1DIR
	P1DIR = 0x00;
#endif
#ifdef P2DIR
	P2DIR = 0x00;
#endif
#ifdef P3DIR
	P3DIR = 0x00;
#endif
#ifdef P4DIR
	P4DIR = 0x00;
#endif
#ifdef P5DIR
	P5DIR = 0x00;
#endif
#ifdef P6DIR
	P6DIR = 0x00;
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


int os_gpio_enable(uint8_t pin, bool direction)
{
	switch (pin >> 3) {
#ifdef __MSP430_HAS_P1SEL__
	case 0:
		P1DIR = direction ? (P1DIR | _BV(pin & 7)) : (P1DIR & ~_BV(pin & 7));
		return 0;
#endif
#ifdef __MSP430_HAS_P2SEL__
	case 1:
		P2DIR = direction ? (P2DIR | _BV(pin & 7)) : (P2DIR & ~_BV(pin & 7));
		return 0;
#endif
#ifdef __MSP430_HAS_P3SEL__
	case 2:
		P3DIR = direction ? (P3DIR | _BV(pin & 7)) : (P3DIR & ~_BV(pin & 7));
		return 0;
#endif
#ifdef __MSP430_HAS_P4SEL__
	case 3:
		P4DIR = direction ? (P4DIR | _BV(pin & 7)) : (P4DIR & ~_BV(pin & 7));
		return 0;
#endif
#ifdef __MSP430_HAS_P5SEL__
	case 4:
		P5DIR = direction ? (P5DIR | _BV(pin & 7)) : (P5DIR & ~_BV(pin & 7));
		return 0;
#endif
#ifdef __MSP430_HAS_P6SEL__
	case 5:
		P6DIR = direction ? (P6DIR | _BV(pin & 7)) : (P6DIR & ~_BV(pin & 7));
		return 0;
#endif
	}
	return -1;
}

int os_gpio_set(uint8_t pin, bool state)
{
	switch (pin >> 3) {
#ifdef __MSP430_HAS_P1SEL__
	case 0:
		P1OUT = state ? (P1OUT | _BV(pin & 7)) : (P1OUT & ~_BV(pin & 7));
		return 0;
#endif
#ifdef __MSP430_HAS_P2SEL__
	case 1:
		P2OUT = state ? (P2OUT | _BV(pin & 7)) : (P2OUT & ~_BV(pin & 7));
		return 0;
#endif
#ifdef __MSP430_HAS_P3SEL__
	case 2:
		P3OUT = state ? (P3OUT | _BV(pin & 7)) : (P3OUT & ~_BV(pin & 7));
		return 0;
#endif
#ifdef __MSP430_HAS_P4SEL__
	case 3:
		P4OUT = state ? (P4OUT | _BV(pin & 7)) : (P4OUT & ~_BV(pin & 7));
		return 0;
#endif
#ifdef __MSP430_HAS_P5SEL__
	case 4:
		P5OUT = state ? (P5OUT | _BV(pin & 7)) : (P5OUT & ~_BV(pin & 7));
		return 0;
#endif
#ifdef __MSP430_HAS_P6SEL__
	case 5:
		P6OUT = state ? (P6OUT | _BV(pin & 7)) : (P6OUT & ~_BV(pin & 7));
		return 0;
#endif
	}
	return -1;
}

int os_gpio_read(uint8_t pin)
{
	switch (pin >> 3) {
#ifdef P1IN
	case 0:
		return P1IN & _BV(pin & 7) ? 1 : 0;
#endif
#ifdef P2IN
	case 1:
		return P2IN & _BV(pin & 7) ? 1 : 0;
#endif
#ifdef P3IN
	case 2:
		return P3IN & _BV(pin & 7) ? 1 : 0;
#endif
#ifdef P4IN
	case 3:
		return P4IN & _BV(pin & 7) ? 1 : 0;
#endif
#ifdef P5IN
	case 4:
		return P5IN & _BV(pin & 7) ? 1 : 0;
#endif
#ifdef P6IN
	case 5:
		return P6IN & _BV(pin & 7) ? 1 : 0;
#endif
	}
	return 0xff;
}
