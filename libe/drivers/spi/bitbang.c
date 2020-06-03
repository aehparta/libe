/*
 * SPI bitbang drivers
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifdef USE_SPI_BITBANG

#include <libe/libe.h>

#ifdef USE_SPI_BITBANG_DYNAMIC
#define SPI_BITBANG_SCLK (master->sclk)
#define SPI_BITBANG_MOSI (master->mosi)
#define SPI_BITBANG_MISO (master->miso)
#else
#ifndef SPI_BITBANG_SCLK
#error "SPI_BITBANG_SCLK not defined"
#endif
#ifndef SPI_BITBANG_MOSI
#error "SPI_BITBANG_MOSI not defined"
#endif
#ifndef SPI_BITBANG_MISO
#error "SPI_BITBANG_MISO not defined"
#endif
#endif


int spibb_master_open(struct spi_master *master, void *context, uint32_t frequency, uint8_t miso, uint8_t mosi, uint8_t sclk)
{
#ifdef USE_SPI_BITBANG_DYNAMIC
	master->sclk = sclk;
	master->mosi = mosi;
	master->miso = miso;
	master->frequency = frequency;
#endif
	gpio_output(SPI_BITBANG_SCLK);
	gpio_low(SPI_BITBANG_SCLK);
	gpio_output(SPI_BITBANG_MOSI);
	gpio_input(SPI_BITBANG_MISO);
	return 0;
}

void spibb_master_close(struct spi_master *master)
{
	gpio_input(SPI_BITBANG_SCLK);
	gpio_input(SPI_BITBANG_MOSI);
	gpio_input(SPI_BITBANG_MISO);
}

int spibb_open(struct spi_device *device, struct spi_master *master, uint8_t ss)
{
	gpio_output(ss);
	gpio_high(ss);
	device->ss = ss;
	return 0;
}

void spibb_close(struct spi_device *device)
{
	gpio_input(device->ss);
}

int spibb_transfer(struct spi_device *device, uint8_t *data, size_t size)
{
	struct spi_master *master = device->master;

	gpio_low(device->ss);
	os_delay_us(1);
	for ( ; size > 0; size--) {
		uint8_t o = *data;
		uint8_t i = 0;
		for (uint8_t m = 0x80; m; m = m >> 1) {
			gpio_set(SPI_BITBANG_MOSI, o & m);
			os_delay_us(1);
			gpio_high(SPI_BITBANG_SCLK);
			os_delay_us(1);
			i |= gpio_read(SPI_BITBANG_MISO) ? m : 0;
			gpio_low(SPI_BITBANG_SCLK);
		}
		*data = i;
		data++;
	}
	gpio_high(device->ss);

	return 0;
}

#endif
