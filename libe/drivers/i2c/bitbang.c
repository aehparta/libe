/*
 * I2C bitbang drivers
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifdef USE_I2C_BITBANG

#include <libe/libe.h>


#define I2C_START() \
	do { \
		gpio_output(I2C_BITBANG_SDA); \
		gpio_high(I2C_BITBANG_SCL); \
		gpio_low(I2C_BITBANG_SDA); \
		I2C_DELAY(); \
		gpio_low(I2C_BITBANG_SCL); \
	} while (0)

#define I2C_STOP() \
	do { \
		gpio_output(I2C_BITBANG_SDA); \
		gpio_low(I2C_BITBANG_SDA); \
		I2C_DELAY(); \
		gpio_high(I2C_BITBANG_SCL); \
		I2C_DELAY(); \
		gpio_high(I2C_BITBANG_SDA); /* this sets pull-up on in avr */ \
		gpio_input(I2C_BITBANG_SDA); \
	} while (0)

#define I2C_WRITE(state) \
	do { \
		gpio_set(I2C_BITBANG_SDA, (state)); \
		I2C_DELAY(); \
		gpio_high(I2C_BITBANG_SCL); \
		I2C_DELAY(); \
		gpio_low(I2C_BITBANG_SCL); \
	} while (0)

#define I2C_READ(var, mask) \
	do { \
		I2C_DELAY(); \
		gpio_high(I2C_BITBANG_SCL); \
		I2C_DELAY(); \
		if (!gpio_read(I2C_BITBANG_SDA)) { \
			(var) &= mask; \
		} \
		gpio_low(I2C_BITBANG_SCL); \
	} while (0)

#define I2C_READ_ACK() \
	do { \
		gpio_high(I2C_BITBANG_SDA); /* this sets pull-up on in avr */ \
		gpio_input(I2C_BITBANG_SDA); \
		I2C_DELAY(); \
		gpio_high(I2C_BITBANG_SCL); \
		I2C_DELAY(); \
		if (gpio_read(I2C_BITBANG_SDA)) { \
			/* no ack received */ \
			I2C_STOP(); \
			return -1; \
		} \
		gpio_low(I2C_BITBANG_SCL); \
	} while (0)


int i2c_master_open(struct i2c_master *master, void *context, uint32_t frequency, uint8_t scl, uint8_t sda)
{
	/* clock is always output */
	gpio_high(I2C_BITBANG_SCL);
	gpio_pullup(I2C_BITBANG_SCL, true);
	gpio_open_drain(I2C_BITBANG_SCL, true);
	gpio_output(I2C_BITBANG_SCL);
	/* data is input as default, but output at start */
	gpio_high(I2C_BITBANG_SDA);
	gpio_pullup(I2C_BITBANG_SDA, true);
	gpio_open_drain(I2C_BITBANG_SDA, true);
	gpio_output(I2C_BITBANG_SDA);

	/* reset the bus by sending a start and stop */
	gpio_high(I2C_BITBANG_SCL);
	gpio_high(I2C_BITBANG_SDA);
	os_delay_us(10);
	gpio_low(I2C_BITBANG_SDA);
	os_delay_us(10);
	gpio_low(I2C_BITBANG_SCL);
	os_delay_us(10);
	gpio_high(I2C_BITBANG_SCL);
	os_delay_us(10);
	gpio_high(I2C_BITBANG_SDA);
	os_delay_us(10);
	gpio_input(I2C_BITBANG_SDA);

	/* save information */
#if defined(TARGET_LINUX) || defined(TARGET_ESP32) || defined(TARGET_PIC32)
	master->frequency = frequency;
#endif

	return 0;
}

void i2c_master_close(struct i2c_master *master)
{
	gpio_input(I2C_BITBANG_SCL);
	gpio_input(I2C_BITBANG_SDA);
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
	for (uint8_t i = 0x40; i; i = i >> 1) {
		I2C_WRITE(dev->address & i);
	}

	/* read mode bit */
	I2C_WRITE(1);

	/* read ack */
	I2C_READ_ACK();

	/* read data */
	for (uint8_t *p = data; size > 0; size--, p++) {
		gpio_high(I2C_BITBANG_SDA); /* this sets pull-up on in avr */
		gpio_input(I2C_BITBANG_SDA);
		*p = 0xff;
		for (uint8_t i = 0x80; i; i = i >> 1) {
			I2C_READ(*p, ~i);
		}
		/* send ack/nack */
		gpio_output(I2C_BITBANG_SDA);
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
	for (uint8_t i = 0x40; i; i = i >> 1) {
		I2C_WRITE(dev->address & i);
	}

	/* write mode bit */
	I2C_WRITE(0);

	/* read ack */
	I2C_READ_ACK();

	/* write data */
	for (uint8_t *p = data; size > 0; size--, p++) {
		gpio_output(I2C_BITBANG_SDA);
		for (uint8_t i = 0x80; i; i = i >> 1) {
			I2C_WRITE(*p & i);
		}
		/* read ack */
		I2C_READ_ACK();
	}

	/* stop */
	I2C_STOP();

	return 0;
}

#endif
