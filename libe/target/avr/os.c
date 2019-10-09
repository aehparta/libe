/*
 * libe cross-platform library: operating system/target specific routines
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include <util/delay.h>
#include <libe/libe.h>


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
