/*
 * Qorvo ACT4751 Programmable DC-DC converter driver
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_ACT4751_H_
#define _LIBE_ACT4751_H_

#include <stdint.h>
#include <libe/libe.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ACT4751 I2C address */
#define ACT4751_ADDR            0x08
#define ACT4751_ADDR_SLAVE1     0x09
#define ACT4751_ADDR_SLAVE2     0x0a
#define ACT4751_ADDR_SLAVE3     0x0b
#define ACT4751_ADDR_SLAVE4     0x0c
#define ACT4751_ADDR_MASTER     0x0d

#define ACT4751_REG_STATE       0x00
#define ACT4751_REG_INT1        0x01
#define ACT4751_REG_INT2        0x02
#define ACT4751_REG_STATUS      0x03
#define ACT4751_REG_RESERVED    0x04
#define ACT4751_REG_COMMAND     0x05
#define ACT4751_REG_INTMASK1    0x06
#define ACT4751_REG_INTMASK2    0x07
#define ACT4751_REG_CFG1        0x08
#define ACT4751_REG_CFG2        0x09
#define ACT4751_REG_DACOUT      0x0a
#define ACT4751_REG_CFG3        0x0b
#define ACT4751_REG_ILIM        0x0c
#define ACT4751_REG_AUXLDO      0x0d
#define ACT4751_REG_CFG4        0x0e
#define ACT4751_REG_CFG5        0x0f


int8_t act4751_open(struct i2c_device *dev, struct i2c_master *master, uint8_t address, uint16_t rilim);
#define act4751_close(dev) i2c_close(dev)

int8_t act4751_set_main_voltage(struct i2c_device *dev, float voltage);
float act4751_get_main_voltage(struct i2c_device *dev);

int8_t act4751_set_main_current(struct i2c_device *dev, float current);
float act4751_get_main_current(struct i2c_device *dev);

int8_t act4751_set_ldo_voltage(struct i2c_device *dev, float voltage);
float act4751_get_ldo_voltage(struct i2c_device *dev);

int8_t act4751_set_mini_buck(struct i2c_device *dev, bool enable);
int8_t act4751_get_mini_buck(struct i2c_device *dev);

#define act4751_set_uvp(dev, enable) i2c_read_modify_write_reg_byte(dev, ACT4751_REG_CFG2, 0xf7, enable ? 0x00 : 0x08)

#ifdef __cplusplus
}
#endif

#endif /* _LIBE_ACT4751_H_ */
