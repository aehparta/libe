/*
 * Microchip MCP3221 12-bit ADC
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifdef USE_I2C

#include <libe/libe.h>


int32_t mcp3221_rd(struct i2c_device *dev, uint8_t channel)
{
	uint8_t data[2];
	IF_R(i2c_read(dev, &data, 2), -1);
	return (data[0] << 8) | data[1];
}

/* tool related functions */
#ifdef COMPILE_TOOL_I2C

void tool_i2c_mcp3221_help(void)
{
	printf("MCP3221\n");
}

int tool_i2c_mcp3221_exec(struct i2c_master *master, uint8_t address, char *command, int argc, char *argv[])
{
	int err = 0;
	printf("exec command %s\n", command);
	return err;
}

#endif

#endif
