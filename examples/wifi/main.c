/*
 * WiFi example.
 */

#include <libe/libe.h>
#include <esp_log.h>

#define WIFI_CONFIG_BUTTON_GPIO 0

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
	/* nvm initialization before wifi */
	nvm_init(NULL, 0);
	/* wifi base init */
	wifi_init();

	/* wifi configuration button */
	button_init(WIFI_CONFIG_BUTTON_GPIO);

	while (1) {
		os_wdt_reset();
		// os_delay_ms(500);
		button_pressed(WIFI_CONFIG_BUTTON_GPIO, 1000) {
			INFO_MSG("wifi configuration button pressed");
			button_repeat(WIFI_CONFIG_BUTTON_GPIO, 2000);
		}
	}

	wifi_quit();
	nvm_quit();
	log_quit();
	os_quit();
	return 0;
}
