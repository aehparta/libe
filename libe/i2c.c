/*
 * libe cross-platform library: I2C common routines
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include "i2c.h"
#include "log.h"
#include "os.h"


uint16_t i2c_read_u16(struct i2c_context *i2c, int nack)
{
	uint16_t value = (uint16_t)i2c_read_byte(i2c, 0);
	value |= (uint16_t)i2c_read_byte(i2c, nack) << 8;
	return value;
}

uint32_t i2c_read_u32(struct i2c_context *i2c, int nack)
{
	uint32_t value = (uint32_t)i2c_read_byte(i2c, 0);
	value |= (uint32_t)i2c_read_byte(i2c, 0) << 8;
	value |= (uint32_t)i2c_read_byte(i2c, 0) << 16;
	value |= (uint32_t)i2c_read_byte(i2c, nack) << 24;
	return value;
}

uint8_t i2c_read_reg_byte(struct i2c_context *i2c, uint8_t address, uint8_t reg)
{
	uint8_t value;

	i2c_addr7(i2c, address, 0);
	i2c_write_byte(i2c, reg);
	i2c_stop(i2c);

	i2c_addr7(i2c, address, 1);
	value = i2c_read_byte(i2c, 1);
	i2c_stop(i2c);

	return value;
}

void i2c_write_reg_byte(struct i2c_context *i2c, uint8_t address, uint8_t reg, uint8_t value)
{
	i2c_addr7(i2c, address, 0);
	i2c_write_byte(i2c, reg);
	i2c_write_byte(i2c, value);
	i2c_stop(i2c);
}

void i2c_test_dump(void)
{
	DEBUG_MSG("i2c test:");
	for (uint8_t i = 0; i < 128; i++) {
		if (!i2c_addr7(NULL, i, 0)) {
			DEBUG_MSG("device found at address %02x", i);
		}
		i2c_stop(NULL);
		os_sleepf(0.001);
	}
}
