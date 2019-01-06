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


/* spi device context */
struct spi_device {
	int8_t ss;
};


spi_master_t spi_master_open(void *context, uint32_t frequency, uint8_t miso, uint8_t mosi, uint8_t sclk)
{
	return NULL;
}

void spi_master_close(spi_master_t master)
{

}


spi_device_t spi_open(spi_master_t master, uint8_t ss)
{
	return NULL;
	
	struct spi_device *d;

	/* setup context */
	SALLOC(d, NULL);

	return d;
}

void spi_close(spi_device_t device)
{
}

int spi_transfer(spi_device_t device, uint8_t *data, size_t size)
{
	return 0;
}
