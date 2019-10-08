/*
 * libe cross-platform library: I2C bitbang drivers
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_I2C_H_
#define _LIBE_I2C_H_
#ifdef USE_I2C

#include <stdint.h>
#ifdef TARGET_LINUX
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#endif
#include <libe/os.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef USE_I2C_BITBANG
#ifdef TARGET_LINUX
struct i2c_master {
	int fd;
};
struct i2c_device {
	struct i2c_master *master;
	uint8_t address;
	uint8_t driver_bits[4];
};
#endif
#endif

/*
 * I2C functions
 */

#ifndef USE_I2C_BITBANG
int i2c_master_open(struct i2c_master *master, void *context, uint32_t frequency, uint8_t scl, uint8_t sda);
void i2c_master_close(struct i2c_master *master);
int i2c_open(struct i2c_device *dev, struct i2c_master *master, uint8_t address);
void i2c_close(struct i2c_device *dev);
int i2c_read(struct i2c_device *dev, void *data, size_t size);
int i2c_write(struct i2c_device *dev, void *data, size_t size);
#else
#include <libe/drivers/i2c/bitbang.h>
#endif

static inline int i2c_write_byte(struct i2c_device *dev, uint8_t value)
{
	return i2c_write(dev, &value, 1);
}

static inline int i2c_write_reg_byte(struct i2c_device *dev, uint8_t reg, uint8_t value)
{
	uint8_t data[2] = { reg, value };
	return i2c_write(dev, data, 2);
}

static inline int i2c_read_reg_byte(struct i2c_device *dev, uint8_t reg)
{
	uint8_t value;
	if (i2c_write(dev, &reg, 1)) {
		return -1;
	}
	if (i2c_read(dev, &value, 1)) {
		return -1;
	}
	return value;
}

#include <libe/drivers/i2c/hdc1080.h>
#include <libe/drivers/i2c/mcp3221.h>
#include <libe/drivers/i2c/fan5702.h>
#include <libe/drivers/i2c/sht21.h>
#include <libe/drivers/i2c/sht31.h>
#include <libe/drivers/i2c/cap1293.h>

#ifdef __cplusplus
}
#endif

#endif /* USE_I2C */
#endif /* _LIBE_I2C_H_ */

