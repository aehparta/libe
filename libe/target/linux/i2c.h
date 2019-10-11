/*
 * I2C linux drivers
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _TARGET_LINUX_I2C_H_
#define _TARGET_LINUX_I2C_H_

#ifdef __cplusplus
extern "C" {
#endif

struct i2c_master {
	int fd;
	uint32_t frequency;
};

struct i2c_device {
	struct i2c_master *master;
	uint8_t address;
	uint8_t driver_bits[4];
};

#ifdef __cplusplus
}
#endif

#endif /* _TARGET_LINUX_I2C_H_ */
