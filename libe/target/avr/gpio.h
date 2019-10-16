/*
 * AVR specific gpio.
 */

#ifndef _TARGET_AVR_GPIO_H_
#define _TARGET_AVR_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>


inline void gpio_enable_inline(uint8_t pin, bool direction)
{
	switch (pin >> 3) {
#ifdef DDRA
	case 0:
		DDRA = direction ? (DDRA | _BV(pin & 7)) : (DDRA & ~_BV(pin & 7));
		return;
#endif
#ifdef DDRB
	case 1:
		DDRB = direction ? (DDRB | _BV(pin & 7)) : (DDRB & ~_BV(pin & 7));
		return;
#endif
#ifdef DDRC
	case 2:
		DDRC = direction ? (DDRC | _BV(pin & 7)) : (DDRC & ~_BV(pin & 7));
		return;
#endif
#ifdef DDRD
	case 3:
		DDRD = direction ? (DDRD | _BV(pin & 7)) : (DDRD & ~_BV(pin & 7));
		return;
#endif
#ifdef DDRE
	case 4:
		DDRE = direction ? (DDRE | _BV(pin & 7)) : (DDRE & ~_BV(pin & 7));
		return;
#endif
#ifdef DDRF
	case 5:
		DDRF = direction ? (DDRF | _BV(pin & 7)) : (DDRF & ~_BV(pin & 7));
		return;
#endif
	}
}

inline void gpio_set_inline(uint8_t pin, uint8_t state)
{
	switch (pin >> 3) {
#ifdef PORTA
	case 0:
		PORTA = state ? (PORTA | _BV(pin & 7)) : (PORTA & ~_BV(pin & 7));
		return;
#endif
#ifdef PORTB
	case 1:
		PORTB = state ? (PORTB | _BV(pin & 7)) : (PORTB & ~_BV(pin & 7));
		return;
#endif
#ifdef PORTC
	case 2:
		PORTC = state ? (PORTC | _BV(pin & 7)) : (PORTC & ~_BV(pin & 7));
		return;
#endif
#ifdef PORTD
	case 3:
		PORTD = state ? (PORTD | _BV(pin & 7)) : (PORTD & ~_BV(pin & 7));
		return;
#endif
#ifdef PORTE
	case 4:
		PORTE = state ? (PORTE | _BV(pin & 7)) : (PORTE & ~_BV(pin & 7));
		return;
#endif
#ifdef PORTF
	case 5:
		PORTF = state ? (PORTF | _BV(pin & 7)) : (PORTF & ~_BV(pin & 7));
		return;
#endif
	}
}

inline uint8_t gpio_read_inline(uint8_t pin)
{
	switch (pin >> 3) {
#ifdef PINA
	case 0:
		return PINA & _BV(pin & 7);
#endif
#ifdef PINB
	case 1:
		return PINB & _BV(pin & 7);
#endif
#ifdef PINC
	case 2:
		return PINC & _BV(pin & 7);
#endif
#ifdef PIND
	case 3:
		return PIND & _BV(pin & 7);
#endif
#ifdef PINE
	case 4:
		return PINE & _BV(pin & 7);
#endif
#ifdef PINF
	case 5:
		return PINF & _BV(pin & 7);
#endif
	}
	return 0;
}

void gpio_enable_callable(uint8_t pin, bool direction);
void gpio_set_callable(uint8_t pin, uint8_t state);
uint8_t gpio_read_callable(uint8_t pin);
// void gpio_pullup_callable(uint8_t pin, bool enable);
// void gpio_open_drain_callable(uint8_t pin, bool enable);
#define gpio_pullup(pin, enable)
#define gpio_open_drain(pin, enable)

#if __STDC_VERSION__ >= 201112L
#define gpio_enable(pin, direction) _Generic((pin), \
	uint8_t: gpio_enable_callable, \
	default: gpio_enable_inline)(pin, direction)
#define gpio_set(pin, state) _Generic((pin), \
	uint8_t: gpio_set_callable, \
	default: gpio_set_inline)(pin, state)
#define gpio_read(pin) _Generic((pin), \
	uint8_t: gpio_read_callable, \
	default: gpio_read_inline)(pin)
#else
#define gpio_enable(pin, direction) gpio_enable_callable(pin, direction)
#define gpio_set(pin, state) gpio_set_callable(pin, state)
#define gpio_read(pin) gpio_read_callable(pin)
#endif


#ifdef __cplusplus
}
#endif

#endif /* _TARGET_AVR_GPIO_H_ */
