/*
 * Blink LED using gpio calls.
 */

#include <libe/libe.h>
#include "../config.h"


#ifdef TARGET_ESP32
int app_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	uint8_t pin = CFG_LED_GPIO0;

	/* low level init: initializes some system basics depending on platform */
	os_init();
	/* debug/log init:
	 *
	 * example in AVR as default, log_init() initializes logging into USART0 with speed
	 * as close to 38400 as possible with given CPU clock
	 */
	log_init();

	/* ftdi init */
#ifdef USE_FTDI
	ERROR_IF_R(os_ftdi_use(OS_FTDI_GPIO_0_TO_63, CFG_FTDI_VID, CFG_FTDI_PID, CFG_FTDI_DESC, CFG_FTDI_SERIAL), 1, "unable to open ftdi device for gpio 0-63");
#endif

	/* gpio init:
	 * in platforms that have different ports (A,B,.. or 1,2,..) for gpio, each
	 * port is chained so that os_gpio_* can access them using only single number
	 * as pin ID. example in AVR PORTA has gpio 0-7, PORTB has gpio 8-15 and so on.
	 * even on AVRs that do not have PORTA (atmega328), PORTB is still 8-15.
	 *
	 * in this example PB4(12) and PB5(13) are used in AVR platform
	 */
	os_gpio_output(pin);
	os_gpio_output(CFG_LED_GPIO1);

	while (1) {
		/* clear watchdog, it is on as default example in pic8 */
		os_wdt_reset();

		NOTICE_MSG("toggling led...");
		
		/* using gpio with variable like this can be fast or slower depending on optimization
		 * in compiler and whether or not the variable is changed or is "static"
		 */
		os_gpio_high(pin);

		/* when using with constant pin number like this, this call will be very optimized */
		os_gpio_low(CFG_LED_GPIO1);
#ifdef TARGET_AVR
		/* previous is same in AVR as writing this */
		PORTB &= ~(1 << 1);
#elif defined(LATB) && defined(TARGET_PIC8)
		/* or in PIC with LATB */
		LATB &= ~(1 << 1);
#elif defined(PORTB) && defined(TARGET_PIC8)
		/* or in older PIC with PORTB */
		PORTB &= ~(1 << 1);
#endif

		/* this will call _delay_ms() in AVR, __delay_ms() in PIC, something else in other platforms */
		os_delay_ms(500);

		/* toggle pins on the other direction */
		os_gpio_low(pin);
		os_gpio_high(CFG_LED_GPIO1);

		/* same delay again */
		os_delay_ms(500);

		/* if the next line is un-commented, variable pin is not seen as "static" anymore
		 * and optimizer in example AVR platform will make calls to
		 * os_gpio_*(pin) here much slower. this can be seen from dissambly.
		 */
		// pin++;
	}
}
