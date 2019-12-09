/*
 * SPI bitbang drivers
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_SPI_BITBANG_H_
#define _LIBE_SPI_BITBANG_H_

#ifdef USE_SPI_BITBANG

#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_SPI_BITBANG_DYNAMIC

#define SPI_BITBANG_SCLK (master->sclk)
#define SPI_BITBANG_MOSI (master->mosi)
#define SPI_BITBANG_MISO (master->miso)
struct spi_master {
	uint8_t sclk;
	uint8_t mosi;
	uint8_t miso;
	uint32_t frequency;
};
struct spi_device {
	struct spi_master *master;
	uint8_t ss;
	uint8_t driver_bits[4];
};

#else

struct spi_master {
#ifdef TARGET_PIC8
	/* xc8 compiler crashes if this is not here */
	uint8_t placeholder;
#endif
};
struct spi_device {
	struct spi_master *master;
	uint8_t ss;
	uint8_t driver_bits[4];
};

#endif


#ifdef __cplusplus
}
#endif

#endif /* USE_SPI_BITBANG */

#endif /* _LIBE_SPI_BITBANG_H_ */
