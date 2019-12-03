/*
 * libe cross-platform library: operating system/target specific routines
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include <util/delay.h>
#include <libe/libe.h>


int8_t os_init(void)
{
	/* disable internal pull-up and set every pin input as default */
#ifdef DDRA
	PORTA = 0x00;
	DDRA = 0x00;
#endif
#ifdef DDRB
	PORTB = 0x00;
	DDRB = 0x00;
#endif
#ifdef DDRC
	PORTC = 0x00;
	DDRC = 0x00;
#endif
#ifdef DDRD
	PORTD = 0x00;
	DDRD = 0x00;
#endif
#ifdef DDRE
	PORTE = 0x00;
	DDRE = 0x00;
#endif
#ifdef DDRF
	PORTF = 0x00;
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
