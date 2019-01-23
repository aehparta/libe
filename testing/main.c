/*
 * Random testing area for development.
 */

#include <libe/os.h>
#include <libe/debug.h>
#include <libe/drivers/gpio/hd44780.h>
#include <libe/i2c.h>
#include <libe/spi.h>

#ifdef TARGET_ESP32
int app_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	os_init();
	log_init(NULL, 0);

#ifdef USE_FTDI
	WARN_IF(os_ftdi_use(OS_FTDI_GPIO_0_TO_63, 0x0403, 0x6014, NULL, NULL), "unable to open ftdi device for gpio 0-63");
	os_ftdi_set_mpsse(0);
#endif

	// struct i2c_context *i2c = i2c_open(NULL);
	// for (uint8_t a = 0; a < 128; a++) {
	// 	INFO_IF(!i2c_addr7(i2c, a, 0), "device at address %02x", a);
	// 	i2c_stop(i2c);
	// }
	// struct hd44780_device lcd1;
	// hd44780_init(&lcd1, 8, 9, 10, 11, 14, 13, 12);

	struct spi_device dev;
	spi_master_open(NULL, NULL, 0, 20, 21, 19);
	spi_open(&dev, NULL, 18);
	os_gpio_output(0);

	while (1) {
		char d[8] = "12345678";
		spi_transfer(&dev, d, 8);
		DEBUG_MSG("toggling led");
		os_gpio_high(0);
		os_sleepf(0.5);
		os_gpio_low(0);
		os_sleepf(0.5);
	}

	log_quit();
	os_quit();
	return 0;
}
