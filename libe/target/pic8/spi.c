/*
 * pic8 SPI driver
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifdef USE_SPI

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

int spi_master_open(struct spi_master *master, void *context, uint32_t frequency, uint8_t miso, uint8_t mosi, uint8_t sclk)
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

	/* calculate clock */
	if (frequency < 1) {
		/* default to 1MHz */
		frequency = 1e6;
	}
	uint8_t sspadd = (F_CPU / frequency / 4) - 1;

	/* enable spi */
#ifdef SSPCON1
	SSPSTAT = 0x40;
	SSPCON1 = 0x2a;
	SSPCON2 = 0;
	SSPCON3 = 0;
	SSP1ADD = sspadd;
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

void spi_master_close(struct spi_master *master)
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

int spi_open(struct spi_device *device, struct spi_master *master, uint8_t ss)
{
	device->ss = ss;
	gpio_output(SPI_HARDCODED_SS);
	gpio_high(SPI_HARDCODED_SS);
	return 0;
}

void spi_close(struct spi_device *device)
{
	gpio_input(SPI_HARDCODED_SS);
}

int spi_transfer(struct spi_device *device, uint8_t *data, size_t size)
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


// void initialise_SPI(void);
// void spi_w(unsigned char data);
// void init_io(void)
// {
// 	TRISA = 0x01;
// 	TRISB = 0x01;   // bit 0 = output, 1 = input
// 	TRISC = 0b10010111;
// 	TRISD = 0x00;
// 	TRISE = 0x00;
// }
// //SPI write command function
// void spi_w(unsigned char data)
// {
// 	unsigned char x;
// 	PORTC, RC6 = 0;
// 	while (!SSPIF);  //wait for transmission complete
// 	x = SSPBUF; //dummy read
// 	SSPBUF = data;
// 	while (!SSPIF);  //wait for transmission complete
// 	x = SSPBUF;
// 	PORTC, RC6 = 1;
// }
// void initialise_SPI(void)
// {
// 	SSPEN = 0;
// 	SSPSTAT = 0b01000000;
// 	SSPCON = 0b00100010;
// 	SSPEN = 1;
// }
// void main()
// {
// 	unsigned char data = 0x05;
// 	init_io();
// 	initialise_SPI();
// 	while (1) {
// 		spi_w(data);
// 		PORTD = data;
// 	}
// }


/* internals */


#endif
