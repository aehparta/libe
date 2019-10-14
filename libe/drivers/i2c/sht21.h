/*
 * Sensirion SHT21 temperature and humidity sensor
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_SHT21_H_
#define _LIBE_SHT21_H_

#include <stdint.h>
#include <stdbool.h>
#include <libe/libe.h>

#ifdef __cplusplus
extern "C" {
#endif

/* SHT21 I2C address */
#define SHT21_ADDR        0x40


int8_t sht21_open(struct i2c_device *dev, struct i2c_master *master, uint8_t ref, uint8_t res, uint8_t h_res);
#define sht21_close(dev) i2c_close(dev)
int8_t sht21_read(struct i2c_device *dev, float *t, float *h);

/* hook when compiling i2c tool */
#ifdef COMPILE_TOOL_I2C
void tool_i2c_sht21_help(void);
int tool_i2c_sht21_exec(struct i2c_master *master, uint8_t address, char *command, int argc, char *argv[]);
#pragma push_macro("tool_i2c_chips")
#undef tool_i2c_chips
#define tool_i2c_chips _Pragma("pop_macro(\"tool_i2c_chips\")") tool_i2c_chips \
	{ "sht21", "Sensirion SHT21 temperature and humidity sensor", tool_i2c_sht21_help, tool_i2c_sht21_exec },
#endif

#ifdef __cplusplus
}
#endif

#endif /* _LIBE_SHT21_H_ */