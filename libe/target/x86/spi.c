/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#include <stdlib.h>
#include <string.h>
#include <libe/spi.h>
#include <libe/debug.h>
#include "spi.h"


#ifdef USE_FTDI

int spi_master_open(struct spi_master *master, void *context, uint32_t frequency, uint8_t miso, uint8_t mosi, uint8_t sclk)
{
	uint16_t divisor;

	/* check context */
	ERROR_IF_R(!context, -1, "need a valid ftdi context to open spi in x86 platform");

	/* calculate frequency divisor */
	if (frequency < 1) {
		/* default to 10 MHz */
		frequency = 10e6;
	}
	divisor = 60e6 / frequency / 2 - 1;

	/* init struct */
	master->ftdi = (struct ftdi_context *)context;
	master->state = 0xf8;
	master->dir = 0xfb;

	/* tweak latency */
	ftdi_set_latency_timer(master->ftdi, 1);
	ftdi_write_data_set_chunksize(master->ftdi, 256);
	ftdi_read_data_set_chunksize(master->ftdi, 256);

	/* setup mpsse */
	ftdi_usb_reset(master->ftdi);
	ftdi_set_bitmode(master->ftdi, 0, BITMODE_RESET);
	ftdi_set_bitmode(master->ftdi, 0, BITMODE_MPSSE);
	ftdi_usb_purge_buffers(master->ftdi);
	uint8_t cmd[] = {
		DIS_DIV_5,
		TCK_DIVISOR, divisor & 0xff, divisor >> 8,
		DIS_ADAPTIVE,
		DIS_3_PHASE,
		SET_BITS_LOW, master->state, master->dir
	};
	ERROR_IF_R(ftdi_write_data(master->ftdi, cmd, sizeof(cmd)) != sizeof(cmd), -1, "mpsse setup failed: %s", ftdi_get_error_string(master->ftdi));

	return 0;
}

void spi_master_close(struct spi_master *master)
{
	ftdi_usb_reset(master->ftdi);
	ftdi_set_bitmode(master->ftdi, 0, BITMODE_RESET);
}

int spi_open(struct spi_device *device, struct spi_master *master, uint8_t ss)
{
	/* base setup */
	device->m = master;
	device->ss = ss;
	return 0;
}

void spi_close(struct spi_device *device)
{
}

int spi_transfer(struct spi_device *device, uint8_t *data, size_t size)
{
	int err;
	uint8_t start[] = {
		SET_BITS_LOW, device->m->state & ~device->ss, device->m->dir,
		MPSSE_DO_WRITE | MPSSE_WRITE_NEG | MPSSE_DO_READ,
		(size - 1) & 0xff, (size - 1) >> 8
	};
	uint8_t end[] = { SET_BITS_LOW, device->m->state | device->ss, device->m->dir };

	/* send start header */
	ERROR_IF_R(ftdi_write_data(device->m->ftdi, start, sizeof(start)) != sizeof(start), -1, "ftdi spi header write failed");
	/* write data */
	err = ftdi_write_data(device->m->ftdi, data, size);
	ERROR_IF_R(err != size, -1, "ftdi spi data write failed: %s", ftdi_get_error_string(device->m->ftdi));

	/* read data */
	for (int x = 0, err = 0; x < 3 && err == 0; x++) {
		err = ftdi_read_data(device->m->ftdi, data, size);
		// DEBUG_IF(err == 0, "try read again, ftdi_read_data() returned zero");
	}
	ERROR_IF_R(err < 0, -1, "ftdi read failed: %s", ftdi_get_error_string(device->m->ftdi));
	ERROR_IF_R(err != size, -1, "ftdi read returned different amount of data than requested, returned: %d, requested: %d", err, size);

	/* write end */
	ERROR_IF_R(ftdi_write_data(device->m->ftdi, end, sizeof(end)) != sizeof(end), -1, "ftdi spi end write failed");

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
