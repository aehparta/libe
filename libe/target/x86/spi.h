/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _TARGET_X86_SPI_H_
#define _TARGET_X86_SPI_H_

#include <stdint.h>
#include <libe/os.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_FTDI

/* ftdi spi master context */
struct spi_master {
	struct ftdi_context *ftdi;
	/* pin states */
	uint8_t state;
	/* pin directions */
	uint8_t dir;
};
/* ftdi spi device context */
struct spi_device {
	struct spi_master *m;
	uint8_t ss;
};

int spi_ftdi_set(struct spi_master *master, uint8_t pins);
int spi_ftdi_clr(struct spi_master *master, uint8_t pins);

#else

struct spi_master {};
struct spi_device {
	struct spi_master *m;
	uint8_t ss;
};

#endif

#ifdef __cplusplus
}
#endif

#endif /* _TARGET_X86_SPI_H_ */
