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

#if defined(TARGET_LINUX) || defined(TARGET_ESP32) || defined(TARGET_PIC32)
/* on linux and other more powerfull platforms allow bitbang frequency to be defined */
#define I2C_DELAY()             os_sleepf(1 / dev->master->frequency)
#elif defined(TARGET_AVR)
#define I2C_DELAY()             _delay_loop_1(F_CPU / 200000 / 3)
#else
/* on other less powerfull platforms lock i2c clock to approximately 100 kHz */
#define I2C_DELAY()             os_delay_ms(1)
#endif

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


#ifdef __cplusplus
}
#endif

#endif /* USE_I2C_BITBANG */

#endif /* _LIBE_BITBANG_H_ */
