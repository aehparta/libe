/*
 * Qorvo ACT4751 Programmable DC-DC converter driver
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifdef USE_I2C

#include "act4751.h"

int8_t act4751_set_main_voltage(struct i2c_device *dev, float voltage)
{
	uint8_t d[3];
	uint16_t vbits = (uint16_t)(voltage * 80);

	/* write first and second register simultaneously */
	d[0] = 0x0a;
	d[1] = (uint8_t)(vbits >> 3);
	d[2] = (i2c_read_reg_byte(dev, 0x0b) & 0xf8) | (uint8_t)(vbits & 0x7);

	return i2c_write(dev, d, 3);
}

float act4751_get_main_voltage(struct i2c_device *dev)
{
	uint8_t d[2];

	/* select register */
	d[0] = 0x0a;
	IF_R(i2c_write(dev, d, 1), -1);

	/* read values */
	IF_R(i2c_read(dev, d, 2), -1);

	/* convert and return */
	return (float)(((uint16_t)d[1] & 0x7) | ((uint16_t)d[0] << 3)) / 80.0;
}

int8_t act4751_set_main_current(struct i2c_device *dev, float current)
{
	return 0;
}

float act4751_get_main_current(struct i2c_device *dev)
{
	return 0;
}

#endif
