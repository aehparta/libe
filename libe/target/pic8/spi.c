/*
 * pic8 SPI driver
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifdef USE_SPI

#include <stdlib.h>
#include <libe/libe.h>


int spi_master_open(struct spi_master *master, void *context, uint32_t frequency, uint8_t miso, uint8_t mosi, uint8_t sclk)
{
#ifdef SSP1STAT
	SSP1STAT = 0;
#else
	SSPSTAT = 0;
#endif
#ifdef SSPCON1
	SSPCON1 = 0x20;
#elif defined(SSP1CON1)
	SSP1CON1 = 0x20;
#else
	SSPCON = 0x20;
#endif
	os_gpio_input(miso);
	os_gpio_output(mosi);
	os_gpio_output(sclk);

	/* if device supports mapping of pins */
#ifdef SSP1DATPPS
	SSP1DATPPS = miso;
#endif
#ifdef SSP1CLKPPS
	SSP1CLKPPS = sclk;
#endif
	os_pin_pps(sclk, 0x13);
	os_pin_pps(mosi, 0x14);

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
#ifdef SSP1BUF
		SSP1BUF = *data;
		while (!SSP1STATbits.BF);
		*data = SSP1BUF;
#else
		SSPBUF = *data;
		while (!SSPSTATbits.BF);
		*data = SSPBUF;
#endif
		data++;
	}
	os_gpio_high(device->ss);

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

#endif
