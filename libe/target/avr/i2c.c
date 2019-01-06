/*
 * libe cross-platform library: I2C bitbang drivers
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include <libe/os.h>
#include <libe/debug.h>
#include <libe/i2c.h>


struct i2c_context *i2c_open(void *context)
{
	/* clock high and as output, we are teh master */
	I2C_SCL_HIGH();
	I2C_SCL_OUTPUT();
	/* data high and as input */
	I2C_SDA_HIGH();
	I2C_SDA_INPUT();
	/* this delay was in the old code for reasons unknown */
	_delay_ms(20);
	return (struct i2c_context *)1;
}

int i2c_addr7(struct i2c_context *i2c, uint8_t addr, uint8_t rw)
{
	I2C_SDA_OUTPUT();

	/* start bit */
	I2C_SCL_HIGH();
	I2C_SDA_LOW();
	I2C_DELAY();
	I2C_SCL_LOW();
	I2C_DELAY();

	/* write address, MSB first */
	I2C_WRITE(addr, 6);
	I2C_WRITE(addr, 5);
	I2C_WRITE(addr, 4);
	I2C_WRITE(addr, 3);
	I2C_WRITE(addr, 2);
	I2C_WRITE(addr, 1);
	I2C_WRITE(addr, 0);

	/* read or write (zero is write) */
	I2C_WRITE_BIT(rw);

	/* read ack */
	I2C_SDA_INPUT();
	I2C_SDA_HIGH();
	if (I2C_READ_BIT()) {
		// DEBUG_MSG("i2c nack received after address (can be a normal condition, device not ready etc)\n");
		return -1;
	}

	return 0;
}

uint8_t i2c_read_byte(struct i2c_context *i2c, int nack)
{
	uint8_t byte = 0;

	I2C_SDA_INPUT();
	I2C_SDA_HIGH();

	I2C_READ(byte, 7);
	I2C_READ(byte, 6);
	I2C_READ(byte, 5);
	I2C_READ(byte, 4);
	I2C_READ(byte, 3);
	I2C_READ(byte, 2);
	I2C_READ(byte, 1);
	I2C_READ(byte, 0);

	/* write ack/nack */
	I2C_SDA_OUTPUT();
	I2C_WRITE_BIT(nack);

	I2C_SDA_INPUT();
	I2C_SDA_HIGH();

	return byte;
}

int i2c_write_byte(struct i2c_context *i2c, uint8_t byte)
{
	I2C_SDA_OUTPUT();

	I2C_WRITE(byte, 7);
	I2C_WRITE(byte, 6);
	I2C_WRITE(byte, 5);
	I2C_WRITE(byte, 4);
	I2C_WRITE(byte, 3);
	I2C_WRITE(byte, 2);
	I2C_WRITE(byte, 1);
	I2C_WRITE(byte, 0);

	/* read ack */
	I2C_SDA_INPUT();
	I2C_SDA_HIGH();
	if (I2C_READ_BIT()) {
		DEBUG_MSG("i2c missing ack from slave after write byte\n");
		return -1;
	}

	return 0;
}

void i2c_stop(struct i2c_context *i2c)
{
	I2C_SDA_OUTPUT();
	I2C_SDA_LOW();

	I2C_SCL_LOW();
	I2C_DELAY();
	I2C_SCL_HIGH();
	I2C_DELAY();

	I2C_SDA_INPUT();
	I2C_SDA_HIGH();
	_delay_us(10);
}
