/*
 * Microchip MCP3221 12-bit ADC
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_MCP3221_H_
#define _LIBE_MCP3221_H_

#ifdef __cplusplus
extern "C" {
#endif

/* device addresses */
#define MCP3221_ADDR_A0     0x48
#define MCP3221_ADDR_A1     0x49
#define MCP3221_ADDR_A2     0x4a
#define MCP3221_ADDR_A3     0x4b
#define MCP3221_ADDR_A4     0x4c
#define MCP3221_ADDR_A5     0x4d
#define MCP3221_ADDR_A6     0x4e
#define MCP3221_ADDR_A7     0x4f

#ifndef MCP3221_ADDR
#define MCP3221_ADDR        MCP3221_ADDR_A5
#endif

#define mcp3221_open(dev, master, address) i2c_open(dev, master, ((address) > 0) ? (address) : MCP3221_ADDR)
#define mcp3221_close(dev) i2c_close(dev)
int32_t mcp3221_rd(struct i2c_device *dev, uint8_t channel);


/* hook when compiling i2c tool */
#ifdef COMPILE_TOOL_I2C
void tool_i2c_mcp3221_help(void);
int tool_i2c_mcp3221_exec(struct i2c_master *master, uint8_t address, char *command, int argc, char *argv[]);
#pragma push_macro("tool_i2c_chips")
#undef tool_i2c_chips
#define tool_i2c_chips _Pragma("pop_macro(\"tool_i2c_chips\")") tool_i2c_chips \
	{ "mcp3221", "Microchip MCP3221 12-bit ADC", tool_i2c_mcp3221_help, tool_i2c_mcp3221_exec },
#endif

#ifdef __cplusplus
}
#endif

#endif /* _LIBE_MCP3221_H_ */