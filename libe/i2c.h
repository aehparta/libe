/*
 * libe cross-platform library: I2C bitbang drivers
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_I2C_H_
#define _LIBE_I2C_H_
#ifdef USE_I2C

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_I2C_BITBANG
#include "drivers/i2c/bitbang.h"
#elif TARGET_AVR
#include "target/avr/i2c.h"
#elif TARGET_PIC8
#include "target/pic8/i2c.h"
#elif TARGET_PIC16
#include "target/pic16/i2c.h"
#elif TARGET_PIC32
#include "target/pic32/i2c.h"
#elif TARGET_MSP430
#include "target/msp430/i2c.h"
#define time_t uint32_t
#elif TARGET_ESP32
#include "target/esp32/i2c.h"
#elif TARGET_LINUX
#include "target/linux/i2c.h"
#endif

/*
 * I2C functions
 */

int8_t i2c_master_open(struct i2c_master *master, void *context, uint32_t frequency, uint8_t scl_pin, uint8_t sda_pin);
void i2c_master_close(struct i2c_master *master);
int8_t i2c_open(struct i2c_device *dev, struct i2c_master *master, uint8_t address);
void i2c_close(struct i2c_device *dev);
int8_t i2c_read(struct i2c_device *dev, void *data, uint8_t size);
int8_t i2c_write(struct i2c_device *dev, void *data, uint8_t size);

static inline int8_t i2c_write_byte(struct i2c_device *dev, uint8_t value)
{
	return i2c_write(dev, &value, 1);
}

static inline int8_t i2c_write_reg_byte(struct i2c_device *dev, uint8_t reg, uint8_t value)
{
	uint8_t data[2] = { reg, value };
	return i2c_write(dev, data, 2);
}

static inline uint8_t i2c_read_reg_byte(struct i2c_device *dev, uint8_t reg)
{
	uint8_t value;
	if (i2c_write(dev, &reg, 1) != 0) {
		return 0xff;
	}
	if (i2c_read(dev, &value, 1) != 0) {
		return 0xff;
	}
	return value;
}

#define i2c_read_modify_write_reg_byte(dev, reg, mask_and_first, set_or_then) i2c_write_reg_byte(dev, reg, (i2c_read_reg_byte(dev, reg) & (mask_and_first)) | (set_or_then))

/* drivers */
#include "drivers/i2c/hdc1080.h"
#include "drivers/i2c/mcp3221.h"
#include "drivers/i2c/mcp4725.h"
#include "drivers/i2c/fan5702.h"
#include "drivers/i2c/sht21.h"
#include "drivers/i2c/sht31.h"
#include "drivers/i2c/cap1293.h"
#include "drivers/i2c/act4751.h"
#include "drivers/i2c/ak1595.h"

#ifdef __cplusplus
}
#endif

#endif /* USE_I2C */
#endif /* _LIBE_I2C_H_ */

