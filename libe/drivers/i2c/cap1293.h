/*
 * Microchip CAP1293 3-channel touch sensor
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_CAP1293_H_
#define _LIBE_CAP1293_H_

#include <stdint.h>
#include <libe/libe.h>

#ifdef __cplusplus
extern "C" {
#endif

/* CAP1293 I2C address */
#define CAP1293_ADDR            0x28

int cap1293_open(struct i2c_device *dev, struct i2c_master *master);
int cap1293_conf(struct i2c_device *dev, uint8_t channels_mask);
#define cap1293_close(dev) i2c_close(dev)

int cap1293_read(struct i2c_device *dev);

/* hook when compiling i2c tool */
#ifdef COMPILE_TOOL_I2C
void tool_i2c_cap1293_help(void);
int tool_i2c_cap1293_exec(struct i2c_master *master, uint8_t address, char *command, int argc, char *argv[]);
#pragma push_macro("tool_i2c_chips")
#undef tool_i2c_chips
#define tool_i2c_chips _Pragma("pop_macro(\"tool_i2c_chips\")") tool_i2c_chips \
	{ "cap1293", "Microchip CAP1293 3-channel touch sensor", tool_i2c_cap1293_help, tool_i2c_cap1293_exec },
#endif

#ifdef __cplusplus
}
#endif

#endif /* _LIBE_CAP1293_H_ */
