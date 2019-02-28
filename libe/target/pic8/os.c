/*
 * libe cross-platform library: operating system/target specific routines
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include <math.h>
#include <libe/os.h>
#include <libe/log.h>


int os_init(void)
{
#ifdef OSCCONbits
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
#elif OSCCON
	OSCCON = 0x72;
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
		os_delay_ms(999);
	}
}

void os_sleepf(os_time_t t)
{
	for (int i = (int)(t * 1000); i > 0; i--) {
		os_delay_us(830);
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


int8_t os_gpio_read(uint8_t pin)
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
	return -1;
}


/* pic8 specific stuff */


#if defined(RA0PPS) || defined(RB0PPS) || defined(RC0PPS)
void os_pin_pps(uint8_t pin, uint8_t action)
{
	switch (pin) {
	/* port A */
#ifdef RA0PPS
	case 0:
		RA0PPS = action;
		break;
#endif
#ifdef RA1PPS
	case 1:
		RA1PPS = action;
		break;
#endif
#ifdef RA2PPS
	case 2:
		RA2PPS = action;
		break;
#endif
#ifdef RA3PPS
	case 3:
		RA3PPS = action;
		break;
#endif
#ifdef RA4PPS
	case 4:
		RA4PPS = action;
		break;
#endif
#ifdef RA5PPS
	case 5:
		RA5PPS = action;
		break;
#endif
#ifdef RA6PPS
	case 6:
		RA6PPS = action;
		break;
#endif
#ifdef RA7PPS
	case 7:
		RA7PPS = action;
		break;
#endif
	/* port B */
#ifdef RB0PPS
	case 8:
		RB0PPS = action;
		break;
#endif
#ifdef RB1PPS
	case 9:
		RB1PPS = action;
		break;
#endif
#ifdef RB2PPS
	case 10:
		RB2PPS = action;
		break;
#endif
#ifdef RB3PPS
	case 11:
		RB3PPS = action;
		break;
#endif
#ifdef RB4PPS
	case 12:
		RB4PPS = action;
		break;
#endif
#ifdef RB5PPS
	case 13:
		RB5PPS = action;
		break;
#endif
#ifdef RB6PPS
	case 14:
		RB6PPS = action;
		break;
#endif
#ifdef RB7PPS
	case 15:
		RB7PPS = action;
		break;
#endif
	/* port C */
#ifdef RC0PPS
	case 16:
		RC0PPS = action;
		break;
#endif
#ifdef RC1PPS
	case 17:
		RC1PPS = action;
		break;
#endif
#ifdef RC2PPS
	case 18:
		RC2PPS = action;
		break;
#endif
#ifdef RC3PPS
	case 19:
		RC3PPS = action;
		break;
#endif
#ifdef RC4PPS
	case 20:
		RC4PPS = action;
		break;
#endif
#ifdef RC5PPS
	case 21:
		RC5PPS = action;
		break;
#endif
#ifdef RC6PPS
	case 22:
		RC6PPS = action;
		break;
#endif
#ifdef RC7PPS
	case 23:
		RC7PPS = action;
		break;
#endif
	/* port D */
#ifdef RD0PPS
	case 24:
		RD0PPS = action;
		break;
#endif
#ifdef RD1PPS
	case 25:
		RD1PPS = action;
		break;
#endif
#ifdef RD2PPS
	case 26:
		RD2PPS = action;
		break;
#endif
#ifdef RD3PPS
	case 27:
		RD3PPS = action;
		break;
#endif
#ifdef RD4PPS
	case 28:
		RD4PPS = action;
		break;
#endif
#ifdef RD5PPS
	case 29:
		RD5PPS = action;
		break;
#endif
#ifdef RD6PPS
	case 30:
		RD6PPS = action;
		break;
#endif
#ifdef RD7PPS
	case 31:
		RD7PPS = action;
		break;
#endif
	/* port E */
#ifdef RE0PPS
	case 32:
		RE0PPS = action;
		break;
#endif
#ifdef RE1PPS
	case 33:
		RE1PPS = action;
		break;
#endif
#ifdef RE2PPS
	case 34:
		RE2PPS = action;
		break;
#endif
#ifdef RE3PPS
	case 35:
		RE3PPS = action;
		break;
#endif
#ifdef RE4PPS
	case 36:
		RE4PPS = action;
		break;
#endif
#ifdef RE5PPS
	case 37:
		RE5PPS = action;
		break;
#endif
#ifdef RE6PPS
	case 38:
		RE6PPS = action;
		break;
#endif
#ifdef RE7PPS
	case 39:
		RE7PPS = action;
		break;
#endif
	/* port F */
#ifdef RF0PPS
	case 40:
		RF0PPS = action;
		break;
#endif
#ifdef RF1PPS
	case 41:
		RF1PPS = action;
		break;
#endif
#ifdef RF2PPS
	case 42:
		RF2PPS = action;
		break;
#endif
#ifdef RF3PPS
	case 43:
		RF3PPS = action;
		break;
#endif
#ifdef RF4PPS
	case 44:
		RF4PPS = action;
		break;
#endif
#ifdef RF5PPS
	case 45:
		RF5PPS = action;
		break;
#endif
#ifdef RF6PPS
	case 46:
		RF6PPS = action;
		break;
#endif
#ifdef RF7PPS
	case 47:
		RF7PPS = action;
		break;
#endif
	}
}
#endif
