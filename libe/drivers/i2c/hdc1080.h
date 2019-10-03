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


/* hook when compiling i2c tool */
#ifdef COMPILE_TOOL_I2C
void tool_i2c_hdc1080_help(void);
void tool_i2c_hdc1080_exec(char *command, int argc, char *argv[]);
#pragma push_macro("tool_i2c_chips")
#undef tool_i2c_chips
#define tool_i2c_chips _Pragma("pop_macro(\"tool_i2c_chips\")") tool_i2c_chips \
	{ "HDC1080", "Ti temperature and humidity sensor", tool_i2c_hdc1080_help, tool_i2c_hdc1080_exec },
#endif

#ifdef __cplusplus
}
#endif

#endif /* _LIBE_HDC1080_H_ */