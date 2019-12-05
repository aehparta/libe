/*
 * I2C bitbang drivers
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_I2C_BITBANG_H_
#define _LIBE_I2C_BITBANG_H_

#ifdef USE_I2C_BITBANG

#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_I2C_BITBANG_DYNAMIC

#define I2C_BITBANG_SCL (master->scl)
#define I2C_BITBANG_SDA (master->sda)
struct i2c_master {
	uint8_t scl;
	uint8_t sda;
	uint32_t frequency;
};
struct i2c_device {
	struct i2c_master *master;
	uint8_t address;
	uint8_t driver_bits[4];
};

#else

struct i2c_master {
};
struct i2c_device {
	struct i2c_master *master;
	uint8_t address;
	uint8_t driver_bits[4];
};

#endif


#ifdef __cplusplus
}
#endif

#endif /* USE_I2C_BITBANG */

#endif /* _LIBE_I2C_BITBANG_H_ */
