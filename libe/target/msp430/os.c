/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#include <libe/libe.h>


int8_t os_init(void)
{
	/* disable watchdog */
	WDTCTL = WDTPW + WDTHOLD;

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
