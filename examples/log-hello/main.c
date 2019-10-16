/*
 * Blink LED using gpio calls.
 */

#include <libe/libe.h>


#ifdef TARGET_ESP32
int app_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	/* low level init: initializes some system basics depending on platform */
	os_init();
	/* debug/log init:
	 *
	 * example in AVR as default, log_init() initializes logging into USART0 with speed
	 * as close to 38400 as possible with given CPU clock
	 */
	log_init();

	while (1) {
		/* clear watchdog, it is on as default example in pic8 */
		os_wdt_reset();
		/* say hello and wait for a second */
		INFO_MSG("Hello world!");
		os_delay_ms(1000);
	}
}
