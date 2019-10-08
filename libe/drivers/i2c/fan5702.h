/*
 * Fairchild FAN5702 LED driver
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_FAN5702_H_
#define _LIBE_FAN5702_H_

#include <stdint.h>
#include <libe/libe.h>

#ifdef __cplusplus
extern "C" {
#endif

/* FAN5702 I2C address */
#define FAN5702_ADDR			0x36

int fan5702_open(struct i2c_device *dev, struct i2c_master *master);
#define fan5702_close(dev) i2c_close(dev)

int fan5702_set(struct i2c_device *dev, uint8_t channel, uint8_t brightness);

/* hook when compiling i2c tool */
#ifdef COMPILE_TOOL_I2C
void tool_i2c_fan5702_help(void);
int tool_i2c_fan5702_exec(struct i2c_master *master, uint8_t address, char *command, int argc, char *argv[]);
#pragma push_macro("tool_i2c_chips")
#undef tool_i2c_chips
#define tool_i2c_chips _Pragma("pop_macro(\"tool_i2c_chips\")") tool_i2c_chips \
	{ "fan5702", "Fairchild FAN5702 LED driver", tool_i2c_fan5702_help, tool_i2c_fan5702_exec },
#endif

#ifdef __cplusplus
}
#endif

#endif /* _LIBE_FAN5702_H_ */
