/*
 * AVR specific os routines as macros.
 */

#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#ifndef _TARGET_OS_H_
#define _TARGET_OS_H_

#define OS_GPIO_AS_MACROS_OR_INLINE     1
#define OS_DELAY_AS_MACROS_OR_INLINE    1

int os_init(void);
#define os_quit()

#define os_delay_ms(x)      _delay_ms(x)
#define os_delay_us(x)      _delay_us(x)

inline int os_gpio_enable(uint8_t pin, bool direction)
{
	switch (pin >> 3) {
#ifdef DDRA
	case 0:
		DDRA = direction ? (DDRA | _BV(pin & 7)) : (DDRA & ~_BV(pin & 7));
		return 0;
#endif
#ifdef DDRB
	case 1:
		DDRB = direction ? (DDRB | _BV(pin & 7)) : (DDRB & ~_BV(pin & 7));
		return 0;
#endif
#ifdef DDRC
	case 2:
		DDRC = direction ? (DDRC | _BV(pin & 7)) : (DDRC & ~_BV(pin & 7));
		return 0;
#endif
#ifdef DDRD
	case 3:
		DDRD = direction ? (DDRD | _BV(pin & 7)) : (DDRD & ~_BV(pin & 7));
		return 0;
#endif
#ifdef DDRE
	case 4:
		DDRE = direction ? (DDRE | _BV(pin & 7)) : (DDRE & ~_BV(pin & 7));
		return 0;
#endif
#ifdef DDRF
	case 5:
		DDRF = direction ? (DDRF | _BV(pin & 7)) : (DDRF & ~_BV(pin & 7));
		return 0;
#endif
	}
	return -1;
}

inline int os_gpio_set(uint8_t pin, bool state)
{
	switch (pin >> 3) {
#ifdef PORTA
	case 0:
		PORTA = state ? (PORTA | _BV(pin & 7)) : (PORTA & ~_BV(pin & 7));
		return 0;
#endif
#ifdef PORTB
	case 1:
		PORTB = state ? (PORTB | _BV(pin & 7)) : (PORTB & ~_BV(pin & 7));
		return 0;
#endif
#ifdef PORTC
	case 2:
		PORTC = state ? (PORTC | _BV(pin & 7)) : (PORTC & ~_BV(pin & 7));
		return 0;
#endif
#ifdef PORTD
	case 3:
		PORTD = state ? (PORTD | _BV(pin & 7)) : (PORTD & ~_BV(pin & 7));
		return 0;
#endif
#ifdef PORTE
	case 4:
		PORTE = state ? (PORTE | _BV(pin & 7)) : (PORTE & ~_BV(pin & 7));
		return 0;
#endif
#ifdef PORTF
	case 5:
		PORTF = state ? (PORTF | _BV(pin & 7)) : (PORTF & ~_BV(pin & 7));
		return 0;
#endif
	}
	return -1;
}

inline uint8_t os_gpio_read(uint8_t pin)
{
	switch (pin >> 3) {
#ifdef PINA
	case 0:
		return PINA & _BV(pin & 7) ? 1 : 0;
#endif
#ifdef PINB
	case 1:
		return PINB & _BV(pin & 7) ? 1 : 0;
#endif
#ifdef PINC
	case 2:
		return PINC & _BV(pin & 7) ? 1 : 0;
#endif
#ifdef PIND
	case 3:
		return PIND & _BV(pin & 7) ? 1 : 0;
#endif
#ifdef PINE
	case 4:
		return PINE & _BV(pin & 7) ? 1 : 0;
#endif
#ifdef PINF
	case 5:
		return PINF & _BV(pin & 7) ? 1 : 0;
#endif
	}
	return 0xff;
}

#endif /* _TARGET_OS_H_ */
