/*
 * Random testing area for development.
 */

#include <libe/os.h>
#include <libe/debug.h>

#ifdef TARGET_ESP32
int app_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	P1DIR = 0xf0;
	P1OUT = 0xaa;
	while (1);

	os_init();
	log_init(NULL, 0);

#ifdef USE_FTDI
	WARN_IF(os_ftdi_use(OS_FTDI_GPIO_0_TO_63, 0x0403, 0x6014, NULL, NULL), "unable to open ftdi device for gpio 0-63");
	os_ftdi_set_mpsse(0);
	// WARN_IF(os_ftdi_use(OS_FTDI_GPIO_64_TO_127, 0x0403, 0x6011, NULL, NULL), "unable to open ftdi device for gpio 64-127");
#endif

	os_gpio_output(8);

	// while (1) {
	// 	DEBUG_MSG("%d", os_gpio_read(0));
	// 	os_sleepf(0.5);
	// }
	// return 0;

	// os_gpio_output(115);
	// os_gpio_output(116);
	// os_gpio_output(117);


	while (1) {
		DEBUG_MSG("toggling led");
		os_gpio_high(8);
		// os_gpio_high(117);
		// os_gpio_low(115);
		// os_gpio_low(116);
		os_sleepf(0.5);
		os_gpio_low(8);
		// os_gpio_low(117);
		// os_gpio_high(115);
		// os_gpio_low(116);
		os_sleepf(0.5);
		// os_gpio_high(8);
		// os_gpio_low(117);
		// os_gpio_low(115);
		// os_gpio_high(116);
		// os_sleepf(0.5);
	}

	log_quit();
	os_quit();
	return 0;
}
