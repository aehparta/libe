/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifdef USE_SPI

#ifdef USE_FTDI

#include <stdlib.h>
#include <string.h>
#include <libe/libe.h>


int spi_master_open(struct spi_master *master, void *context, uint32_t frequency, uint8_t miso, uint8_t mosi, uint8_t sclk)
{
	uint16_t divisor;

	/* check for mpsse mode */
	if ((sclk % 16) == 0 && (mosi - sclk) == 1 && (miso - sclk) == 2 && os_ftdi_get_mode(sclk) == BITMODE_MPSSE) {
		/* do something? */
	} else {
		ERROR_MSG("spi bitbang not supported (yet?)");
		return -1;
	}

	/* set gpio inital values and directions */
	gpio_low(miso);
	gpio_low(mosi);
	gpio_low(sclk);
	gpio_input(miso);
	gpio_output(mosi);
	gpio_output(sclk);

	/* save pins */
	master->miso = miso;
	master->mosi = mosi;
	master->sclk = sclk;

	/* calculate frequency divisor */
	if (frequency < 1) {
		/* default to 10 MHz */
		frequency = 10e6;
	}
	divisor = 60e6 / frequency / 2 - 1;

	/* setup frequency */
	uint8_t cmd[] = {
		TCK_DIVISOR, divisor & 0xff, divisor >> 8,
	};
	ERROR_IF_R(ftdi_write_data(os_ftdi_get_context(sclk), cmd, sizeof(cmd)) != sizeof(cmd), -1, "mpsse clock setup failed: %s", ftdi_get_error_string(os_ftdi_get_context(sclk)));

	return 0;
}

void spi_master_close(struct spi_master *master)
{
}

int spi_open(struct spi_device *device, struct spi_master *master, uint8_t ss)
{
	ERROR_IF_R(os_ftdi_has_pin(ss), -1, "pin %d is not available", ss);
	gpio_high(ss);
	gpio_output(ss);
	device->m = master;
	device->ss = ss;
	return 0;
}

void spi_close(struct spi_device *device)
{
}

int spi_transfer(struct spi_device *device, uint8_t *data, size_t size)
{
	struct ftdi_context *ftdi = os_ftdi_get_context(device->m->sclk);
	int err;
	uint8_t start[] = {
		MPSSE_DO_WRITE | MPSSE_WRITE_NEG | MPSSE_DO_READ,
		(size - 1) & 0xff, (size - 1) >> 8
	};

	/* select chip */
	gpio_low(device->ss);

	/* send start header */
	ERROR_IF_R(ftdi_write_data(ftdi, start, sizeof(start)) != sizeof(start), -1, "ftdi spi header write failed");
	/* write data */
	err = ftdi_write_data(ftdi, data, size);
	ERROR_IF_R(err != size, -1, "ftdi spi data write failed: %s", ftdi_get_error_string(ftdi));

	/* read data */
	for (int x = 0, err = 0; x < 3 && err == 0; x++) {
		err = ftdi_read_data(ftdi, data, size);
		// DEBUG_IF(err == 0, "try read again, ftdi_read_data() returned zero");
	}
	ERROR_IF_R(err < 0, -1, "ftdi read failed: %s", ftdi_get_error_string(ftdi));
	ERROR_IF_R(err != size, -1, "ftdi read returned different amount of data than requested, returned: %d, requested: %d", err, size);

	/* release select and push changes to chip */
	gpio_high(device->ss);

	return 0;
}

#else

/* empty spi functions */

int spi_master_open(struct spi_master *master, void *context, uint32_t frequency, uint8_t miso, uint8_t mosi, uint8_t sclk)
{
	return -1;
}

void spi_master_close(struct spi_master *master)
{

}

int spi_open(struct spi_device *device, struct spi_master *master, uint8_t ss)
{
	return -1;
}

void spi_close(struct spi_device *device)
{

}

int spi_transfer(struct spi_device *device, uint8_t *data, size_t size)
{
	return -1;
}

int spi_ftdi_set(struct spi_master *master, uint8_t pins)
{
	return -1;
}

int spi_ftdi_clr(struct spi_master *master, uint8_t pins)
{
	return -1;
}

#endif

#endif
