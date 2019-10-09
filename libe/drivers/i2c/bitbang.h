/*
 * I2C bitbang drivers
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_BITBANG_H_
#define _LIBE_BITBANG_H_

#ifdef USE_I2C_BITBANG

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct i2c_master {
	uint8_t scl;
	uint8_t sda;
#ifdef TARGET_LINUX
	uint32_t frequency;
#endif
};
struct i2c_device {
	struct i2c_master *master;
	uint8_t address;
	uint8_t driver_bits[4];
};

#ifdef TARGET_LINUX
#define I2C_DELAY()             os_sleepf(1 / dev->master->frequency)
#else
#define I2C_DELAY()             os_delay_us(5)
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
		os_gpio_set(dev->master->sda, state ? 1 : 0); \
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


#ifdef __cplusplus
}
#endif

#endif /* USE_I2C_BITBANG */

#endif /* _LIBE_BITBANG_H_ */
