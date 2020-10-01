/*
 * Blink LED using gpio calls.
 */

#include <libe/libe.h>


#ifdef TARGET_ESP32
int app_main(int argc, char *argv[])
#else
int main(void)
#endif
{
	uint8_t pin = LED_GPIO0;

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
	ERROR_IF_R(os_ftdi_use(OS_FTDI_GPIO_0_TO_63, FTDI_VID, FTDI_PID, FTDI_DESC, FTDI_SERIAL), 1, "unable to open ftdi device for gpio 0-63");
#endif

	/* gpio init:
	 * in platforms that have different ports (A,B,.. or 1,2,..) for gpio, each
	 * port is chained so that gpio_* can access them using only single number
	 * as pin ID. example in AVR PORTA has gpio 0-7, PORTB has gpio 8-15 and so on.
	 * even on AVRs that do not have PORTA (atmega328), PORTB is still 8-15.
	 *
	 * in this example PB4(12) and PB5(13) are used in AVR platform
	 */
	gpio_output(pin);
	gpio_output(LED_GPIO1);

#ifdef TARGET_RPI
	/* check errors */
	ERROR_IF_R(error_last, 1, "was unable to set gpio as output, reason: %s", error_last);
#endif

	while (1) {
		/* clear watchdog, it is on as default example in pic8 */
		os_wdt_reset();

		NOTICE_MSG("toggling led...");

		/* using gpio with variable like this can be fast or slower depending on optimization
		 * in compiler and whether or not the variable is changed or is "static"
		 */
		gpio_high(pin);

		/* when using with constant pin number like this, this call will be very optimized */
		gpio_low(LED_GPIO1);

		/* previous is same in AVR as writing this */
		// PORTx &= ~(bit << 1);
		/* or in PIC with LATx */
		// LATx &= ~(bit << 1);
		/* or in older PIC with PORTx */
		// PORTx &= ~(bit << 1);

		/* this will call _delay_ms() in AVR, __delay_ms() in PIC, something else in other platforms */
		os_delay_ms(500);

		/* toggle pins on the other direction */
		gpio_low(pin);
		gpio_high(LED_GPIO1);

		/* same delay again */
		os_delay_ms(500);

		/* if the next lines are un-commented, variable pin is not seen as "static" anymore
		 * and optimizer in example AVR platform will make calls to
		 * gpio_*(pin) here much slower. this can be seen from dissambly.
		 */
		// pin++;
		// gpio_output(pin);
	}

	return 0;
}
