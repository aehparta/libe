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
#include <libe/libe.h>


int spidev_master_open(struct spi_master *master, void *context, uint32_t frequency, uint8_t miso, uint8_t mosi, uint8_t sclk)
{
	int err;
	int fd;
	uint8_t mode = 0;
	uint8_t bits = 8;

	/* set master fd to invalid at start */
	master->fd = -1;

	if (!context) {
#ifdef TARGET_OPI
		context = "/dev/spidev1.0";
#else
		context = "/dev/spidev0.0";
#endif
	}
	fd = open(context, O_RDWR);
	CRIT_IF_R(fd < 0, -1, "spi master open failed");

	/* default to 10 MHz */
	if (frequency < 1) {
		frequency = SPIDEV_DEFAULT_FREQUENCY;
	}

	/* spi mode */
	err = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	CRIT_IF_R(err < 0, -1, "can't set spi write mode");
	err = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	CRIT_IF_R(err < 0, -1, "can't set spi read mode");

	/* bits per word */
	err = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	CRIT_IF_R(err < 0, -1, "can't set bits per word for write");
	err = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	CRIT_IF_R(err < 0, -1, "can't set bits per word for read");

	/* max speed hz */
	err = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &frequency);
	CRIT_IF_R(err < 0, -1, "can't set write max speed");
	err = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &frequency);
	CRIT_IF_R(err < 0, -1, "can't set read max speed");

	/* setup context */
	master->fd = fd;
	master->frequency = frequency;

	return 0;
}

void spidev_master_close(struct spi_master *master)
{
	if (master->fd >= 0) {
		close(master->fd);
	}
}

int spidev_open(struct spi_device *device, struct spi_master *master, uint8_t ss)
{
	/* base setup */
	device->master = master;
	device->ss = ss;
	return 0;
}

void spidev_close(struct spi_device *device)
{
}

int spidev_transfer(struct spi_device *device, uint8_t *data, size_t size)
{
	int err;
	struct spi_ioc_transfer tr;

	memset(&tr, 0, sizeof(tr));
	tr.tx_buf = (unsigned long)data;
	tr.rx_buf = (unsigned long)data;
	tr.len = size;
	tr.speed_hz = device->master->frequency;
	tr.bits_per_word = 8;
	tr.cs_change = 0;

	err = ioctl(device->master->fd, SPI_IOC_MESSAGE(1), &tr);
	if (err < 1) {
		ERROR_MSG("can't send spi message");
		return -1;
	}

	return 0;
}
