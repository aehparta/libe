/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef __SPI_H__
#define __SPI_H__


#include <stdint.h>
#include <string.h>
#ifdef TARGET_ESP32
#include <driver/spi_master.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif

typedef void * spi_master_t;
typedef void * spi_device_t;

spi_master_t spi_master_open(void *context, uint32_t frequency, uint8_t miso, uint8_t mosi, uint8_t sclk);
void spi_master_close(spi_master_t master);

spi_device_t spi_open(spi_master_t master, uint8_t ss);
void spi_close(spi_device_t device);
int spi_transfer(spi_device_t device, uint8_t *data, size_t size);

#ifdef TARGET_X86
/* only when using ftdi */
int spi_ftdi_set(spi_master_t master, uint8_t pins);
int spi_ftdi_clr(spi_master_t master, uint8_t pins);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */
