/*
 * Qorvo ACT4751 Programmable DC-DC converter driver
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifdef USE_I2C

#include "act4751.h"

int8_t act4751_open(struct i2c_device *dev, struct i2c_master *master, uint8_t address, uint16_t rilim)
{
	uint16_t *r = (uint16_t *)dev->driver_bits;
	*r = rilim;
	return i2c_open(dev, master, address);
}

int8_t act4751_set_main_voltage(struct i2c_device *dev, float voltage)
{
	uint8_t d[3];
	uint16_t vbits = (uint16_t)(voltage * 80);
	if (voltage > 24.0) {
		vbits = 0x7ff;
	}
	/* if voltage is zero, just disable main buck */
	// if (voltage <= 0.0) {
	// 	i2c_write_reg_byte(dev, 0x05, 0x00);
	// 	return 0.0;
	// }
	/* write first and second register simultaneously */
	d[0] = 0x0a;
	d[1] = (uint8_t)(vbits >> 3);
	d[2] = (i2c_read_reg_byte(dev, 0x0b) & 0xf8) | (uint8_t)(vbits & 0x7);
	i2c_write(dev, d, 3);
	/* apply */
	i2c_write_reg_byte(dev, 0x05, 0x03);
	return 0;
}

float act4751_get_main_voltage(struct i2c_device *dev)
{
	uint8_t d[2];
	/* select register */
	d[0] = 0x0a;
	IF_R(i2c_write(dev, d, 1), NAN);
	/* read values */
	IF_R(i2c_read(dev, d, 2), NAN);
	/* convert and return */
	return (float)(((uint16_t)d[1] & 0x7) | ((uint16_t)d[0] << 3)) / 80.0;
}

int8_t act4751_set_main_current(struct i2c_device *dev, float current)
{
	/* calculate register value */
	uint16_t *r = (uint16_t *)dev->driver_bits;
	float max = 0.000000391 * 256.0 * (float)(*r) * 2.4976023;
	uint16_t v = (uint8_t)(current / max * 256.0);
	if (v > 0) {
		v -= 1;
	}
	if (v > 0xff) {
		v = 0xff;
	}
	/* write register */
	i2c_write_reg_byte(dev, 0x0c, (uint8_t)v);
	return 0;
}

float act4751_get_main_current(struct i2c_device *dev)
{
	uint16_t *r = (uint16_t *)dev->driver_bits;
	float v = i2c_read_reg_byte(dev, 0x0c);
	return (0.000000391 * 256.0 * (float)(*r) * 2.4976023) * ((v + 1.0) / 256.0);
}

int8_t act4751_set_ldo_voltage(struct i2c_device *dev, float voltage)
{
	uint8_t cfg2 = i2c_read_reg_byte(dev, 0x09);
	/* if voltage is zero, disable ldo */
	if (voltage <= 0.0) {
		i2c_write_reg_byte(dev, 0x09, cfg2 & ~0x01);
		return 0.0;
	}
	/* calculate register value */
	uint8_t v = (uint8_t)((voltage - 0.8999) / (4.05 - 0.8999) * 63.0);
	if (v > 0x3f) {
		v = 0x3f;
	}
	/* write register value */
	i2c_write_reg_byte(dev, 0x0d, v);
	/* enable ldo if disabled */
	if (!(cfg2 & 0x01)) {
		i2c_write_reg_byte(dev, 0x09, cfg2 | 0x01);
	}
	return 0;
}

float act4751_get_ldo_voltage(struct i2c_device *dev)
{
	float v = (float)i2c_read_reg_byte(dev, 0x0d);
	return v / 63.0 * (4.05 - 0.8999) + 0.8999;
}

int8_t act4751_set_mini_buck(struct i2c_device *dev, bool enable)
{
	uint8_t cfg2 = i2c_read_reg_byte(dev, 0x09) & ~0x02;
	i2c_write_reg_byte(dev, 0x09, cfg2 | (enable ? 0x02 : 0x00));
	return 0;
}

int8_t act4751_get_mini_buck(struct i2c_device *dev)
{
	uint8_t d;
	error_if(i2c_write_byte(dev, 0x09), -1, "act4751 not responding");
	error_if(i2c_read(dev, &d, 1), -1, "act4751 read failed");
	return d & 0x02 ? 1 : 0;
}

#endif
