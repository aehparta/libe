/*
 * I2C AVR drivers
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#if defined(USE_I2C) && !defined(USE_I2C_BITBANG)

#include <libe/libe.h>

#define TWCR_BASE ((1 << TWINT) | (1 << TWEA) | (1 << TWEN))


int i2c_master_open(struct i2c_master *master, void *context, uint32_t frequency, uint8_t scl, uint8_t sda)
{
	/* check that F_CPU isn't too small so that negatives values would appear */
	float f_div = (F_CPU / frequency - 16);
	f_div = f_div < 0 ? 0 : f_div;
	/* bit rate register value */
	TWBR = f_div / 2; /* using prescaler 1 from TWSR */
	TWSR = 0x00;
	TWCR = TWCR_BASE;

	return 0;
}

void i2c_master_close(struct i2c_master *master)
{
	TWCR = 0x00;
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
	TWCR = TWCR_BASE | (1 << TWSTA);
	while (!(TWCR & (1 << TWINT)));
	/* send address + write (zero) */
	TWDR = dev->address;
	/* clear interrupt flag and start transmission */
	TWCR = TWCR_BASE;
	while (!(TWCR & (1 << TWINT)));
	if ((TWSR & 0xf8) != 0x18) {
		error_last = "no ack received";
		TWCR = TWCR_BASE | (1 << TWSTO);
		return -1;
	}

	uint8_t *b = data;
	for (size_t i = 0; i < size; i++) {
		TWDR = *b;
		/* clear interrupt flag and start transmission */
		TWCR = TWCR_BASE;
		/* wait for transmission */
		while (!(TWCR & (1 << TWINT)));
		/* check status */
		if ((TWSR & 0xf8) != 0x28) {
			error_last = "no ack received after sending data";
			TWCR = TWCR_BASE | (1 << TWSTO);
			return -1;
		}
		b++;
	}

	TWCR = TWCR_BASE | (1 << TWSTO);

	return 0;
}

#endif
