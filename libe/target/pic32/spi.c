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

struct spi_master {
	uint32_t frequency;
};

struct spi_device {
	struct spi_master *m;
	uint8_t ss;
};


spi_master_t spi_master_open(void *context, uint32_t frequency, uint8_t miso, uint8_t mosi, uint8_t sclk)
{
	struct spi_master *m;

	/* setup context */
	SALLOC(m, NULL);
	m->frequency = frequency;

	return (spi_master_t)m;
}

void spi_master_close(spi_master_t master)
{
	if (master) {
		struct spi_master *m = (struct spi_master *)master;
		free(m);
	}
}

spi_device_t spi_open(spi_master_t master, uint8_t ss)
{
	struct spi_master *m = (struct spi_master *)master;
	struct spi_device *d;

	/* setup context */
	SALLOC(d, NULL);
	d->m = m;
	d->ss = ss;
	os_gpio_output(ss);
	os_gpio_high(ss);

	return (spi_master_t)m;
}

void spi_close(spi_device_t device)
{
	if (device) {
		struct spi_device *d = (struct spi_device *)device;
		os_gpio_input(d->ss);
		free(d);
	}
}

int spi_transfer(spi_device_t device, uint8_t *data, size_t size)
{
	// struct spi_device *d = (struct spi_device *)device;
	return 0;
}
