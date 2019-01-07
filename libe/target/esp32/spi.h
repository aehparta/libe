/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _TARGET_ESP32_SPI_H_
#define _TARGET_ESP32_SPI_H_

#include <stdint.h>
#include <driver/spi_master.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SPI_DEFAULT_FREQUENCY 10e6

/* esp32 spi master context */
struct spi_master {
	spi_host_device_t host;
	uint32_t frequency;
};
/* esp32 spi device context */
struct spi_device {
	spi_device_handle_t device;
};

#ifdef __cplusplus
}
#endif

#endif /* _TARGET_ESP32_SPI_H_ */
