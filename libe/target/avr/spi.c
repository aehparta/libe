/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <libe/spi.h>
#include <libe/debug.h>
#include <libe/os.h>


/* spi device context */
struct spi_device {
	int8_t ss;
};


spi_master_t spi_master_open(void *context, uint32_t frequency, uint8_t miso, uint8_t mosi, uint8_t sclk)
{
	/* set MOSI, SCK and SS as output and MISO as input */
	os_gpio_output(10);
	os_gpio_output(11);
	os_gpio_input(12);
	os_gpio_output(13);
	/* enable spi */
	SPCR = (1 << SPE) | (1 << MSTR) | (0 << SPR1) | (1 << SPR0) | (0 << DORD);
	/* avr's seem to have only one spi, no need for separate struct */
	return (spi_master_t)1;
}

void spi_master_close(spi_master_t master)
{
	SPCR = 0;
	os_gpio_input(10);
	os_gpio_input(11);
	os_gpio_input(12);
	os_gpio_input(13);
}

spi_device_t spi_open(spi_master_t master, uint8_t ss)
{
	os_gpio_output(ss);
	os_gpio_high(ss);
	/* do not return zero even if the pin is zero :P */
	return (spi_device_t)(ss + 1);
}

void spi_close(spi_device_t device)
{
	uint8_t ss = ((int)device) - 1;
	os_gpio_input(ss);
}

int spi_transfer(spi_device_t device, uint8_t *data, size_t size)
{
	uint8_t ss = ((int)device) - 1;

	os_gpio_low(ss);
	for ( ; size > 0; size--) {
		SPDR = *data;
		while (!(SPSR & (1 << SPIF)));
		*data = SPDR;
		data++;
	}
	os_gpio_high(ss);
	
	return 0;
}
