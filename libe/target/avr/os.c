/*
 * libe cross-platform library: operating system/target specific routines
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include <util/delay.h>
#include <libe/os.h>
#include <libe/debug.h>


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

#ifdef FUSE_CKDIV8
	/* change clock from default 1MHZ (CKDIV8) to 8MHZ (no CKDIV8) */
	CLKPR = 0x80; /* enable clock divisor change */
	CLKPR = 0x00; /* change clock divisor to one */
#endif

#ifdef REFS1
	/* read random seed from adc channel 7 which is not connected to anything,
	 * best that can be done atm,
	 * should be at least somewhat random due to system noice
	 */
	ADMUX = (1 << REFS1) | (1 << REFS0) | 7;
	ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN) | (1 << ADSC);
	uint16_t seed = 0;
	for (uint8_t i = 0; i < 16; i += 2) {
		while (!(ADCSRA & (1 << ADIF)));
		seed |= (ADCL & 3) << i;
		seed |= (ADCH & 0);
		ADCSRA |= (1 << ADSC) | (1 << ADIF);
		_delay_ms(10);
	}
	ADMUX = 0;
	ADCSRA = 0;
	srand(seed);
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
