/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _TARGET_LINUX_SPIFTDI_H_
#define _TARGET_LINUX_SPIFTDI_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SPI_MASTER_NEED_PINS
#define SPI_DEVICE_NEED_MASTER
#define SPI_DEVICE_NEED_SS

#define SPIFTDI_DEFAULT_FREQUENCY 1e6

int spiftdi_master_open(struct spi_master *master, void *context, uint32_t frequency, uint8_t miso, uint8_t mosi, uint8_t sclk);
void spiftdi_master_close(struct spi_master *master);

int spiftdi_open(struct spi_device *device, struct spi_master *master, uint8_t ss);
void spiftdi_close(struct spi_device *device);
int spiftdi_transfer(struct spi_device *device, uint8_t *data, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* _TARGET_LINUX_SPIFTDI_H_ */
