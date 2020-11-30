/*
 * Microchip MCP4725 12-bit DAC
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_MCP4725_H_
#define _LIBE_MCP4725_H_

#include <stdint.h>
#include <math.h>
#include <libe/libe.h>

#ifdef __cplusplus
extern "C" {
#endif

/* MCP4725 I2C address, 0x60/0x61 are default, others can be requested from manufacturer */
#define MCP4725_ADDR_A0         0x60
#define MCP4725_ADDR_A1         0x61
#define MCP4725_ADDR_A2         0x62
#define MCP4725_ADDR_A3         0x63

#ifndef MCP4725_ADDR
#define MCP4725_ADDR            0x60
#endif


int8_t mcp4725_open(struct i2c_device *dev, struct i2c_master *master, uint8_t address);
#define mcp4725_close(dev) i2c_close(dev)

int8_t mcp4725_wr(struct i2c_device *dev, uint8_t channel, int32_t v);
int8_t mcp4725_wr_ee(struct i2c_device *dev);

int32_t mcp4725_rd(struct i2c_device *dev, uint8_t channel);

float mcp4725_rd_pdr(struct i2c_device *dev);
int32_t mcp4725_rd_ee(struct i2c_device *dev);
float mcp4725_rd_ee_pdr(struct i2c_device *dev);

/* hook when compiling i2c tool */
#ifdef COMPILE_TOOL_I2C
void tool_i2c_mcp4725_help(void);
int tool_i2c_mcp4725_exec(struct i2c_master *master, uint8_t address, char *command, int argc, char *argv[]);
#pragma push_macro("tool_i2c_chips")
#undef tool_i2c_chips
#define tool_i2c_chips _Pragma("pop_macro(\"tool_i2c_chips\")") tool_i2c_chips \
	{ "mcp4725", "Microchip MCP4725 12-bit DAC", tool_i2c_mcp4725_help, tool_i2c_mcp4725_exec },
#endif

#ifdef __cplusplus
}
#endif

#endif /* _LIBE_MCP4725_H_ */
