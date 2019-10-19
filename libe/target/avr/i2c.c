/*
 * I2C AVR drivers
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#if defined(USE_I2C) && !defined(USE_I2C_BITBANG)

#include <libe/libe.h>

#define TWCR_BASE ((1 << TWINT) | (1 << TWEA) | (1 << TWEN))


int8_t i2c_master_open(struct i2c_master *master, void *context, uint32_t frequency, uint8_t scl, uint8_t sda)
{
	/* default to 100 kHz */
	if (frequency < 1) {
		frequency = 100000;
	}

	/* check that F_CPU isn't too small so that negatives values would appear */
	float f_div = (F_CPU / frequency - 16);
	f_div = f_div < 0 ? 0 : f_div;
	/* bit rate register value */
	TWBR = f_div / 2; /* using prescaler 1 from TWSR */
	TWSR = 0x00;

	/* reset bus */
	TWCR = TWCR_BASE | (1 << TWSTA);
	while (!(TWCR & (1 << TWINT)));
	TWCR = TWCR_BASE | (1 << TWSTO);

	return 0;
}

void i2c_master_close(struct i2c_master *master)
{
	TWCR = 0x00;
}

int8_t i2c_open(struct i2c_device *dev, struct i2c_master *master, uint8_t address)
{
	dev->master = master;
	dev->address = address << 1;
	return i2c_write(dev, NULL, 0);
}

void i2c_close(struct i2c_device *dev)
{

}

int8_t i2c_read(struct i2c_device *dev, void *data, uint8_t size)
{
	int err = -1;

	/* send start and assume it was sent ok */
	TWCR = TWCR_BASE | (1 << TWSTA);
	while (!(TWCR & (1 << TWINT)));

	/* send address + read (one) */
	TWDR = dev->address | 1;
	TWCR = TWCR_BASE;
	/* wait for transmission and check response */
	while (!(TWCR & (1 << TWINT)));
	if ((TWSR & 0xf8) != 0x40) {
		error_set_last("no ack received");
		goto out_err;
	}

out_err:
	return err;
}

int8_t i2c_write(struct i2c_device *dev, void *data, uint8_t size)
{
	int err = -1;

	/* send start and assume it was sent ok */
	TWCR = TWCR_BASE | (1 << TWSTA);
	while (!(TWCR & (1 << TWINT)));

	/* send address + write (zero) */
	TWDR = dev->address;
	TWCR = TWCR_BASE;
	/* wait for transmission and check response */
	while (!(TWCR & (1 << TWINT)));
	if ((TWSR & 0xf8) != 0x18) {
		error_set_last("no ack received");
		goto out_err;
	}

	/* send data */
	for (uint8_t *p = data; size > 0; size--, p++) {
		TWDR = *p;
		TWCR = TWCR_BASE;
		/* wait for transmission and check response */
		while (!(TWCR & (1 << TWINT)));
		if ((TWSR & 0xf8) != 0x28) {
			error_set_last("no ack received after sending data");
			goto out_err;
		}
	}

	err = 0;

out_err:
	/* always send stop */
	TWCR = TWCR_BASE | (1 << TWSTO);
	return err;
}

#endif
