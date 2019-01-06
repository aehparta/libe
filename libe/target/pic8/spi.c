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


spi_master_t spi_master_open(void *context, uint32_t frequency, uint8_t miso, uint8_t mosi, uint8_t sclk)
{
	os_gpio_output(19); /* sclk */
	os_gpio_input(20); /* miso */
	os_gpio_output(21); /* mosi */
#ifdef SSPCON1
	SSPCON1 = 0x20;
#else
	SSPCON = 0x20;
#endif
	SSPSTAT = 0;
	return (spi_master_t)1;
}

void spi_master_close(spi_master_t master)
{
#ifdef SSPCON1
	SSPCON1 = 0;
#else
	SSPCON = 0;
#endif
	SSPSTAT = 0;
	os_gpio_input(19);
	os_gpio_input(20);
	os_gpio_input(21);
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
	uint8_t ss = ((uint8_t)device) - 1;
	os_gpio_input(ss);
}

int spi_transfer(spi_device_t device, uint8_t *data, size_t size)
{
	uint8_t ss = ((uint8_t)device) - 1;

	os_gpio_low(ss);
	for ( ; size > 0; size--) {
		SSPBUF = *data;
		while (!BF);
		*data = SSPBUF;
		data++;
	}
	os_gpio_high(ss);
	
	return 0;
}
