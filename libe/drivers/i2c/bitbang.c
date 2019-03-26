/*
 * libe cross-platform library: I2C bitbang drivers
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include <libe/os.h>
#include <libe/log.h>
#include <libe/i2c.h>


#ifdef TARGET_AVR
#define I2C_DELAY()         _delay_us(5)
#else
#define I2C_DELAY(delay)         os_sleepf(1 / dev->master->frequency)
#endif

#define I2C_START() \
	do { \
		os_gpio_output(dev->master->sda); \
		os_gpio_high(dev->master->scl); \
		os_gpio_low(dev->master->sda); \
		I2C_DELAY(); \
		os_gpio_low(dev->master->scl); \
	} while (0)

#define I2C_STOP() \
	do { \
		os_gpio_output(dev->master->sda); \
		os_gpio_low(dev->master->sda); \
		I2C_DELAY(); \
		os_gpio_high(dev->master->scl); \
		I2C_DELAY(); \
		os_gpio_input(dev->master->sda); \
	} while (0)

#define I2C_WRITE(state) \
	do { \
		os_gpio_set(dev->master->sda, state ? true : false); \
		I2C_DELAY(); \
		os_gpio_high(dev->master->scl); \
		I2C_DELAY(); \
		os_gpio_low(dev->master->scl); \
	} while (0)

#define I2C_READ(var, mask) \
	do { \
		I2C_DELAY(); \
		os_gpio_high(dev->master->scl); \
		I2C_DELAY(); \
		if (!os_gpio_read(dev->master->sda)) { \
			(var) &= mask; \
		} \
		os_gpio_low(dev->master->scl); \
	} while (0)

#define I2C_READ_ACK() \
	do { \
		os_gpio_input(dev->master->sda); \
		I2C_DELAY(); \
		os_gpio_high(dev->master->scl); \
		I2C_DELAY(); \
		if (os_gpio_read(dev->master->sda)) { \
			/* no ack received */ \
			I2C_STOP(); \
			return -1; \
		} \
		os_gpio_low(dev->master->scl); \
	} while (0)


int i2c_master_open(struct i2c_master *master, void *context, uint32_t frequency, uint8_t scl, uint8_t sda)
{
	/* clock is always output */
	os_gpio_output(scl);
	/* data is input as default */
	os_gpio_input(sda);

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


// int i2c_addr7(struct i2c_context *i2c, uint8_t addr, uint8_t rw)
// {
// 	I2C_SDA_OUTPUT();

// 	/* start bit */
// 	I2C_SCL_HIGH();
// 	I2C_SDA_LOW();
// 	I2C_DELAY();
// 	I2C_SCL_LOW();
// 	I2C_DELAY();

// 	/* write address, MSB first */
// 	I2C_WRITE(addr, 6);
// 	I2C_WRITE(addr, 5);
// 	I2C_WRITE(addr, 4);
// 	I2C_WRITE(addr, 3);
// 	I2C_WRITE(addr, 2);
// 	I2C_WRITE(addr, 1);
// 	I2C_WRITE(addr, 0);

// 	/* read or write (zero is write) */
// 	I2C_WRITE_BIT(rw);

// 	/* read ack */
// 	I2C_SDA_INPUT();
// 	I2C_SDA_HIGH();
// 	if (I2C_READ_BIT()) {
// 		// DEBUG_MSG("i2c nack received after address (can be a normal condition, device not ready etc)\n");
// 		return -1;
// 	}

// 	return 0;
// }

// uint8_t i2c_read_byte(struct i2c_context *i2c, int nack)
// {
// 	uint8_t byte = 0;

// 	I2C_SDA_INPUT();
// 	I2C_SDA_HIGH();

// 	I2C_READ(byte, 7);
// 	I2C_READ(byte, 6);
// 	I2C_READ(byte, 5);
// 	I2C_READ(byte, 4);
// 	I2C_READ(byte, 3);
// 	I2C_READ(byte, 2);
// 	I2C_READ(byte, 1);
// 	I2C_READ(byte, 0);

// 	/* write ack/nack */
// 	I2C_SDA_OUTPUT();
// 	I2C_WRITE_BIT(nack);

// 	I2C_SDA_INPUT();
// 	I2C_SDA_HIGH();

// 	return byte;
// }

// int i2c_write_byte(struct i2c_context *i2c, uint8_t byte)
// {
// 	I2C_SDA_OUTPUT();

// 	I2C_WRITE(byte, 7);
// 	I2C_WRITE(byte, 6);
// 	I2C_WRITE(byte, 5);
// 	I2C_WRITE(byte, 4);
// 	I2C_WRITE(byte, 3);
// 	I2C_WRITE(byte, 2);
// 	I2C_WRITE(byte, 1);
// 	I2C_WRITE(byte, 0);

// 	/* read ack */
// 	I2C_SDA_INPUT();
// 	I2C_SDA_HIGH();
// 	if (I2C_READ_BIT()) {
// 		DEBUG_MSG("i2c missing ack from slave after write byte\n");
// 		return -1;
// 	}

// 	return 0;
// }

// void i2c_stop(struct i2c_context *i2c)
// {
// 	I2C_SDA_OUTPUT();
// 	I2C_SDA_LOW();

// 	I2C_SCL_LOW();
// 	I2C_DELAY();
// 	I2C_SCL_HIGH();
// 	I2C_DELAY();

// 	I2C_SDA_INPUT();
// 	I2C_SDA_HIGH();
// 	_delay_us(10);
// }
