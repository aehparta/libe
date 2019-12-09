/*
 * I2C bitbang drivers
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifdef USE_I2C_BITBANG

#include <libe/libe.h>

#ifndef USE_I2C_BITBANG_DYNAMIC
#ifndef I2C_BITBANG_SCL
#error "I2C_BITBANG_SCL pin definition missing, it must be set when using I2C bitbang"
#endif
#ifndef I2C_BITBANG_SDA
#error "I2C_BITBANG_SDA pin definition missing, it must be set when using I2C bitbang"
#endif
#endif

#if !defined(I2C_BITBANG_DELAY_FUNC)
#ifdef USE_I2C_BITBANG_DYNAMIC
#define I2C_BITBANG_DELAY_FUNC              os_sleepf(1 / master->frequency / 2)
#elif defined(TARGET_AVR)
/* on avr lock i2c clock to approximately 100 kHz using the better _delay_loop() instead of _delay_us() */
#define I2C_BITBANG_DELAY_FUNC              _delay_loop_1(F_CPU / 200000 / 3)
#else
/* on other less powerfull platforms lock i2c clock to approximately 100 kHz */
#define I2C_BITBANG_DELAY_FUNC              os_delay_us(4)
#endif
#endif

#define I2C_START() \
	do { \
		gpio_output(I2C_BITBANG_SDA); \
		gpio_high(I2C_BITBANG_SCL); \
		gpio_low(I2C_BITBANG_SDA); \
		I2C_BITBANG_DELAY_FUNC; \
		gpio_low(I2C_BITBANG_SCL); \
	} while (0)

#define I2C_STOP() \
	do { \
		gpio_output(I2C_BITBANG_SDA); \
		gpio_low(I2C_BITBANG_SDA); \
		I2C_BITBANG_DELAY_FUNC; \
		gpio_high(I2C_BITBANG_SCL); \
		I2C_BITBANG_DELAY_FUNC; \
		gpio_input(I2C_BITBANG_SDA); \
	} while (0)

#define I2C_WRITE(state) \
	do { \
		gpio_set(I2C_BITBANG_SDA, (state)); \
		I2C_BITBANG_DELAY_FUNC; \
		gpio_high(I2C_BITBANG_SCL); \
		I2C_BITBANG_DELAY_FUNC; \
		gpio_low(I2C_BITBANG_SCL); \
	} while (0)

#define I2C_READ(var, mask) \
	do { \
		I2C_BITBANG_DELAY_FUNC; \
		gpio_high(I2C_BITBANG_SCL); \
		I2C_BITBANG_DELAY_FUNC; \
		if (!gpio_read(I2C_BITBANG_SDA)) { \
			(var) &= mask; \
		} \
		gpio_low(I2C_BITBANG_SCL); \
	} while (0)

#define I2C_READ_ACK() \
	do { \
		gpio_input(I2C_BITBANG_SDA); \
		I2C_BITBANG_DELAY_FUNC; \
		gpio_high(I2C_BITBANG_SCL); \
		I2C_BITBANG_DELAY_FUNC; \
		if (gpio_read(I2C_BITBANG_SDA)) { \
			/* no ack received */ \
			I2C_STOP(); \
			return -1; \
		} \
		gpio_low(I2C_BITBANG_SCL); \
	} while (0)


int8_t i2c_master_open(struct i2c_master *master, void *context, uint32_t frequency, uint8_t scl, uint8_t sda)
{
	/* save information */
#ifdef USE_I2C_BITBANG_DYNAMIC
	master->scl = scl;
	master->sda = sda;
	master->frequency = frequency > 0 ? frequency : 100000;
#endif

	/* clock is always output */
	gpio_high(I2C_BITBANG_SCL);
	gpio_open_drain(I2C_BITBANG_SCL, true);
	gpio_output(I2C_BITBANG_SCL);
	/* data is input as default, but output at start */
	gpio_high(I2C_BITBANG_SDA);
	gpio_open_drain(I2C_BITBANG_SDA, true);
	gpio_output(I2C_BITBANG_SDA);

	/* reset the bus by opening non-existing device */
	struct i2c_device dev;
	i2c_open(&dev, master, 0xff);
	i2c_close(&dev);

	return 0;
}

void i2c_master_close(struct i2c_master *master)
{
	gpio_input(I2C_BITBANG_SCL);
	gpio_input(I2C_BITBANG_SDA);
}

int8_t i2c_open(struct i2c_device *dev, struct i2c_master *master, uint8_t address)
{
	dev->master = master;
	dev->address = address;
	/* do a write without any data to detect if there is a device at this address */
	return i2c_write(dev, NULL, 0);
}

void i2c_close(struct i2c_device *dev)
{

}

int8_t i2c_read(struct i2c_device *dev, void *data, uint8_t size)
{
#ifdef USE_I2C_BITBANG_DYNAMIC
	struct i2c_master *master = dev->master;
#endif

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

int8_t i2c_write(struct i2c_device *dev, void *data, uint8_t size)
{
#ifdef USE_I2C_BITBANG_DYNAMIC
	struct i2c_master *master = dev->master;
#endif
	
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
