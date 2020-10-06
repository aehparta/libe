/*
 * pic8 SPI driver
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#include <stdlib.h>
#include <libe/libe.h>

#ifndef SPI_HARDCODED_SS
#define SPI_HARDCODED_SS (device->ss)
#endif

#ifndef SPI_HARDCODED_MISO
#define SPI_HARDCODED_MISO miso
#endif
#ifndef SPI_HARDCODED_MOSI
#define SPI_HARDCODED_MOSI mosi
#endif
#ifndef SPI_HARDCODED_SCLK
#define SPI_HARDCODED_SCLK sclk
#endif
#ifndef SPI_HARDCODED_FREQUENCY
#define SPI_HARDCODED_FREQUENCY frequency
#define SPI_HARDCODED_FREQUENCY_NOT_IN_USE
#endif

int spii_master_open(struct spi_master *master, void *context, uint32_t frequency, uint8_t miso, uint8_t mosi, uint8_t sclk)
{
	gpio_input(SPI_HARDCODED_MISO);
	gpio_output(SPI_HARDCODED_MOSI);
	gpio_output(SPI_HARDCODED_SCLK);

	/* if device supports mapping of pins */
#ifdef SSP1CLKPPS
	SSP1CLKPPS = SPI_HARDCODED_SCLK;
#endif
#ifdef SSP1DATPPS
	SSP1DATPPS = SPI_HARDCODED_MISO;
#endif
	os_pin_pps(SPI_HARDCODED_SCLK, 0x18);
	os_pin_pps(SPI_HARDCODED_MOSI, 0x19);

#ifdef SPI_HARDCODED_FREQUENCY_NOT_IN_USE
	/* calculate clock */
	if (frequency < 1) {
		/* default to 1MHz */
		frequency = 1e6;
	}
#endif
	uint8_t sspadd = (uint8_t)((F_CPU / SPI_HARDCODED_FREQUENCY / 4) - 1);

	/* enable spi */
#ifdef SSPCON1
	SSPSTAT = 0x40;
	SSPCON1 = 0x2a;
	SSPCON2 = 0;
	SSPCON3 = 0;
	SSPADD = sspadd;
#elif defined(SSP1CON1)
	SSP1STAT = 0x40;
	SSP1CON1 = 0x2a;
	SSP1CON2 = 0;
	SSP1CON3 = 0;
	SSP1ADD = sspadd;
#else
	SSPSTAT = 0x40;
	SSPCON = 0x2a;
	SSPADD = sspadd;
#endif

	return 0;
}

void spii_master_close(struct spi_master *master)
{
#ifdef SSPCON1
	SSPCON1 = 0;
#elif defined(SSP1CON1)
	SSP1CON1 = 0;
#else
	SSPCON = 0;
#endif
#ifdef SSP1STAT
	SSP1STAT = 0;
#else
	SSPSTAT = 0;
#endif
}

int spii_open(struct spi_device *device, struct spi_master *master, uint8_t ss)
{
	device->ss = ss;
	gpio_output(SPI_HARDCODED_SS);
	gpio_high(SPI_HARDCODED_SS);
	return 0;
}

void spii_close(struct spi_device *device)
{
	gpio_input(SPI_HARDCODED_SS);
}

int spii_transfer(struct spi_device *device, uint8_t *data, size_t size)
{
	gpio_low(SPI_HARDCODED_SS);
	for ( ; size > 0; size--) {
#ifdef SSP1BUF
		SSP1BUF = *data;
		while (!PIR1bits.SSP1IF);
		PIR1bits.SSP1IF = 0;
		*data = SSP1BUF;
#else
		SSPBUF = *data;
		while (!PIRbits.SSPIF);
		PIRbits.SSPIF = 0;
		*data = SSPBUF;
#endif
		data++;
	}
	gpio_high(SPI_HARDCODED_SS);

	return 0;
}
