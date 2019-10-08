/*
 * Sensirion SHT31 temperature and humidity sensor
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */


/*
 * @todo This driver is missing features the chip has
 */


#ifdef USE_I2C

#include "sht31.h"


int sht31_open(struct i2c_device *dev, struct i2c_master *master)
{
	return sht31_open_addr(dev, master, SHT31_ADDR);
}

int sht31_open_addr(struct i2c_device *dev, struct i2c_master *master, uint8_t address)
{
	/* try to detect sht31 */
	IF_R(i2c_open(dev, master, address), -1);
	return 0;
}

int sht31_conf(struct i2c_device *dev, bool heater, uint8_t repeatability)
{
	uint8_t data[2];

	/* heater */
	data[0] = 0x30;
	data[1] = heater ? 0x6d : 0x66;
	IF_R(i2c_write(dev, data, 2), -1);

	/* save repeatability */
	switch (repeatability) {
	case SHT31_REPEATABILITY_HIGH:
		dev->driver_bits[0] = 0x00;
		break;
	case SHT31_REPEATABILITY_MEDIUM:
		dev->driver_bits[0] = 0x0b;
		break;
	case SHT31_REPEATABILITY_LOW:
		dev->driver_bits[0] = 0x16;
		break;
	}

	return 0;
}

int sht31_read(struct i2c_device *dev, float *t, float *h)
{
	uint8_t data[6] = { 0x24 };
	data[1] = dev->driver_bits[0];

	/* issue read */
	IF_R(i2c_write(dev, data, 2), -1);

	/* wait for result */
	while (i2c_read(dev, data, 6));

	/* convert result */
	if (t) {
		*t = (float)(data[0] << 8 | data[1]) / 65535.0 * 175 - 45;
	}
	if (h) {
		*h = (float)(data[3] << 8 | data[4]) / 65535.0 * 315 - 49;
	}

	return 0;
}

float sht31_read_temperature(struct i2c_device *dev)
{
	float v;
	IF_R(sht31_read(dev, &v, NULL), -274.0);
	return v;
}

float sht31_read_humidity(struct i2c_device *dev)
{
	float v;
	IF_R(sht31_read(dev, NULL, &v), -1.0);
	return v;
}

/* tool related functions */
#ifdef COMPILE_TOOL_I2C

void tool_i2c_sht31_help(void)
{
	printf("Addresses: 0x%02x, 0x%02x\n\n", SHT31_ADDR_A, SHT31_ADDR_B);
	printf(
	    "Commands:\n"
	    "  read [options]       Read temperature and humidity\n\n"
	    "Options:\n"
	    "  heat                 Enable heater, without this option the heater is disabled\n\n");
	printf("SHT31 repeatability is set to high.\n");
}

int tool_i2c_sht31_exec(struct i2c_master *master, uint8_t address, char *command, int argc, char *argv[])
{
	int err = -1;
	struct i2c_device dev;
	bool heater = false;

	/* only one command */
	if (strcmp(command, "read")) {
		fprintf(stderr, "Invalid command.\n");
		return -1;
	}

	/* parse extra arguments, stupid but simple way */
	for (int i = 1; i < argc; i++) {
		heater = strcmp(argv[i], "heat") == 0 ? true : heater;
	}

	/* if address is zero, selected default address */
	address = address ? address : SHT31_ADDR;

	/* open chip */
	if (sht31_open_addr(&dev, master, address)) {
		fprintf(stderr, "Chip not found, reason: %s\n", error_last);
	} else if (sht31_conf(&dev, heater, SHT31_REPEATABILITY_HIGH)) {
		fprintf(stderr, "Chip initialization failed, reason: %s\n", error_last);
	} else {
		/* execute command */
		float t = -275.15, h = -1.0;
		/* read temperature and humidity in sequence */
		if (!sht31_read(&dev, &t, &h)) {
			printf("%.2f °C\n%.2f %%RH\n", t, h);
			err = 0;
		} else {
			fprintf(stderr, "Failed to read chip.\n");
		}
	}

	sht31_close(&dev);

	return err;
}

#endif

#endif
