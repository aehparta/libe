/*
 * I2C ESP32 drivers
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifndef _TARGET_ESP32_I2C_H_
#define _TARGET_ESP32_I2C_H_

#include <driver/i2c.h>

#ifdef __cplusplus
extern "C" {
#endif

struct i2c_master {
	i2c_port_t port;
};

struct i2c_device {
	struct i2c_master *master;
	uint8_t address;
	uint8_t driver_bits[4];
};

#ifdef __cplusplus
}
#endif

#endif /* _TARGET_ESP32_I2C_H_ */
