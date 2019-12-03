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


int8_t sht31_open(struct i2c_device *dev, struct i2c_master *master, int32_t reference, int32_t repeatability)
{
	uint8_t addr = reference ? SHT31_ADDR : reference;
	/* try to detect sht31 */
	error_if(i2c_open(dev, master, addr), -1, "sht31 not detected");
	/* save repeatability */
	dev->driver_bits[0] = (uint8_t)repeatability;
	return 0;
}

int8_t sht31_heater(struct i2c_device *dev, bool on)
{
	uint8_t data[2];
	data[0] = 0x30;
	data[1] = on ? 0x6d : 0x66;
	return i2c_write(dev, data, 2);
}

int8_t sht31_read(struct i2c_device *dev, float *t, float *h)
{
	uint8_t data[6];

	/* issue read */
	data[0] = 0x24;
	data[1] = dev->driver_bits[0];
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
	err = sht31_open(&dev, master, address, SHT31_REPEATABILITY_HIGH);
	if (err == -2) {
		fprintf(stderr, "Chip initialization failed, reason: %s\n", error_last);
		return -1;
	} else if (err < 0) {
		fprintf(stderr, "Chip not found, reason: %s\n", error_last);
		return -1;
	}

	/* setup heater */
	if (heater) {
		if (sht31_heater(&dev, heater)) {
			fprintf(stderr, "Unable to enable heater, reason: %s\n", error_last);
			sht31_close(&dev);
			return -1;
		}
	}

	/* execute command */
	float t = -275.15, h = -1.0;
	/* read temperature and humidity in sequence */
	if (!sht31_read(&dev, &t, &h)) {
		printf("%.2f °C\n%.2f %%RH\n", t, h);
		err = 0;
	} else {
		fprintf(stderr, "Failed to read chip.\n");
	}

	sht31_close(&dev);

	return err;
}

#endif

#endif
