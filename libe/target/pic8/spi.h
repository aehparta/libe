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

/* spi master context */
struct spi_master {
	/* pick compiler crashes without this.. */
	uint8_t placeholder;
};

/* spi device context */
struct spi_device {
	uint8_t ss;
};

#ifdef __cplusplus
}
#endif

#endif /* _TARGET_PIC8_SPI_H_ */
