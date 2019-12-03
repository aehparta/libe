/*
 * Scan i2c bus for devices.
 */

#include <libe/libe.h>
#ifdef TARGET_LINUX
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#ifndef I2C_CONTEXT
#define I2C_CONTEXT NULL
#define I2C_SCL 0
#define I2C_SDA 0
#endif


struct driver {
	char *name;
	int8_t (*open)(struct i2c_device *dev, struct i2c_master *master, int32_t reference, int32_t resolution);
	int8_t (*read)(struct i2c_device *dev, float *t, float *h);
};

struct driver drivers[] = {
	{ "hdc1080", hdc1080_open, hdc1080_read }, /* this must be before sht21, they have same address and only hdc1080 can be detected */
	{ "sht21", sht21_open, sht21_read },
	{ "sht31", sht31_open, sht31_read },
	{ NULL, NULL, NULL }
};

#ifdef TARGET_ESP32
int app_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	void *context = I2C_CONTEXT;
	struct i2c_master master;
	struct i2c_device dev;

	/* base init */
	os_init();
	log_init();

	/* check i2c device if using linux */
#ifdef USE_FTDI
	ERROR_IF_R(os_ftdi_use(OS_FTDI_GPIO_0_TO_63, FTDI_VID, FTDI_PID, FTDI_DESC, FTDI_SERIAL), 1, "unable to open ftdi device for gpio 0-63");
#endif
#if defined(TARGET_LINUX) && !defined(USE_I2C_BITBANG)
	ERROR_IF_R(argc < 2, 1, "give i2c device as first argument");
	context = argv[1];
#endif

	/* open i2c */
	ERROR_IF_R(i2c_master_open(&master, context, 0, I2C_SCL, I2C_SDA), 1, "unable to open i2c device");

	/* try to find a temperature and humidity chip */
	for (int i = 0; drivers[i].open; i++) {
		if (!drivers[i].open(&dev, &master, 0, 0)) {
			printf("Found %s\r\n", drivers[i].name);
			while (1) {
				float t, h;
				drivers[i].read(&dev, &t, &h);
				printf("temperature: %f, humidity: %f\r\n", t, h);
				os_delay_ms(1000);
			}
		} else {
#ifdef USE_ERROR
			printf("%s not found, error: %s\n", drivers[i].name, error_last);
#endif
		}
	}

	ERROR_MSG("did not find any supported t&h chip");

	/* close i2c */
	i2c_master_close(&master);

	log_quit();
	os_quit();

	return 0;
}
