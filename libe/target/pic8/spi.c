/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#include <stdlib.h>
#include <string.h>
#include <libe/spi.h>
#include <libe/log.h>
#include <libe/os.h>


int spi_master_open(struct spi_master *master, void *context, uint32_t frequency, uint8_t miso, uint8_t mosi, uint8_t sclk)
{
	os_gpio_input(miso);
	os_gpio_output(mosi);
	os_gpio_output(sclk);
#ifdef SSPCON1
	SSPCON1 = 0x20;
#else
	SSPCON = 0x20;
#endif
	SSPSTAT = 0;
	return 0;
}

void spi_master_close(struct spi_master *master)
{
#ifdef SSPCON1
	SSPCON1 = 0;
#else
	SSPCON = 0;
#endif
	SSPSTAT = 0;
}

int spi_open(struct spi_device *device, struct spi_master *master, uint8_t ss)
{
	os_gpio_output(ss);
	os_gpio_high(ss);
	device->ss = ss;
	return 0;
}

void spi_close(struct spi_device *device)
{
	os_gpio_input(device->ss);
}

int spi_transfer(struct spi_device *device, uint8_t *data, size_t size)
{
	os_gpio_low(device->ss);
	for ( ; size > 0; size--) {
		SSPBUF = *data;
		while (!BF);
		*data = SSPBUF;
		data++;
	}
	os_gpio_high(device->ss);
	
	return 0;
}
