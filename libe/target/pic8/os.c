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
	/* pll multiplier, 0=4x, 1=3x */
	OSCCONbits.SPLLMULT = 0;
	/* pll enable, if enabled by config then this is ignored */
	OSCCONbits.SPLLEN = 0;
	/* internal oscillator frequency */
	OSCCONbits.IRCF = 0xf;

	/* wait for internal oscillator and pll to start and stabilize */
	while (!OSCSTATbits.HFIOFR || !OSCSTATbits.HFIOFS || !!OSCSTATbits.PLLRDY);
	/* start using internal oscillator */
	OSCCONbits.SCS = 0x2;

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
#ifdef LATA /* for pics that have LATx */
	case 0:
		LATA = state ? (LATA | _BV(pin & 7)) : (LATA & ~_BV(pin & 7));
		return 0;
#elif defined(PORTA) /* for pics that have PORTx */
	case 0:
		PORTA = state ? (PORTA | _BV(pin & 7)) : (PORTA & ~_BV(pin & 7));
		return 0;
#endif /* LATx */
#ifdef LATB
	case 1:
		LATB = state ? (LATB | _BV(pin & 7)) : (LATB & ~_BV(pin & 7));
		return 0;
#elif defined(PORTB) /* for pics that have PORTx */
	case 1:
		PORTB = state ? (PORTB | _BV(pin & 7)) : (PORTB & ~_BV(pin & 7));
		return 0;
#endif /* LATx */
#ifdef LATC
	case 2:
		LATC = state ? (LATC | _BV(pin & 7)) : (LATC & ~_BV(pin & 7));
		return 0;
#elif defined(PORTC) /* for pics that have PORTx */
	case 2:
		PORTC = state ? (PORTC | _BV(pin & 7)) : (PORTC & ~_BV(pin & 7));
		return 0;
#endif /* LATx */
#ifdef LATD
	case 3:
		LATD = state ? (LATD | _BV(pin & 7)) : (LATD & ~_BV(pin & 7));
		return 0;
#elif defined(PORTD) /* for pics that have PORTx */
	case 3:
		PORTD = state ? (PORTD | _BV(pin & 7)) : (PORTD & ~_BV(pin & 7));
		return 0;
#endif /* LATx */
#ifdef LATE
	case 4:
		LATE = state ? (LATE | _BV(pin & 7)) : (LATE & ~_BV(pin & 7));
		return 0;
#elif defined(PORTE) /* for pics that have PORTx */
	case 4:
		PORTE = state ? (PORTE | _BV(pin & 7)) : (PORTE & ~_BV(pin & 7));
		return 0;
#endif /* LATx */
#ifdef LATF
	case 5:
		LATF = state ? (LATF | _BV(pin & 7)) : (LATF & ~_BV(pin & 7));
		return 0;
#elif defined(PORTF) /* for pics that have PORTx */
	case 5:
		PORTF = state ? (PORTF | _BV(pin & 7)) : (PORTF & ~_BV(pin & 7));
		return 0;
#endif /* LATx */
	}
	return -1;
}


uint8_t os_gpio_read(uint8_t pin)
{
	switch (pin >> 3) {
#ifdef PORTA
	case 0:
		return PORTA & _BV(pin & 7) ? 1 : 0;
#endif
#ifdef PORTB
	case 1:
		return PORTB & _BV(pin & 7) ? 1 : 0;
#endif
#ifdef PORTC
	case 2:
		return PORTC & _BV(pin & 7) ? 1 : 0;
#endif
#ifdef PORTD
	case 3:
		return PORTD & _BV(pin & 7) ? 1 : 0;
#endif
#ifdef PORTE
	case 4:
		return PORTE & _BV(pin & 7) ? 1 : 0;
#endif
#ifdef PORTF
	case 5:
		return PORTF & _BV(pin & 7) ? 1 : 0;
#endif
	}
	return 0xff;
}
