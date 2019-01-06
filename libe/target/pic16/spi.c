/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#include <stdlib.h>
#include <string.h>
#include <libe/spi.h>
#include <libe/debug.h>
#include <libe/os.h>


struct spi_context *spi_open(void *context, uint32_t frequency, int ss)
{
	struct spi_context *spi;

	/* setup context */
	SALLOC(spi, NULL);

	return spi;
}

void spi_close(struct spi_context *spi)
{
	free(spi);
}

int spi_transfer(struct spi_context *spi, uint8_t *data, size_t size)
{
	return 0;
}
