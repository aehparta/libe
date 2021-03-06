/*
 * Fairchild FAN5702 LED driver
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifdef USE_I2C

#include "fan5702.h"


int fan5702_open(struct i2c_device *dev, struct i2c_master *master)
{
	/* try to detect fan5702 */
	return i2c_open(dev, master, FAN5702_ADDR);
}

int fan5702_cfg(struct i2c_device *dev)
{
	/* drive current to 20 mA and EN pin as enable */
	IF_R(i2c_write_reg_byte(dev, 0x10, 0x18), -1);
	IF_R(i2c_write_reg_byte(dev, 0x20, 0x00), -1);
	return 0;
}

int fan5702_set(struct i2c_device *dev, uint8_t channel, uint8_t brightness)
{
	switch (channel) {
	case 3:
	case 4:
	case 5:
	case 6:
		channel = channel << 4;
		break;
	default:
		error_set_last("invalid channel");
		return -1;
	}
	return i2c_write_reg_byte(dev, channel, brightness);
}

/* tool related functions */
#ifdef COMPILE_TOOL_I2C

void tool_i2c_fan5702_help(void)
{
	printf(
	    "Commands:\n"
	    "  <CHANNEL>=<BRIGHTNESS> [...]     Set given channels to given brightess each\n"
	    "Multiple channels can be defined.\n"
	    "\n"
	    "Accepted values:\n"
	    "  CHANNEL      1-6\n"
	    "  BRIGHTNESS   0-63\n"
	);
}

int tool_i2c_fan5702_exec(struct i2c_master *master, uint8_t address, char *command, int argc, char *argv[])
{
	int err = -1;
	struct i2c_device dev;

	/* check commands */
	for (int i = 0; i < argc; i++) {
		if (strlen(argv[i]) < 3) {
			fprintf(stderr, "Invalid command: %s\n", argv[i]);
			return -1;
		} else if (argv[i][1] != '=') {
			fprintf(stderr, "Invalid command: %s\n", argv[i]);
			return -1;
		} else if (argv[i][0] < '1' || argv[i][0] > '6') {
			fprintf(stderr, "Invalid channel: %s\n", argv[i]);
			return -1;
		} else if (atoi(argv[i] + 2) < 0 || atoi(argv[i] + 2) > 63) {
			fprintf(stderr, "Invalid value: %s\n", argv[i]);
			return -1;
		}
	}

	/* open chip */
	if (fan5702_open(&dev, master)) {
		fprintf(stderr, "Chip not found, reason: %s\n", error_last);
		return -1;
	} else if (fan5702_cfg(&dev)) {
		fprintf(stderr, "Chip configuration failed, reason: %s\n", error_last);
		return -1;
	}

	/* apply commands */
	for (int i = 0; i < argc; i++) {
		if (fan5702_set(&dev, argv[i][0] - '0', atoi(argv[i] + 2))) {
			fprintf(stderr, "Failed to set channel, reason: %s\n", error_last);
			break;
		} else {
			printf("Set channel %c brightness to %d%% (%d)\n", argv[i][0], 100 * atoi(argv[i] + 2) / 63, atoi(argv[i] + 2));
		}
	}

	fan5702_close(&dev);

	return err;
}

#endif

#endif
