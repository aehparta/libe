/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _TARGET_RPI_SPI_H_
#define _TARGET_RPI_SPI_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SPI_DEFAULT_FREQUENCY 10e6

/* ftdi spi master context */
struct spi_master {
	int fd;
	uint32_t frequency;
};
/* ftdi spi device context */
struct spi_device {
	struct spi_master *m;
	uint8_t ss;
};

#ifdef __cplusplus
}
#endif

#endif /* _TARGET_RPI_SPI_H_ */
