/*
 * libe cross-platform library: I2C bitbang drivers
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_I2C_H_
#define _LIBE_I2C_H_

#include <stdint.h>
#ifdef TARGET_LINUX
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#endif
#include <libe/os.h>


#ifdef USE_I2C_BITBANG
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
};
#elif TARGET_LINUX
struct i2c_master {
	int fd;
};
struct i2c_device {
	struct i2c_master *master;
	uint8_t address;
};
#endif

/*
 * I2C functions
 */

int i2c_master_open(struct i2c_master *master, void *context, uint32_t frequency, uint8_t scl, uint8_t sda);
void i2c_master_close(struct i2c_master *master);

int i2c_open(struct i2c_device *dev, struct i2c_master *master, uint8_t address);
void i2c_close(struct i2c_device *dev);

int i2c_read(struct i2c_device *i2c, void *data, size_t size);
int i2c_write(struct i2c_device *i2c, void *data, size_t size);


#endif /* _LIBE_I2C_H_ */

