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


int spi_master_open(struct spi_master *master, void *context, uint32_t frequency, uint8_t miso, uint8_t mosi, uint8_t sclk)
{
	return -1;
}

void spi_master_close(struct spi_master *master)
{

}


int spi_open(struct spi_device *device, struct spi_master *master, uint8_t ss)
{
	return -1;
}

void spi_close(struct spi_device *device)
{
}

int spi_transfer(struct spi_device *device, uint8_t *data, size_t size)
{
	return -1;
}
