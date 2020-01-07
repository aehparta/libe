/*
 * Microchip CAP1293 3-channel touch sensor
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifdef USE_I2C

#include "cap1293.h"

int cap1293_open(struct i2c_device *dev, struct i2c_master *master)
{
	uint8_t data[2];
	/* try to detect cap1293 */
	IF_R(i2c_open(dev, master, CAP1293_ADDR), -1);
	IF_R(i2c_write_byte(dev, 0xfd), -1);
	IF_R(i2c_read(dev, data, 2), -1);
	if (data[0] != 0x6f || data[1] != 0x5d) {
		error_set_last("product ID or manufacturer ID does not match");
		return -1;
	}
	return 0;
}

int cap1293_conf(struct i2c_device *dev, uint8_t channels_mask)
{
	/* main control */
	IF_R(i2c_write_reg_byte(dev, 0x00, 0x08), -1);

	/* disable all active inputs for setup */
	IF_R(i2c_write_reg_byte(dev, 0x21, 0x00), -1);

	/* recalibration and repeat delays */
	IF_R(i2c_write_reg_byte(dev, 0x22, 0xaf), -1);

	/* press and hold delay */
	IF_R(i2c_write_reg_byte(dev, 0x23, 0x0f), -1);

	/* interrupts from active channels */
	IF_R(i2c_write_reg_byte(dev, 0x27, channels_mask), -1);

	/* repeat off */
	IF_R(i2c_write_reg_byte(dev, 0x28, 0x00), -1);

	/* signal guard off */
	IF_R(i2c_write_reg_byte(dev, 0x29, 0x00), -1);

	/* configuration */
	IF_R(i2c_write_reg_byte(dev, 0x44, 0x41), -1);

	/* sensitivity */
	IF_R(i2c_write_reg_byte(dev, 0x1f, 0x2f), -1);

	/* disable power button feature */
	IF_R(i2c_write_reg_byte(dev, 0x61, 0x00), -1);

	/* enable/disable channels */
	IF_R(i2c_write_reg_byte(dev, 0x21, channels_mask), -1);

	return 0;
}

int cap1293_read(struct i2c_device *dev)
{
	int value;
	/* read */
	value = i2c_read_reg_byte(dev, 0x00);
	IF_R(value < 0, -1);
	if (value & 1) {
		value = i2c_read_reg_byte(dev, 0x03);
		IF_R(value < 0, -1);
		/* clear interrupt and clear current state at the same time */
		IF_R(i2c_write_reg_byte(dev, 0x00, 0x08), -1);
		return value & 0x7;
	}
	return 0;
}

/* tool related functions */
#ifdef COMPILE_TOOL_I2C

void tool_i2c_cap1293_help(void)
{
	printf(
	    "Commands:\n"
	    "  read         Read touch sensors\n"
	);
}

int tool_i2c_cap1293_exec(struct i2c_master *master, uint8_t address, char *command, int argc, char *argv[])
{
	int err = -1, sensors;
	struct i2c_device dev;

	/* only one command */
	if (strcmp(command, "read")) {
		fprintf(stderr, "Invalid command.\n");
		return -1;
	}

	/* open chip */
	if (cap1293_open(&dev, master)) {
		fprintf(stderr, "Chip not found, reason: %s\n", error_last);
		return -1;
	} else if (cap1293_conf(&dev, 0x07)) {
		fprintf(stderr, "Chip configuration failed, reason: %s\n", error_last);
		return -1;
	}

	/* read */
	while (1) {
		sensors = cap1293_read(&dev);
		if (sensors >= 0) {
			printf("Channel #1 is %s\n", sensors & 1 ? "on" : "off");
			printf("Channel #2 is %s\n", sensors & 2 ? "on" : "off");
			printf("Channel #3 is %s\n", sensors & 4 ? "on" : "off");
		} else {
			fprintf(stderr, "Chip read failed, reason: %s\n", error_last);
			err = -1;
			break;
		}
		os_sleepf(1.0);
	}

	cap1293_close(&dev);

	return err;
}

#endif

#endif
