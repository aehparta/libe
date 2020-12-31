/*
 * AK1595 Bluetooth LE transmitter
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_AK1595_H_
#define _LIBE_AK1595_H_

#include <stdint.h>
#include <libe/libe.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ACT4751 I2C address */
#define AK1595_A0           0x28

#define ak1595_open(dev, master, addr)      i2c_open(dev, master, addr)
#define ak1595_close(dev)                   i2c_close(dev)


#ifdef __cplusplus
}
#endif

#endif /* _LIBE_AK1595_H_ */
