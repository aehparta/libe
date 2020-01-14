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

	/* system time starting point: 2000-01-01T00:01:00Z */
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
