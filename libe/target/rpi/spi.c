/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/spi/spidev.h>
#include <libe/spi.h>
#include <libe/debug.h>
#include <libe/os.h>

/* ftdi spi master context */
struct spi_master {
	int fd;
	uint32_t frequency;
};
/* ftdi spi device context */
struct spi_device {
	struct spi_master *m;
	uint8_t ss;
};

spi_master_t spi_master_open(void *context, uint32_t frequency, uint8_t miso, uint8_t mosi, uint8_t sclk)
{
	struct spi_master *m;
	int err;
	int fd;
	uint8_t mode = 0;
	uint8_t bits = 8;

	if (!context) {
		context = "/dev/spidev0.0";
	}
	fd = open(context, O_RDWR);
	CRIT_IF_R(fd < 0, NULL, "spi master open failed");

	/* default to 10 MHz */
	if (frequency < 1) {
		frequency = SPI_DEFAULT_FREQUENCY;
	}

	/* spi mode */
	err = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	CRIT_IF_R(err < 0, NULL, "can't set spi write mode");
	err = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	CRIT_IF_R(err < 0, NULL, "can't set spi read mode");

	/* bits per word */
	err = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	CRIT_IF_R(err < 0, NULL, "can't set bits per word for write");
	err = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	CRIT_IF_R(err < 0, NULL, "can't set bits per word for read");

	/* max speed hz */
	err = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &frequency);
	CRIT_IF_R(err < 0, NULL, "can't set write max speed");
	err = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &frequency);
	CRIT_IF_R(err < 0, NULL, "can't set read max speed");

	/* setup context */
	SALLOC(m, NULL);
	m->fd = fd;
	m->frequency = frequency;

	return (spi_master_t)m;
}

void spi_master_close(spi_master_t master)
{
	if (master) {
		struct spi_master *m = (struct spi_master *)master;
		close(m->fd);
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
	struct spi_ioc_transfer tr;

	memset(&tr, 0, sizeof(tr));
	tr.tx_buf = (unsigned long)data;
	tr.rx_buf = (unsigned long)data;
	tr.len = size;
	tr.speed_hz = d->m->frequency;
	tr.bits_per_word = 8;
	tr.cs_change = 0;

	err = ioctl(d->m->fd, SPI_IOC_MESSAGE(1), &tr);
	if (err < 1) {
		ERROR_MSG("can't send spi message");
		return -1;
	}

	return 0;
}
