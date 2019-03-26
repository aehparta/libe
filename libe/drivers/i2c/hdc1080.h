/*
 * libe cross-platform library: Ti HDC1080 driver
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_HDC1080_H_
#define _LIBE_HDC1080_H_

#include <stdint.h>
#include <libe/i2c.h>

#ifdef __cplusplus
extern "C" {
#endif

/* HDC1080 I2C address */
#define HDC1080_ADDR        0x40


int hdc1080_open(struct i2c_device *dev, struct i2c_master *master);
#define hdc1080_close(dev) i2c_close(dev)

/* Read temperature and humidity and then save them to each given pointer if they are not NULL */
int hdc1080_read(struct i2c_device *dev, float *t, float *h);


#ifdef __cplusplus
}
#endif

#endif /* _LIBE_HDC1080_H_ */