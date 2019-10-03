/*
 * libe cross-platform library: Ti HDC1080 driver
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifdef USE_I2C

#include <libe/log.h>
#include "hdc1080.h"


int hdc1080_open(struct i2c_device *dev, struct i2c_master *master)
{
	uint8_t data[3];

	/* try to detect hdc1080, sht21 has the same address */
	IF_R(i2c_open(dev, master, HDC1080_ADDR), -1);

	/* read manufacturer id */
	data[0] = 0xfe;
	IF_R(i2c_write(dev, data, 1), -1);
	IF_R(i2c_read(dev, data, 2), -1);
	IF_R(data[0] != 0x54 || data[1] != 0x49, -1);

	/* read device id */
	data[0] = 0xff;
	IF_R(i2c_write(dev, data, 1), -1);
	IF_R(i2c_read(dev, data, 2), -1);
	IF_R(data[0] != 0x10 || data[1] != 0x50, -1);

	/* configure sensor:
	 *  - do software reset
	 *  - heater: off
	 *  - mode: temperature and humidity are acquired together
	 *  - tres: 14 bits
	 *  - hres: 14 bits
	 */
	data[0] = 0x02;
	data[1] = 0x10;
	data[2] = 0x00;
	IF_R(i2c_write(dev, data, 3), -1);

	return 0;
}

int hdc1080_read(struct i2c_device *dev, float *t, float *h)
{
	uint8_t data[4];

	/* trigger measurement */
	data[0] = 0x00;
	IF_R(i2c_write(dev, data, 1), -1);

	/* read until result is ready */
	while (i2c_read(dev, data, 4));

	/* convert result bits to actual temperature */
	if (t) {
		*t = (float)(data[0] << 8 | data[1]) / 65536.0 * 165.0 - 40.0;
	}
	if (h) {
		*h = (float)(data[2] << 8 | data[3]) / 65536.0 * 100.0;
	}

	return 0;
}

/* tool related functions */
#ifdef COMPILE_TOOL_I2C

void tool_i2c_hdc1080_help(void)
{
	printf("HDC1080\n");
}

void tool_i2c_hdc1080_exec(char *command, int argc, char *argv[])
{
	printf("exec command %s\n", command);
}

#endif

#endif
