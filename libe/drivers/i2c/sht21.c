/*
 * Sensirion SHT21 temperature and humidity sensor
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifdef USE_I2C

#include "sht21.h"


int8_t sht21_open(struct i2c_device *dev, struct i2c_master *master, int32_t reference, int32_t resolution)
{
	/* try to detect sht21 */
	error_if(i2c_open(dev, master, SHT21_ADDR), -1, "sht21 not detected");
	/* hdc1080 has the same address but there is no way to identify sht21 */

	if (resolution < 0) {
		/* skip configuration */
		return 0;
	}

	uint8_t data[2] = { 0xe7, 0x00 };

	/* read user register first */
	error_if(i2c_write(dev, data, 1), -2, "sht21 user register read failed");
	error_if(i2c_read(dev, data + 1, 1), -2, "sht21 user register read failed");

	/* clear other than reserved bits */
	data[1] &= 0xb9;

	/* setup resolution */
	switch (resolution) {
	default:
	case 14:
		/* RH = 12 bits, T = 14 bits */
		/* this is zero (and default) */
		break;
	case 12:
		/* RH = 8 bits, T = 12 bits */
		data[1] |= 0x01;
		break;
	case 13:
		/* RH = 10 bits, T = 13 bits */
		data[1] |= 0x80;
		break;
	case 11:
		/* RH = 11 bits, T = 11 bits */
		data[1] |= 0x81;
		break;
	}

	/* save configuration so later modifications dont need to read it */
	dev->driver_bits[0] = data[1];

	/* heater */
	// data[1] |= heater << 2;

	/* write */
	data[0] = 0xe6;
	error_if(i2c_write(dev, data, 2), -2, "sht21 configuration failed");

	return 0;
}

int8_t sht21_heater(struct i2c_device *dev, bool on)
{
	uint8_t data[2];
	data[0] = 0xe6;
	data[1] = on ? dev->driver_bits[0] | 0x04 : dev->driver_bits[0] & 0xfb;
	return i2c_write(dev, data, 2);
}

static float sht21_read_temperature(struct i2c_device *dev)
{
	uint8_t data[3];

	/* trigger measurement */
	data[0] = 0xf3;
	IF_R(i2c_write(dev, data, 1), -1);

	/* read until result is ready */
	while (i2c_read(dev, data, 3));

	/* clear status bits */
	data[1] &= 0xfc;

	return (float)(data[0] << 8 | data[1]) / 65536.0 * 175.72 - 46.85;
}

static float sht21_read_humidity(struct i2c_device *dev)
{
	uint8_t data[3];

	/* trigger measurement */
	data[0] = 0xf5;
	IF_R(i2c_write(dev, data, 1), -1);

	/* read until result is ready */
	while (i2c_read(dev, data, 3));

	/* clear status bits */
	data[1] &= 0xfc;

	return (float)(data[0] << 8 | data[1]) / 65536.0 * 125.0 - 6.0;
}

int8_t sht21_read(struct i2c_device *dev, float *t, float *h)
{
	/* read */
	if (t) {
		*t = sht21_read_temperature(dev);
		if (*t < -273.15) {
			return -1;
		}
	}
	if (h) {
		*h = sht21_read_humidity(dev);
		if (*h < 0.0) {
			return -1;
		}
	}

	return 0;
}

/* tool related functions */
#ifdef COMPILE_TOOL_I2C

void tool_i2c_sht21_help(void)
{
	printf(
	    "Commands:\n"
	    "  read [options]       Read temperature and humidity\n"
	    "Options:\n"
	    "  heat                 Enable heater, without this option the heater is disabled\n"
	    "  resolution=[14|13|12|11]\n"
	    "                       SHT21 resolution setting:\n"
	    "                         14: T = 14 bits, RH = 12 bits (default)\n"
	    "                         13: T = 13 bits, RH = 10 bits\n"
	    "                         12: T = 12 bits, RH = 8 bits\n"
	    "                         11: T = 11 bits, RH = 11 bits\n"
	);
}

int tool_i2c_sht21_exec(struct i2c_master *master, uint8_t address, char *command, int argc, char *argv[])
{
	int err = -1;
	struct i2c_device dev;
	bool heater = false;
	uint8_t resolution = 0;

	/* only one command */
	if (strcmp(command, "read")) {
		fprintf(stderr, "Invalid command.\n");
		return -1;
	}

	/* parse extra arguments, stupid but simple way */
	for (int i = 1; i < argc; i++) {
		heater = strcmp(argv[i], "heat") == 0 ? true : heater;
		if (argv[i] == strstr(argv[i], "resolution=") && strlen(argv[i]) == 5) {
			resolution = atoi(&argv[i][11]);
		}
	}

	/* open chip */
<<<<<<< HEAD
	err = sht21_open(&dev, master, 0, res, 0);
=======
	err = sht21_open(&dev, master, 0, resolution);
>>>>>>> 99eb627a0abe7bc8bca59765e49aed4a3edc8931
	if (err == -2) {
		fprintf(stderr, "Chip initialization failed, reason: %s\n", error_last);
		return -1;
	} else if (err < 0) {
		fprintf(stderr, "Chip not found, reason: %s\n", error_last);
		return -1;
	}

	/* setup heater */
	if (heater) {
		if (sht21_heater(&dev, heater)) {
			fprintf(stderr, "Unable to enable heater, reason: %s\n", error_last);
			sht21_close(&dev);
			return -1;
		}
	}

	/* execute command */
	float t = -275.15, h = -1.0;
	/* read temperature and humidity in sequence */
	if (!sht21_read(&dev, &t, &h)) {
		printf("%.2f °C\n%.2f %%RH\n", t, h);
		err = 0;
	} else {
		fprintf(stderr, "Failed to read chip.\n");
	}

	sht21_close(&dev);

	return err;
}

#endif

#endif
