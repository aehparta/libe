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


int hdc1080_open(struct i2c_device *dev, struct i2c_master *master);
int hdc1080_conf(struct i2c_device *dev, bool heater, bool mode, uint8_t t_res, uint8_t h_res);

#define hdc1080_close(dev) i2c_close(dev)

int hdc1080_read(struct i2c_device *dev, float *t, float *h);
float hdc1080_read_temperature(struct i2c_device *dev);
float hdc1080_read_humidity(struct i2c_device *dev);


/* hook when compiling i2c tool */
#ifdef COMPILE_TOOL_I2C
void tool_i2c_hdc1080_help(void);
int tool_i2c_hdc1080_exec(struct i2c_master *master, char *command, int argc, char *argv[]);
#pragma push_macro("tool_i2c_chips")
#undef tool_i2c_chips
#define tool_i2c_chips _Pragma("pop_macro(\"tool_i2c_chips\")") tool_i2c_chips \
	{ "hdc1080", "Ti HDC1080 temperature and humidity sensor", tool_i2c_hdc1080_help, tool_i2c_hdc1080_exec },
#endif

#ifdef __cplusplus
}
#endif

#endif /* _LIBE_HDC1080_H_ */