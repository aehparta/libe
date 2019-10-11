/*
 * pic8 I2C drivers
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#if defined(USE_I2C) && !defined(USE_I2C_BITBANG)

#include <libe/libe.h>

int i2c_master_open(struct i2c_master *master, void *context, uint32_t frequency, uint8_t scl, uint8_t sda)
{
	SSPCON = 0x28;
	SSPCON2 = 0x00;
	SSPADD = F_CPU / (4 * frequency) - 1;
	SSPSTAT = 0;

	gpio_input(16);
	gpio_input(17);

	return 0;
}

void i2c_master_close(struct i2c_master *master)
{
}

int i2c_open(struct i2c_device *dev, struct i2c_master *master, uint8_t address)
{
	dev->master = master;
	dev->address = address << 1;
	return i2c_write(dev, NULL, 0);
}

void i2c_close(struct i2c_device *dev)
{

}

int i2c_read(struct i2c_device *dev, void *data, size_t size)
{
	return 0;
}

int i2c_write(struct i2c_device *dev, void *data, size_t size)
{
	SEN = 1;
	SSPBUF = dev->address;
	PEN = 1;
	return 0;
}

#endif
