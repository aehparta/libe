/*
 * WiFi example.
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
	/* debug/log init */
	log_init();
	/* wifi base init */
	wifi_init();

	while (1) {
		os_delay_ms(1);
	}

	wifi_quit();
	log_quit();
	os_quit();
	return 0;
}
