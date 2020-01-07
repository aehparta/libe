/*
 * Qorvo ACT4751 Programmable DC-DC converter driver
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_ACT4751_H_
#define _LIBE_ACT4751_H_

#include <stdint.h>
#include <libe/libe.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ACT4751 I2C address */
#define ACT4751_ADDR            0x08
#define ACT4751_ADDR_SLAVE1     0x09
#define ACT4751_ADDR_SLAVE2     0x0a
#define ACT4751_ADDR_SLAVE3     0x0b
#define ACT4751_ADDR_SLAVE4     0x0c
#define ACT4751_ADDR_MASTER     0x0d

#define act4751_open(dev, master, address) i2c_open(dev, master, address)
#define act4751_close(dev) i2c_close(dev)

int8_t act4751_set_main_voltage(struct i2c_device *dev, float voltage);
float act4751_get_main_voltage(struct i2c_device *dev);

int8_t act4751_set_main_current(struct i2c_device *dev, float current);
float act4751_get_main_current(struct i2c_device *dev);


#ifdef __cplusplus
}
#endif

#endif /* _LIBE_ACT4751_H_ */
