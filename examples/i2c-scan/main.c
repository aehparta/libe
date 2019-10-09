/*
 * Scan i2c bus for devices.
 */

#include <libe/libe.h>
#ifdef TARGET_LINUX
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#include "../config.h"


#ifdef TARGET_ESP32
int app_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	void *context = CFG_I2C_CONTEXT;
	struct i2c_master i2c;
	struct i2c_device dev;

	/* base init */
	os_init();
	log_init();
	
	/* check i2c device if using linux */
#ifdef USE_FTDI
	ERROR_IF_R(os_ftdi_use(OS_FTDI_GPIO_0_TO_63, CFG_FTDI_VID, CFG_FTDI_PID, CFG_FTDI_DESC, CFG_FTDI_SERIAL), 1, "unable to open ftdi device for gpio 0-63");
#endif
#if defined(TARGET_LINUX) && !defined(USE_I2C_BITBANG)
	ERROR_IF_R(argc < 2, 1, "give i2c device as first argument");
	context = argv[1];
#endif

	/* open i2c */
	ERROR_IF_R(i2c_master_open(&i2c, context, CFG_I2C_FREQUENCY, CFG_I2C_SCL, CFG_I2C_SDA), 1, "unable to open i2c device");

	/* scan i2c bus */
	int found = 0;
	for (int a = 0; a < 128; a++) {
		if (i2c_open(&dev, &i2c, a) == 0) {
			INFO_MSG("Device found at address %02x", a);
			found++;
		}
		i2c_close(&dev);
	}
	INFO_MSG("Found %d devices.", found);

	/* close i2c */
	i2c_master_close(&i2c);

	log_quit();
	os_quit();

	return 0;
}
