/*
 * Sensirion SHT31 temperature and humidity sensor
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_SHT31_H_
#define _LIBE_SHT31_H_

#include <stdint.h>
#include <stdbool.h>
#include <libe/libe.h>

#ifdef __cplusplus
extern "C" {
#endif

/* SHT31 I2C addresses */
#define SHT31_ADDR_A        0x44
#define SHT31_ADDR_B        0x45
/* default address for open command without address */
#ifndef SHT31_ADDR
#define SHT31_ADDR          SHT31_ADDR_A
#endif

/* SHT31 repeatability */
#define SHT31_REPEATABILITY_HIGH        0
#define SHT31_REPEATABILITY_MEDIUM      1
#define SHT31_REPEATABILITY_LOW         2


int sht31_open(struct i2c_device *dev, struct i2c_master *master);
int sht31_open_addr(struct i2c_device *dev, struct i2c_master *master, uint8_t address);
int sht31_conf(struct i2c_device *dev, bool heater, uint8_t repeatability);

#define sht31_close(dev) i2c_close(dev)

int sht31_read(struct i2c_device *dev, float *t, float *h);
float sht31_read_temperature(struct i2c_device *dev);
float sht31_read_humidity(struct i2c_device *dev);


/* hook when compiling i2c tool */
#ifdef COMPILE_TOOL_I2C
void tool_i2c_sht31_help(void);
int tool_i2c_sht31_exec(struct i2c_master *master, uint8_t address, char *command, int argc, char *argv[]);
#pragma push_macro("tool_i2c_chips")
#undef tool_i2c_chips
#define tool_i2c_chips _Pragma("pop_macro(\"tool_i2c_chips\")") tool_i2c_chips \
	{ "sht31", "Sensirion SHT31 temperature and humidity sensor", tool_i2c_sht31_help, tool_i2c_sht31_exec },
#endif

#ifdef __cplusplus
}
#endif

#endif /* _LIBE_SHT31_H_ */