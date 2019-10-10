/*
 * I2C bitbang drivers
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_BITBANG_H_
#define _LIBE_BITBANG_H_

#ifdef USE_I2C_BITBANG

#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef I2C_BITBANG_SCL
#error "I2C_BITBANG_SCL pin definition missing, it must be set when using I2C bitbang"
#endif
#ifndef I2C_BITBANG_SDA
#error "I2C_BITBANG_SDA pin definition missing, it must be set when using I2C bitbang"
#endif

#ifdef TARGET_LINUX
#define I2C_DELAY()             os_sleepf(1 / dev->master->frequency)
#else
#define I2C_DELAY()             os_delay_us(5)
#endif

#define I2C_START() \
	do { \
		os_gpio_output(I2C_BITBANG_SDA); \
		os_gpio_high(I2C_BITBANG_SCL); \
		os_gpio_low(I2C_BITBANG_SDA); \
		I2C_DELAY(); \
		os_gpio_low(I2C_BITBANG_SCL); \
	} while (0)

#define I2C_STOP() \
	do { \
		os_gpio_output(I2C_BITBANG_SDA); \
		os_gpio_low(I2C_BITBANG_SDA); \
		I2C_DELAY(); \
		os_gpio_high(I2C_BITBANG_SCL); \
		I2C_DELAY(); \
		os_gpio_input(I2C_BITBANG_SDA); \
	} while (0)

#define I2C_WRITE(state) \
	do { \
		os_gpio_set(I2C_BITBANG_SDA, state ? 1 : 0); \
		I2C_DELAY(); \
		os_gpio_high(I2C_BITBANG_SCL); \
		I2C_DELAY(); \
		os_gpio_low(I2C_BITBANG_SCL); \
	} while (0)

#define I2C_READ(var, mask) \
	do { \
		I2C_DELAY(); \
		os_gpio_high(I2C_BITBANG_SCL); \
		I2C_DELAY(); \
		if (!os_gpio_read(I2C_BITBANG_SDA)) { \
			(var) &= mask; \
		} \
		os_gpio_low(I2C_BITBANG_SCL); \
	} while (0)

#define I2C_READ_ACK() \
	do { \
		os_gpio_input(I2C_BITBANG_SDA); \
		I2C_DELAY(); \
		os_gpio_high(I2C_BITBANG_SCL); \
		I2C_DELAY(); \
		if (os_gpio_read(I2C_BITBANG_SDA)) { \
			/* no ack received */ \
			I2C_STOP(); \
			return -1; \
		} \
		os_gpio_low(I2C_BITBANG_SCL); \
	} while (0)


#ifdef __cplusplus
}
#endif

#endif /* USE_I2C_BITBANG */

#endif /* _LIBE_BITBANG_H_ */
