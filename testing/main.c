/*
 * Random testing area for development.
 */

#include <libe/libe.h>


#ifdef TARGET_ESP32
int app_main(int argc, char *argv[])
#else
int main(void)
#endif
{
	uint8_t x = os_gpio_read(1);

	// os_init();
	// log_init();

	// os_gpio_output(x);
	// os_gpio_set(0, 1);
	// LATA |= 2;

	// log_quit();
	// os_quit();
	return 0;
}
