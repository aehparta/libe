/*
 * libe cross-platform library: mcp3221 i2c driver
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_MCP3221_H_
#define _LIBE_MCP3221_H_

#include <stdint.h>
#include <libe/i2c.h>

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


#define mcp3221_open(dev, master, address) i2c_open(dev, master, address)
#define mcp3221_close(dev) i2c_close(dev)
int16_t mcp3221_read(struct i2c_device *dev);

#ifdef __cplusplus
}
#endif

#endif /* _LIBE_MCP3221_H_ */