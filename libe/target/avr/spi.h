/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _TARGET_AVR_SPI_H_
#define _TARGET_AVR_SPI_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ftdi spi master context */
struct spi_master {
};

/* ftdi spi device context */
struct spi_device {
	uint8_t ss;
};

#ifdef __cplusplus
}
#endif

#endif /* _TARGET_AVR_SPI_H_ */
