/*
 * Microchip MCP4725 12-bit DAC
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#include <libe/libe.h>

#ifdef USE_I2C

int8_t mcp4725_open(struct i2c_device *dev, struct i2c_master *master, uint8_t address)
{
	uint8_t b[5];
	IF_R(i2c_open(dev, master, address > 0 ? address : MCP4725_ADDR), -1);
	IF_R(i2c_read(dev, b, 5), -1);
	dev->driver_bits[0] = 0x40 | ((b[3] & 0x60) >> 4);
	dev->driver_bits[1] = b[3] & 0x0f;
	dev->driver_bits[2] = b[4];
	return 0;
}

int8_t mcp4724_wr(struct i2c_device *dev, uint8_t channel, int32_t v)
{
	dev->driver_bits[0] &= ~0x20;
	dev->driver_bits[1] = v >> 4;
	dev->driver_bits[2] = v << 4;
	return i2c_write(dev, dev->driver_bits, 3);
}

int8_t mcp4724_wr_ee(struct i2c_device *dev)
{
	dev->driver_bits[0] |= 0x20;
	return i2c_write(dev, dev->driver_bits, 3);
}

int32_t mcp4725_rd(struct i2c_device *dev, uint8_t channel)
{
	uint8_t b[3];
	IF_R(i2c_read(dev, b, 3), -1);
	return ((uint16_t)b[1] << 4) | ((uint16_t)b[2] >> 4);
}

float mcp4725_rd_pdr(struct i2c_device *dev)
{
	uint8_t v;
	IF_R(i2c_read(dev, &v, 1), -1);
	switch (v & 0x06) {
	case 0x00:
		return NAN;
	case 0x02:
		return 1e3;
	case 0x04:
		return 1e6;
	case 0x06:
		return 5e6;
	}
	return -1;
}

int32_t mcp4725_rd_ee(struct i2c_device *dev)
{
	uint8_t b[5];
	IF_R(i2c_read(dev, b, 5), -1);
	return (((uint16_t)b[3] & 0x0f) << 8) | (uint16_t)b[4];
}

float mcp4725_rd_ee_pdr(struct i2c_device *dev)
{
	uint8_t b[5];
	IF_R(i2c_read(dev, b, 5), -1);
	switch (b[3] & 0x60) {
	case 0x00:
		return NAN;
	case 0x20:
		return 1e3;
	case 0x40:
		return 1e6;
	case 0x60:
		return 5e6;
	}
	return -1;
}

/* tool related functions */
#ifdef COMPILE_TOOL_I2C

void tool_i2c_mcp4725_help(void)
{
	printf(
	    "Commands:\n"
	    "  rd                   Read chip contents\n"
	    "  wr <value>           Write new DAC output value (0-4095 or 0x0-0xfff)\n"
	    "  ee <value>           Write new EEPROM and DAC output value (0-4095 or 0x0-0xfff)\n"
	);
}

int tool_i2c_mcp4725_exec(struct i2c_master *master, uint8_t address, char *command, int argc, char *argv[])
{
	int err = -1;
	struct i2c_device dev;
	uint16_t value = 0;

	/* check command */
	if (strcmp(command, "rd") && strcmp(command, "wr") && strcmp(command, "ee")) {
		fprintf(stderr, "Invalid command.\n");
		return -1;
	}
	if (strcmp(command, "wr") == 0 || strcmp(command, "ee") == 0) {
		if (argc != 2) {
			fprintf(stderr, "Write command needs a value.\n");
			return -1;
		}
		char *p;
		value = strtoul(argv[1], &p, 0);
		if (argv[1] == p || value > 0xfff || value < 0) {
			fprintf(stderr, "Invalid value for write command\n");
			return -1;
		}
	}

	/* open chip */
	if (mcp4725_open(&dev, master, address)) {
		fprintf(stderr, "Chip not found, address: 0x%02x, reason: %s\n", address, error_last);
		return -1;
	}

	/* if do write */
	if (strcmp(command, "wr") == 0) {
		mcp4724_wr(&dev, 0, value);
	} else if (strcmp(command, "ee") == 0) {
		mcp4724_wr(&dev, 0, value);
		mcp4724_wr_ee(&dev);
	} else {
		/* do read */
		int32_t v = mcp4725_rd(&dev, 0);
		float pdr = mcp4725_rd_pdr(&dev);
		int32_t vee = mcp4725_rd_ee(&dev);
		float pdr_ee = mcp4725_rd_ee_pdr(&dev);
		printf("DAC value: 0x%04x, %4d, power down mode resistance: %f\n", v, v, pdr);
		printf("EEPROM:    0x%04x, %4d, power down mode resistance: %f\n", vee, vee, pdr_ee);
	}

	return err;
}

#endif

#endif
