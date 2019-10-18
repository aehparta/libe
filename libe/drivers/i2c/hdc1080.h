/*
 * Ti HDC1080 temperature and humidity sensor
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_HDC1080_H_
#define _LIBE_HDC1080_H_

#include <stdint.h>
#include <stdbool.h>
#include <libe/libe.h>

#ifdef __cplusplus
extern "C" {
#endif

/* HDC1080 I2C address */
#define HDC1080_ADDR        0x40


int8_t hdc1080_open(struct i2c_device *dev, struct i2c_master *master, uint8_t ref, int8_t res, int8_t h_res);
#define hdc1080_close(dev) i2c_close(dev)
int8_t hdc1080_heater(struct i2c_device *dev, bool on);
int8_t hdc1080_read(struct i2c_device *dev, float *t, float *h);

/* hook when compiling i2c tool */
#ifdef COMPILE_TOOL_I2C
void tool_i2c_hdc1080_help(void);
int tool_i2c_hdc1080_exec(struct i2c_master *master, uint8_t address, char *command, int argc, char *argv[]);
#pragma push_macro("tool_i2c_chips")
#undef tool_i2c_chips
#define tool_i2c_chips _Pragma("pop_macro(\"tool_i2c_chips\")") tool_i2c_chips \
	{ "hdc1080", "Ti HDC1080 temperature and humidity sensor", tool_i2c_hdc1080_help, tool_i2c_hdc1080_exec },
#endif

#ifdef __cplusplus
}
#endif

#endif /* _LIBE_HDC1080_H_ */