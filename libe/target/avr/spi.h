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

static inline int spi_transfer(struct spi_device *device, uint8_t *data, size_t size)
{
	os_gpio_low(device->ss);
	for ( ; size > 0; size--) {
		SPDR = *data;
		while (!(SPSR & (1 << SPIF)));
		*data = SPDR;
		data++;
	}
	os_gpio_high(device->ss);

	return 0;
}

#ifdef __cplusplus
}
#endif

#endif /* _TARGET_AVR_SPI_H_ */
