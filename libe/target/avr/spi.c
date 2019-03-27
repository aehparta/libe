/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <libe/spi.h>
#include <libe/log.h>
#include <libe/os.h>


int spi_master_open(struct spi_master *master, void *context, uint32_t frequency, uint8_t miso, uint8_t mosi, uint8_t sclk)
{
	/* set MOSI, SCK and SS as output and MISO as input */
	os_gpio_output(10);
	os_gpio_output(11);
	os_gpio_input(12);
	os_gpio_output(13);
	/* enable spi */
	SPCR = (1 << SPE) | (1 << MSTR) | (0 << SPR1) | (1 << SPR0) | (0 << DORD);

	/* avr's seem to have only one spi btw */

	return 0;
}

void spi_master_close(struct spi_master *master)
{
	SPCR = 0;
	os_gpio_input(10);
	os_gpio_input(11);
	os_gpio_input(12);
	os_gpio_input(13);
}

int spi_open(struct spi_device *device, struct spi_master *master, uint8_t ss)
{
	device->ss = ss;
	os_gpio_output(ss);
	os_gpio_high(ss);
	/* do not return zero even if the pin is zero :P */
	return 0;
}

void spi_close(struct spi_device *device)
{
	os_gpio_input(device->ss);
}

