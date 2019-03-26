/*
 * Random testing area for development.
 */

#include <libe/os.h>
#include <libe/log.h>
#include <libe/drivers/i2c/hdc1080.h>
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
	void *context = NULL;

#ifdef USE_FTDI
	ERROR_IF_R(os_ftdi_use(OS_FTDI_GPIO_0_TO_63, 0x0403, 0x6011, NULL, NULL), 1, "unable to open ftdi device for gpio 0-63");
	// os_ftdi_set_mpsse(0);
#endif
#if defined(TARGET_LINUX) && !defined(USE_I2C_BITBANG)
	ERROR_IF_R(argc < 2, 1, "give i2c device as first argument");
	context = argv[1];
#endif

	struct i2c_master i2c;
	struct i2c_device dev;

	// i2c_master_open(&i2c, context, 100000, 0, 1);
	i2c_master_open(&i2c, context, 100000, 30, 29);
	ERROR_IF_R(hdc1080_open(&dev, &i2c), 1, "unable to open hdc1080");

	while (1) {
		float t, h;
		ERROR_IF_R(hdc1080_read(&dev, &t, &h), 1, "unable to read hdc1080 values");
		DEBUG_MSG("t: %3.2f, h: %3.2f", t, h);
		os_sleepi(1);
	}

	hdc1080_close(&dev);
	i2c_master_close(&i2c);

	log_quit();
	os_quit();
	return 0;
}
