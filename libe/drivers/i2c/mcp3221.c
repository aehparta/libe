/*
 * libe cross-platform library: mcp3221 i2c driver
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#include <libe/log.h>
#include "mcp3221.h"


int16_t mcp3221_read(struct i2c_device *dev)
{
	uint8_t data[2];
	IF_R(i2c_read(dev, &data, 2), -1);
	return (data[0] << 8) | data[1];
}

