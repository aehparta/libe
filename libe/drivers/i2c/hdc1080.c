/*
 * Ti HDC1080 temperature and humidity sensor
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifdef USE_I2C

#include "hdc1080.h"


int8_t hdc1080_open(struct i2c_device *dev, struct i2c_master *master, uint8_t ref, int8_t res, int8_t h_res)
{
	uint8_t data[3];

	/* try to detect hdc1080, sht21 has the same address */
	error_if(i2c_open(dev, master, HDC1080_ADDR), -10, "hdc1080 not detected");

	/* read manufacturer id */
	data[0] = 0xfe;
	IF_R(i2c_write(dev, data, 1) != 1, -1);
	IF_R(i2c_read(dev, data, 2) != 2, -1);
	error_if(data[0] != 0x54 || data[1] != 0x49, -11, "invalid manufacturer id read from hdc1080");

	/* read device id */
	data[0] = 0xff;
	IF_R(i2c_write(dev, data, 1) != 1, -1);
	IF_R(i2c_read(dev, data, 2) != 2, -1);
	error_if(data[0] != 0x10 || data[1] != 0x50, -12, "invalid device id read from hdc1080");

	if (res < 0 || h_res < 0) {
		/* skip configuration */
		return 0;
	}

	/* configure */
	data[0] = 0x02;
	data[1] = (1 << 4) | (res == 11 ? 1 : 0);
	if (h_res == 11) {
		data[1] |= 1;
	} else if (h_res == 8) {
		data[1] |= 2;
	}
	data[2] = 0x00;
	error_if(i2c_write(dev, data, 3) != 3, -2, "unable to configure chip");
	/* save configuration so later modifications dont need to read it */
	dev->driver_bits[0] = data[1];

	return 0;
}

int8_t hdc1080_heater(struct i2c_device *dev, bool on)
{
	uint8_t data[3];
	data[0] = 0x02;
	data[1] = dev->driver_bits[0] = on ? dev->driver_bits[0] | 0x20 : dev->driver_bits[0] & 0x1f;
	data[2] = 0x00;

	int8_t err = i2c_write(dev, data, 3);
	if (err == 3) {
		/* configuration written */
		return 0;
	} else if (err < 0) {
		/* device did not respond or other similar error */
		return err;
	}
	/* device propably responded, but something else went wrong */
	return -2;
}

int8_t hdc1080_read(struct i2c_device *dev, float *t, float *h)
{
	uint8_t data[4];

	/* trigger measurement */
	data[0] = 0x00;
	IF_R(i2c_write(dev, data, 1) != 1, -1);

	/* read until result is ready */
	while (i2c_read(dev, data, 4) != 4);

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
	printf(
	    "Commands:\n"
	    "  read [options]       Read temperature and humidity\n"
	    "Options:\n"
	    "  heat                 Enable heater, without this option the heater is disabled\n"
	    "  mode=[0|1]           0 is to aquire temperature and humidity separately, 1 in sequence (default)\n"
	    "  t_res=[14|11]        Temperature resolution in bits, 14 is default\n"
	    "  h_res=[14|11|8]      Humidity resolution in bits, 14 is default\n"
	);
}

int tool_i2c_hdc1080_exec(struct i2c_master *master, uint8_t address, char *command, int argc, char *argv[])
{
	int err = -1;
	struct i2c_device dev;
	bool heater = false;
	uint8_t t_res = 14, h_res = 14;

	/* only one command */
	if (strcmp(command, "read")) {
		fprintf(stderr, "Invalid command.\n");
		return -1;
	}

	/* parse extra arguments, stupid but simple way */
	for (int i = 1; i < argc; i++) {
		heater = strcmp(argv[i], "heat") == 0 ? true : heater;
		t_res = strcmp(argv[i], "t_res=11") == 0 ? 11 : t_res;
		h_res = strcmp(argv[i], "h_res=11") == 0 ? 11 : h_res;
		h_res = strcmp(argv[i], "h_res=8") == 0 ? 8 : h_res;
	}

	/* open chip */
	err = hdc1080_open(&dev, master, 0, t_res, h_res);
	if (err == -2) {
		fprintf(stderr, "Chip initialization failed, reason: %s\n", error_last);
		return -1;
	} else if (err < 0) {
		fprintf(stderr, "Chip not found, reason: %s\n", error_last);
		return -1;
	}

	/* setup heater */
	if (heater) {
		if (hdc1080_heater(&dev, heater) < 0) {
			fprintf(stderr, "Unable to enable heater, reason: %s\n", error_last);
			hdc1080_close(&dev);
			return -1;
		}
	}

	/* execute command */
	float t = -275.15, h = -1.0;
	/* read temperature and humidity in sequence */
	if (!hdc1080_read(&dev, &t, &h)) {
		printf("%.2f °C\n%.2f %%RH\n", t, h);
		err = 0;
	} else {
		fprintf(stderr, "Failed to read chip.\n");
	}

	hdc1080_close(&dev);

	return err;
}

#endif

#endif
