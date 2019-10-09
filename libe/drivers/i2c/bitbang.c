/*
 * I2C bitbang drivers
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifdef USE_I2C_BITBANG

#include <libe/libe.h>


int i2c_master_open(struct i2c_master *master, void *context, uint32_t frequency, uint8_t scl, uint8_t sda)
{
	/* clock is always output */
	os_gpio_output(scl);
	os_gpio_pullup(scl, true);
	os_gpio_open_drain(scl, true);
	/* data is input as default */
	os_gpio_input(sda);
	os_gpio_pullup(sda, true);
	os_gpio_open_drain(sda, true);

	/* reset the bus by clocking enough cycles and then doing stop */
	os_gpio_low(scl);
	for (int i = 0; i < 9; i++) {
		os_delay_us(5);
		os_gpio_high(scl);
		os_delay_us(5);
		os_gpio_low(scl);
	}
	os_delay_us(5);
	os_gpio_output(sda);
	os_gpio_low(sda);
	os_delay_us(5);
	os_gpio_high(sda);
	os_delay_us(5);
	os_gpio_high(scl);
	os_delay_us(5);
	os_gpio_input(sda);

	/* save information */
	master->scl = scl;
	master->sda = sda;
#ifdef TARGET_LINUX
	master->frequency = frequency;
#endif

	return 0;
}

void i2c_master_close(struct i2c_master *master)
{
	os_gpio_input(master->scl);
	os_gpio_input(master->sda);
}

int i2c_open(struct i2c_device *dev, struct i2c_master *master, uint8_t address)
{
	dev->master = master;
	dev->address = address;
	/* do a write without any data to detect if there is a device at this address */
	return i2c_write(dev, NULL, 0);
}

void i2c_close(struct i2c_device *dev)
{

}

int i2c_read(struct i2c_device *dev, void *data, size_t size)
{
	/* start bit */
	I2C_START();

	/* address */
	I2C_WRITE(dev->address & 0x40);
	I2C_WRITE(dev->address & 0x20);
	I2C_WRITE(dev->address & 0x10);
	I2C_WRITE(dev->address & 0x08);
	I2C_WRITE(dev->address & 0x04);
	I2C_WRITE(dev->address & 0x02);
	I2C_WRITE(dev->address & 0x01);

	/* read mode bit */
	I2C_WRITE(1);

	/* read ack */
	I2C_READ_ACK();

	/* read data */
	for (uint8_t *p = data; size > 0; size--, p++) {
		os_gpio_input(dev->master->sda);
		*p = 0xff;
		I2C_READ(*p, ~0x80);
		I2C_READ(*p, ~0x40);
		I2C_READ(*p, ~0x20);
		I2C_READ(*p, ~0x10);
		I2C_READ(*p, ~0x08);
		I2C_READ(*p, ~0x04);
		I2C_READ(*p, ~0x02);
		I2C_READ(*p, ~0x01);

		/* send ack/nack */
		os_gpio_output(dev->master->sda);
		I2C_WRITE(size > 1 ? 0 : 1);
	}

	/* stop */
	I2C_STOP();

	return 0;
}

int i2c_write(struct i2c_device *dev, void *data, size_t size)
{
	/* start bit */
	I2C_START();

	/* address */
	I2C_WRITE(dev->address & 0x40);
	I2C_WRITE(dev->address & 0x20);
	I2C_WRITE(dev->address & 0x10);
	I2C_WRITE(dev->address & 0x08);
	I2C_WRITE(dev->address & 0x04);
	I2C_WRITE(dev->address & 0x02);
	I2C_WRITE(dev->address & 0x01);

	/* write mode bit */
	I2C_WRITE(0);

	/* read ack */
	I2C_READ_ACK();

	/* write data */
	for (uint8_t *p = data; size > 0; size--, p++) {
		os_gpio_output(dev->master->sda);
		I2C_WRITE(*p & 0x80);
		I2C_WRITE(*p & 0x40);
		I2C_WRITE(*p & 0x20);
		I2C_WRITE(*p & 0x10);
		I2C_WRITE(*p & 0x08);
		I2C_WRITE(*p & 0x04);
		I2C_WRITE(*p & 0x02);
		I2C_WRITE(*p & 0x01);

		/* read ack */
		I2C_READ_ACK();
	}

	/* stop */
	I2C_STOP();

	return 0;
}

#endif
