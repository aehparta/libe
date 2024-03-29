/*
 * libe cross-platform library: operating system/target specific routines
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include <util/delay.h>
#include <libe/libe.h>

#ifdef USE_AVR_CLOCK_TIMER1_100HZ
static volatile uint8_t hz100 = 0;
static volatile time_t seconds = 0;
#endif


int8_t os_init(void)
{
#ifdef USE_AVR_CLOCK_DIV_DISABLE
#ifdef FUSE_CKDIV8
	/* change clock from default 1MHZ (CKDIV8) to 8MHZ (no CKDIV8) */
	CLKPR = 0x80; /* enable clock divisor change */
	CLKPR = 0x00; /* change clock divisor to one */
#endif
#endif

#ifdef USE_AVR_CLOCK_TIMER1_100HZ
	/* 100 Hz timer */
	TIMSK1 = (1 << OCIE1A); /* compare match A interrupt enable */
	OCR1A = F_CPU / 8 / 100;
	/* enable timer, won't run though until interrupts are enabled too:
	 *  - clock source to F_CPU with prescaler 8
	 *  - mode to CTC
	 */
	TCCR1A = 0x00;
	TCCR1B = (1 << CS11) | (1 << WGM12);
#endif

	/* system time starting point: 2000-01-01T00:00:00Z */
	set_system_time(0);
	set_zone(0);
	set_dst(NULL);

	return 0;
}

time_t os_timei(void)
{
#ifdef USE_AVR_CLOCK_TIMER1_100HZ
	return seconds;
#else
	WARN_MSG("not implemented without USE_AVR_CLOCK_TIMER1_100HZ");
#endif
	return 0;
}

os_time_t os_timef(void)
{
#ifdef USE_AVR_CLOCK_TIMER1_100HZ
	return (os_time_t)seconds + (os_time_t)hz100 / 100;
#else
	WARN_MSG("not implemented without USE_AVR_CLOCK_TIMER1_100HZ");
#endif
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


/* internals */


#ifdef USE_AVR_CLOCK_TIMER1_100HZ
/* catch timer 1 compare interrupt, this must happen at 100 Hz */
ISR(TIMER1_COMPA_vect)
{
	if (hz100 < 99) {
		hz100++;
	} else {
		hz100 = 0;
		seconds++;
		system_tick();
	}
}
#endif
