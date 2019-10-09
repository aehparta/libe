/*
 * 8-bit pic specific os routines as macros.
 */

#ifndef _TARGET_OS_H_
#define _TARGET_OS_H_

#define _XTAL_FREQ          F_CPU
#include <xc.h>

void *memcpy(void *dest, const void *src, size_t n);

#define OS_GPIO_AS_MACROS_OR_INLINE     1
#define OS_DELAY_AS_MACROS_OR_INLINE 1
#define OS_GPIO_HAS_PULLUP 1
#define OS_GPIO_HAS_OPEN_DRAIN 1

int os_init(void);
#define os_quit()

#define os_wdt_reset() __asm("clrwdt")

#define _BV(_PIN)           (1 << (_PIN))

#define os_delay_ms(x)      __delay_ms(x)
#define os_delay_us(x)      __delay_us(x)

#if defined(RA0PPS) || defined(RB0PPS) || defined(RC0PPS)
#define OS_HAS_PPS
void os_pin_pps(uint8_t pin, uint8_t action);
#endif


#ifdef PORTA
#define _PIN_IFN_A(pin, condition, reg) if ((pin & 0xf8) == 0x00) { if (condition) { reg ## A &= ~_BV(pin & 7); } else { reg ## A |= _BV(pin & 7); } }
#else
#define _PIN_IFN_A(pin, condition, reg) if(0) {}
#endif
#ifdef PORTB
#define _PIN_IFN_B(pin, condition, reg) if ((pin & 0xf8) == 0x08) { if (condition) { reg ## B &= ~_BV(pin & 7); } else { reg ## B |= _BV(pin & 7); } }
#else
#define _PIN_IFN_B(pin, condition, reg) if(0) {}
#endif
#ifdef PORTC
#define _PIN_IFN_C(pin, condition, reg) if ((pin & 0xf8) == 0x10) { if (condition) { reg ## C &= ~_BV(pin & 7); } else { reg ## C |= _BV(pin & 7); } }
#else
#define _PIN_IFN_C(pin, condition, reg) if(0) {}
#endif
#ifdef PORTD
#define _PIN_IFN_D(pin, condition, reg) if ((pin & 0xf8) == 0x18) { if (condition) { reg ## D &= ~_BV(pin & 7); } else { reg ## D |= _BV(pin & 7); } }
#else
#define _PIN_IFN_D(pin, condition, reg) if(0) {}
#endif
#ifdef PORTE
#define _PIN_IFN_E(pin, condition, reg) if ((pin & 0xf8) == 0x20) { if (condition) { reg ## E &= ~_BV(pin & 7); } else { reg ## E |= _BV(pin & 7); } }
#else
#define _PIN_IFN_E(pin, condition, reg) if(0) {}
#endif
#ifdef PORTF
#define _PIN_IFN_F(pin, condition, reg) if ((pin & 0xf8) == 0x28) { if (condition) { reg ## F &= ~_BV(pin & 7); } else { reg ## F |= _BV(pin & 7); } }
#else
#define _PIN_IFN_F(pin, condition, reg) if(0) {}
#endif

#define os_gpio_enable_(pin, direction) \
	do { \
		if (0) {} \
		else _PIN_IFN_A(pin, direction, TRIS) \
		else _PIN_IFN_B(pin, direction, TRIS) \
		else _PIN_IFN_C(pin, direction, TRIS) \
		else _PIN_IFN_D(pin, direction, TRIS) \
		else _PIN_IFN_E(pin, direction, TRIS) \
		else _PIN_IFN_F(pin, direction, TRIS) \
	} while(0)

static inline void os_gpio_enable(uint8_t pin, bool direction)
{
	switch (pin >> 3) {
#ifdef TRISA
	case 0:
		TRISA = direction ? (TRISA & ~_BV(pin & 7)) : (TRISA | _BV(pin & 7));
		return;
#endif
#ifdef TRISB
	case 1:
		TRISB = direction ? (TRISB & ~_BV(pin & 7)) : (TRISB | _BV(pin & 7));
		return;
#endif
#ifdef TRISC
	case 2:
		TRISC = direction ? (TRISC & ~_BV(pin & 7)) : (TRISC | _BV(pin & 7));
		return;
#endif
#ifdef TRISD
	case 3:
		TRISD = direction ? (TRISD & ~_BV(pin & 7)) : (TRISD | _BV(pin & 7));
		return;
#endif
#ifdef TRISE
	case 4:
		TRISE = direction ? (TRISE & ~_BV(pin & 7)) : (TRISE | _BV(pin & 7));
		return;
#endif
#ifdef TRISF
	case 5:
		TRISF = direction ? (TRISF & ~_BV(pin & 7)) : (TRISF | _BV(pin & 7));
		return;
#endif
	}
	return;
}

#ifdef PORTA
#define _PIN_IF_A(pin, condition, reg) if ((pin & 0xf8) == 0x00) { if (condition) { reg ## A |= _BV(pin & 7); } else { reg ## A &= ~_BV(pin & 7); } }
#else
#define _PIN_IF_A(pin, condition, reg) if(0) {}
#endif
#ifdef PORTB
#define _PIN_IF_B(pin, condition, reg) if ((pin & 0xf8) == 0x08) { if (condition) { reg ## B |= _BV(pin & 7); } else { reg ## B &= ~_BV(pin & 7); } }
#else
#define _PIN_IF_B(pin, condition, reg) if(0) {}
#endif
#ifdef PORTC
#define _PIN_IF_C(pin, condition, reg) if ((pin & 0xf8) == 0x10) { if (condition) { reg ## C |= _BV(pin & 7); } else { reg ## C &= ~_BV(pin & 7); } }
#else
#define _PIN_IF_C(pin, condition, reg) if(0) {}
#endif
#ifdef PORTD
#define _PIN_IF_D(pin, condition, reg) if ((pin & 0xf8) == 0x18) { if (condition) { reg ## D |= _BV(pin & 7); } else { reg ## D &= ~_BV(pin & 7); } }
#else
#define _PIN_IF_D(pin, condition, reg) if(0) {}
#endif
#ifdef PORTE
#define _PIN_IF_E(pin, condition, reg) if ((pin & 0xf8) == 0x20) { if (condition) { reg ## E |= _BV(pin & 7); } else { reg ## E &= ~_BV(pin & 7); } }
#else
#define _PIN_IF_E(pin, condition, reg) if(0) {}
#endif
#ifdef PORTF
#define _PIN_IF_F(pin, condition, reg) if ((pin & 0xf8) == 0x28) { if (condition) { reg ## F |= _BV(pin & 7); } else { reg ## F &= ~_BV(pin & 7); } }
#else
#define _PIN_IF_F(pin, condition, reg) if(0) {}
#endif

#define os_gpio_set_(ping, state) \
	do { \
		if (0) {} \
		else _PIN_IF_A(pin, state, LAT) \
		else _PIN_IF_B(pin, state, LAT) \
		else _PIN_IF_C(pin, state, LAT) \
		else _PIN_IF_D(pin, state, LAT) \
		else _PIN_IF_E(pin, state, LAT) \
		else _PIN_IF_F(pin, state, LAT) \
	} while (0)

static inline void os_gpio_set(uint8_t pin, bool state)
{
	switch (pin >> 3) {
#ifdef LATA /* for pics that have LATx */
	case 0:
		LATA = state ? (LATA | _BV(pin & 7)) : (LATA & ~_BV(pin & 7));
		return;
#elif defined(PORTA) /* for pics that have PORTx */
	case 0:
		PORTA = state ? (PORTA | _BV(pin & 7)) : (PORTA & ~_BV(pin & 7));
		return;
#endif /* LATx */
#ifdef LATB
	case 1:
		LATB = state ? (LATB | _BV(pin & 7)) : (LATB & ~_BV(pin & 7));
		return;
#elif defined(PORTB) /* for pics that have PORTx */
	case 1:
		PORTB = state ? (PORTB | _BV(pin & 7)) : (PORTB & ~_BV(pin & 7));
		return;
#endif /* LATx */
#ifdef LATC
	case 2:
		LATC = state ? (LATC | _BV(pin & 7)) : (LATC & ~_BV(pin & 7));
		return;
#elif defined(PORTC) /* for pics that have PORTx */
	case 2:
		PORTC = state ? (PORTC | _BV(pin & 7)) : (PORTC & ~_BV(pin & 7));
		return;
#endif /* LATx */
#ifdef LATD
	case 3:
		LATD = state ? (LATD | _BV(pin & 7)) : (LATD & ~_BV(pin & 7));
		return;
#elif defined(PORTD) /* for pics that have PORTx */
	case 3:
		PORTD = state ? (PORTD | _BV(pin & 7)) : (PORTD & ~_BV(pin & 7));
		return;
#endif /* LATx */
#ifdef LATE
	case 4:
		LATE = state ? (LATE | _BV(pin & 7)) : (LATE & ~_BV(pin & 7));
		return;
#elif defined(PORTE) /* for pics that have PORTx */
	case 4:
		PORTE = state ? (PORTE | _BV(pin & 7)) : (PORTE & ~_BV(pin & 7));
		return;
#endif /* LATx */
#ifdef LATF
	case 5:
		LATF = state ? (LATF | _BV(pin & 7)) : (LATF & ~_BV(pin & 7));
		return;
#elif defined(PORTF) /* for pics that have PORTx */
	case 5:
		PORTF = state ? (PORTF | _BV(pin & 7)) : (PORTF & ~_BV(pin & 7));
		return;
#endif /* LATx */
	}
	return;
}

static inline int8_t os_gpio_read(uint8_t pin)
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

static inline int8_t os_gpio_pullup(uint8_t pin, bool enable)
{
	switch (pin >> 3) {
#ifdef WPUA
	case 0:
		WPUA = !enable ? (WPUA & ~_BV(pin & 7)) : (WPUA | _BV(pin & 7));
		return 0;
#endif
#ifdef WPUB
	case 1:
		WPUB = !enable ? (WPUB & ~_BV(pin & 7)) : (WPUB | _BV(pin & 7));
		return 0;
#endif
#ifdef WPUC
	case 2:
		WPUC = !enable ? (WPUC & ~_BV(pin & 7)) : (WPUC | _BV(pin & 7));
		return 0;
#endif
#ifdef WPUD
	case 3:
		WPUD = !enable ? (WPUD & ~_BV(pin & 7)) : (WPUD | _BV(pin & 7));
		return 0;
#endif
#ifdef WPUE
	case 4:
		WPUE = !enable ? (WPUE & ~_BV(pin & 7)) : (WPUE | _BV(pin & 7));
		return 0;
#endif
#ifdef WPUF
	case 5:
		WPUF = !enable ? (WPUF & ~_BV(pin & 7)) : (WPUF | _BV(pin & 7));
		return 0;
#endif
	}
	return -1;
}

static inline int8_t os_gpio_open_drain(uint8_t pin, bool enable)
{
	switch (pin >> 3) {
#ifdef ODCONA
	case 0:
		ODCONA = !enable ? (ODCONA & ~_BV(pin & 7)) : (ODCONA | _BV(pin & 7));
		return 0;
#endif
#ifdef ODCONB
	case 1:
		ODCONB = !enable ? (ODCONB & ~_BV(pin & 7)) : (ODCONB | _BV(pin & 7));
		return 0;
#endif
#ifdef ODCONC
	case 2:
		ODCONC = !enable ? (ODCONC & ~_BV(pin & 7)) : (ODCONC | _BV(pin & 7));
		return 0;
#endif
#ifdef ODCOND
	case 3:
		ODCOND = !enable ? (ODCOND & ~_BV(pin & 7)) : (ODCOND | _BV(pin & 7));
		return 0;
#endif
#ifdef ODCONE
	case 4:
		ODCONE = !enable ? (ODCONE & ~_BV(pin & 7)) : (ODCONE | _BV(pin & 7));
		return 0;
#endif
#ifdef ODCONF
	case 5:
		ODCONF = !enable ? (ODCONF & ~_BV(pin & 7)) : (ODCONF | _BV(pin & 7));
		return 0;
#endif
	}
	return -1;
}

#endif /* _TARGET_OS_H_ */
