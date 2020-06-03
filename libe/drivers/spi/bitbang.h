/*
 * SPI bitbang drivers
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_SPI_BITBANG_H_
#define _LIBE_SPI_BITBANG_H_

#include <libe/libe.h>

#ifdef __cplusplus
extern "C" {
#endif

int spibb_master_open(struct spi_master *master, void *context, uint32_t frequency, uint8_t miso, uint8_t mosi, uint8_t sclk);
void spibb_master_close(struct spi_master *master);

int spibb_open(struct spi_device *device, struct spi_master *master, uint8_t ss);
void spibb_close(struct spi_device *device);
int spibb_transfer(struct spi_device *device, uint8_t *data, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* _LIBE_SPI_BITBANG_H_ */
