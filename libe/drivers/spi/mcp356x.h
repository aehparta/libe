/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_MCP356X_H_
#define _LIBE_MCP356X_H_

#include <stdint.h>
#include <libe/libe.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SPI_DEVICE_NEED_OPT_CB
#define SPI_DEVICE_NEED_OPT_CB
#endif

#if !defined(SPI_DEVICE_NEED_BYTES) || SPI_DEVICE_NEED_BYTES < 1
#undef SPI_DEVICE_NEED_BYTES
#define SPI_DEVICE_NEED_BYTES 1
#endif


#define MCP356X_DEFAULT_ADDR        0x40

#define MCP356X_CMD_CONV_START      (0xa << 2)
#define MCP356X_CMD_STANDBY         (0xb << 2)
#define MCP356X_CMD_SHUTDOWN        (0xc << 2)
#define MCP356X_CMD_FULL_SHUTDOWN   (0xd << 2)
#define MCP356X_CMD_RESET           (0xe << 2)

#define MCP356X_ADDR_ADCDATA        (0x0 << 2)
#define MCP356X_ADDR_CONFIG0        (0x1 << 2)
#define MCP356X_ADDR_CONFIG1        (0x2 << 2)
#define MCP356X_ADDR_CONFIG2        (0x3 << 2)
#define MCP356X_ADDR_CONFIG3        (0x4 << 2)
#define MCP356X_ADDR_IRQ            (0x5 << 2)
#define MCP356X_ADDR_MUX            (0x6 << 2)
#define MCP356X_ADDR_OFFSET_CAL     (0x9 << 2)

#define MCP356X_STATIC_READ         1
#define MCP356X_INC_WRITE           2
#define MCP356X_INC_READ            3

#define MCP356X_CH_01               0x01
#define MCP356X_CH_23               0x23
#define MCP356X_CH_45               0x45
#define MCP356X_CH_67               0x67
#define MCP356X_CH_TEMP             0xde

#define MCP356X_OPT_ADDR            (OPT_START_DEVICE + 0)
#define MCP356X_OPT_CLK_SEL         (OPT_START_DEVICE + 4)
#define MCP356X_OPT_CS_SEL          (OPT_START_DEVICE + 5)
#define MCP356X_OPT_ADC_MODE        (OPT_START_DEVICE + 6)
#define MCP356X_OPT_PRE             (OPT_START_DEVICE + 7)
#define MCP356X_OPT_OSR             (OPT_START_DEVICE + 8)
#define MCP356X_OPT_BOOST           (OPT_START_DEVICE + 9)
#define MCP356X_OPT_GAIN            (OPT_START_DEVICE + 10)
#define MCP356X_OPT_AZ_MUX          (OPT_START_DEVICE + 11)
#define MCP356X_OPT_CONV_MODE       (OPT_START_DEVICE + 12)
// #define MCP356X_OPT_DATA_FORMAT     (OPT_START_DEVICE + 13)
// #define MCP356X_OPT_CRC_FORMAT      (OPT_START_DEVICE + 14)
// #define MCP356X_OPT_EN_CRCCOM       (OPT_START_DEVICE + 15)
// #define MCP356X_OPT_EN_OFFCAL       (OPT_START_DEVICE + 16)
// #define MCP356X_OPT_EN_GAINCAL      (OPT_START_DEVICE + 17)
#define MCP356X_OPT_IRQ_MODE        (OPT_START_DEVICE + 18)
// #define MCP356X_OPT_EN_FAST_CMD     (OPT_START_DEVICE + 19)
// #define MCP356X_OPT_CRC_EN_STP      (OPT_START_DEVICE + 20)

#define MCP356X_OSR_98304           0xf
#define MCP356X_OSR_81920           0xe
#define MCP356X_OSR_49152           0xd
#define MCP356X_OSR_40960           0xc
#define MCP356X_OSR_24576           0xb
#define MCP356X_OSR_20480           0xa
#define MCP356X_OSR_16384           0x9
#define MCP356X_OSR_8192            0x8
#define MCP356X_OSR_4096            0x7
#define MCP356X_OSR_2048            0x6
#define MCP356X_OSR_1024            0x5
#define MCP356X_OSR_512             0x4
#define MCP356X_OSR_256             0x3
#define MCP356X_OSR_128             0x2
#define MCP356X_OSR_64              0x1
#define MCP356X_OSR_32              0x0

#define MCP356X_GAIN_X64            0x7
#define MCP356X_GAIN_X32            0x6
#define MCP356X_GAIN_X16            0x5
#define MCP356X_GAIN_X8             0x4
#define MCP356X_GAIN_X4             0x3
#define MCP356X_GAIN_X2             0x2
#define MCP356X_GAIN_X1             0x1
#define MCP356X_GAIN_XTHIRD         0x0


int8_t mcp356x_open(struct spi_device *device, struct spi_master *master, uint8_t ss);
void mcp356x_close(struct spi_device *device);
int32_t mcp356x_rd(struct spi_device *device);
int8_t mcp356x_ch(struct spi_device *device, int8_t channel);

uint8_t mcp356x_fast_command(struct spi_device *device, uint8_t command);


#ifdef __cplusplus
}
#endif

#endif /* _LIBE_MCP356X_H_ */