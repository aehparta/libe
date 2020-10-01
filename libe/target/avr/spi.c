/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#include <stdlib.h>
#include <string.h>
#include <libe/libe.h>


int spii_master_open(struct spi_master *master, void *context, uint32_t frequency, uint8_t miso, uint8_t mosi, uint8_t sclk)
{
	/* set MOSI, SCK and SS as output and MISO as input */
	gpio_output(GPIOB2); /* SS */
	gpio_output(GPIOB3); /* MOSI */
	gpio_input(GPIOB4); /* MISO */
	gpio_output(GPIOB5); /* SCLK */
	/* enable spi */
	SPCR = (1 << SPE) | (1 << MSTR) | (0 << SPR1) | (1 << SPR0) | (0 << DORD);

	/* avr's seem to have only one spi btw */

	return 0;
}

void spii_master_close(struct spi_master *master)
{
	SPCR = 0;
	gpio_input(GPIOB2);
	gpio_input(GPIOB3);
	gpio_input(GPIOB4);
	gpio_input(GPIOB5);
}

int spii_open(struct spi_device *device, struct spi_master *master, uint8_t ss)
{
	device->ss = ss;
	gpio_output(ss);
	gpio_high(ss);
	return 0;
}

void spii_close(struct spi_device *device)
{
	gpio_input(device->ss);
}

int spii_transfer(struct spi_device *device, uint8_t *data, size_t size)
{
	gpio_low(device->ss);
	for ( ; size > 0; size--) {
		SPDR = *data;
		while (!(SPSR & (1 << SPIF)));
		*data = SPDR;
		data++;
	}
	gpio_high(device->ss);

	return 0;
}
