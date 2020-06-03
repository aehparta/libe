/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _TARGET_PIC8_SPI_H_
#define _TARGET_PIC8_SPI_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SPI_HAS_INTEGRATED
#define SPI_DEVICE_NEED_SS

int spii_master_open(struct spi_master *master, void *context, uint32_t frequency, uint8_t miso, uint8_t mosi, uint8_t sclk);
void spii_master_close(struct spi_master *master);

int spii_open(struct spi_device *device, struct spi_master *master, uint8_t ss);
void spii_close(struct spi_device *device);
int spii_transfer(struct spi_device *device, uint8_t *data, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* _TARGET_PIC8_SPI_H_ */
