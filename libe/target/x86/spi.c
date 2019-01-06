/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#include <stdlib.h>
#include <string.h>
#ifdef USE_FTDI
#include <libftdi1/ftdi.h>
#endif
#include <libe/spi.h>
#include <libe/debug.h>


#ifdef USE_FTDI

/* ftdi spi master context */
struct spi_master {
	struct ftdi_context *ftdi;
	uint8_t ss;
	/* pin states */
	uint8_t state;
	/* pin directions */
	uint8_t dir;
};
/* ftdi spi device context */
struct spi_device {
	struct spi_master *m;
	uint8_t ss;
};


spi_master_t spi_master_open(void *context, uint32_t frequency, uint8_t miso, uint8_t mosi, uint8_t sclk)
{
	struct spi_master *m;
	uint16_t divisor;

	/* check context */
	ERROR_IF_R(!context, NULL, "need a valid ftdi context to open spi in x86 platform");

	/* calculate frequency divisor */
	if (frequency < 1) {
		/* default to 10 MHz */
		frequency = 10e6;
	}
	divisor = 60e6 / frequency / 2 - 1;

	/* init struct */
	SALLOC(m, NULL);
	m->ftdi = (struct ftdi_context *)context;
	m->state = 0xf8;
	m->dir = 0xfb;

	/* tweak latency */
	ftdi_set_latency_timer(m->ftdi, 1);
	ftdi_write_data_set_chunksize(m->ftdi, 256);
	ftdi_read_data_set_chunksize(m->ftdi, 256);

	/* setup mpsse */
	ftdi_usb_reset(m->ftdi);
	ftdi_set_bitmode(m->ftdi, 0, BITMODE_RESET);
	ftdi_set_bitmode(m->ftdi, 0, BITMODE_MPSSE);
	ftdi_usb_purge_buffers(m->ftdi);
	uint8_t cmd[] = {
		DIS_DIV_5,
		TCK_DIVISOR, divisor & 0xff, divisor >> 8,
		DIS_ADAPTIVE,
		DIS_3_PHASE,
		SET_BITS_LOW, m->state, m->dir
	};
	ERROR_IF_R(ftdi_write_data(m->ftdi, cmd, sizeof(cmd)) != sizeof(cmd), NULL, "mpsse setup failed: %s", ftdi_get_error_string(m->ftdi));

	return (spi_master_t)m;
}

void spi_master_close(spi_master_t master)
{
	if (master) {
		struct spi_master *m = (struct spi_master *)master;
		ftdi_usb_reset(m->ftdi);
		ftdi_set_bitmode(m->ftdi, 0, BITMODE_RESET);
		free(m);
	}
}

spi_device_t spi_open(spi_master_t master, uint8_t ss)
{
	struct spi_device *d;

	/* base setup */
	SALLOC(d, NULL);
	d->m = (struct spi_master *)master;
	d->ss = ss;

	return (spi_device_t)d;
}

void spi_close(spi_device_t device)
{
	if (device) {
		struct spi_device *d = (struct spi_device *)device;
		free(d);
	}
}

int spi_transfer(spi_device_t device, uint8_t *data, size_t size)
{
	struct spi_device *d = (struct spi_device *)device;
	int err;
	uint8_t start[] = {
		SET_BITS_LOW, d->m->state & ~d->ss, d->m->dir,
		MPSSE_DO_WRITE | MPSSE_WRITE_NEG | MPSSE_DO_READ,
		(size - 1) & 0xff, (size - 1) >> 8
	};
	uint8_t end[] = { SET_BITS_LOW, d->m->state | d->ss, d->m->dir };

	/* send start header */
	ERROR_IF_R(ftdi_write_data(d->m->ftdi, start, sizeof(start)) != sizeof(start), -1, "ftdi spi header write failed");
	/* write data */
	err = ftdi_write_data(d->m->ftdi, data, size);
	ERROR_IF_R(err != size, -1, "ftdi spi data write failed: %s", ftdi_get_error_string(d->m->ftdi));

	/* read data */
	for (int x = 0, err = 0; x < 3 && err == 0; x++) {
		err = ftdi_read_data(d->m->ftdi, data, size);
		// DEBUG_IF(err == 0, "try read again, ftdi_read_data() returned zero");
	}
	ERROR_IF_R(err < 0, -1, "ftdi read failed: %s", ftdi_get_error_string(d->m->ftdi));
	ERROR_IF_R(err != size, -1, "ftdi read returned different amount of data than requested, returned: %d, requested: %d", err, size);

	/* write end */
	ERROR_IF_R(ftdi_write_data(d->m->ftdi, end, sizeof(end)) != sizeof(end), -1, "ftdi spi end write failed");

	return 0;
}

int spi_ftdi_set(spi_device_t device, uint8_t pins)
{
	struct spi_device *d = (struct spi_device *)device;
	d->m->state |= pins;
	uint8_t cmd[] = {
		SET_BITS_LOW, d->m->state, d->m->dir
	};
	if (ftdi_write_data(d->m->ftdi, cmd, sizeof(cmd)) != sizeof(cmd)) {
		return -1;
	}
	return 0;
}

int spi_ftdi_clr(spi_device_t device, uint8_t pins)
{
	struct spi_device *d = (struct spi_device *)device;
	d->m->state &= ~pins;
	uint8_t cmd[] = {
		SET_BITS_LOW, d->m->state, d->m->dir
	};
	if (ftdi_write_data(d->m->ftdi, cmd, sizeof(cmd)) != sizeof(cmd)) {
		return -1;
	}
	return 0;
}

#endif
