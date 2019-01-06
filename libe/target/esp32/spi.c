/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#include <stdlib.h>
#include <string.h>
#include <esp_heap_caps.h>
#include <libe/spi.h>
#include <libe/debug.h>
#include <libe/os.h>

/* esp32 default spi gpio */
#ifndef ESP32_SPI_MISO
#define ESP32_SPI_MISO      GPIO_NUM_12
#endif
#ifndef ESP32_SPI_MOSI
#define ESP32_SPI_MOSI      GPIO_NUM_13
#endif
#ifndef ESP32_SPI_SCLK
#define ESP32_SPI_SCLK      GPIO_NUM_14
#endif


/* esp32 spi master context */
struct spi_master {
	spi_host_device_t host;
	uint32_t frequency;
};
/* esp32 spi device context */
struct spi_device {
	spi_device_handle_t device;
	uint8_t ss;
};


spi_master_t spi_master_open(void *context, uint32_t frequency, uint8_t miso, uint8_t mosi, uint8_t sclk)
{
	spi_bus_config_t buscfg;
	struct spi_master *m;

	SALLOC(m, NULL);
	m->host = (spi_host_device_t)context;
	m->frequency = frequency < 1 ? 10e6 : frequency;

	/* initialize the SPI bus */
	memset(&buscfg, 0, sizeof(buscfg));
	buscfg.flags = SPICOMMON_BUSFLAG_MASTER;
	buscfg.miso_io_num = miso;
	buscfg.mosi_io_num = mosi;
	buscfg.sclk_io_num = sclk;
	buscfg.quadwp_io_num = -1;
	buscfg.quadhd_io_num = -1;
	ERROR_IF_R(spi_bus_initialize(m->host, &buscfg, 1) != ESP_OK, NULL, "spi bus initialization failed");

	return (spi_master_t)m;
}

void spi_master_close(spi_master_t master)
{
	if (master) {
		struct spi_master *m = (struct spi_master *)master;
		spi_bus_free(m->host);
		free(m);
	}
}

spi_device_t spi_open(spi_master_t master, uint8_t ss)
{
	struct spi_master *m = (struct spi_master *)master;
	struct spi_device *d;
	spi_device_interface_config_t devcfg;

	/* setup context */
	SALLOC(d, NULL);

	memset(&devcfg, 0, sizeof(devcfg));
	devcfg.clock_speed_hz = m->frequency;
	devcfg.mode = 0;
	devcfg.queue_size = 1;
	devcfg.spics_io_num = ss;

	ERROR_IF_R(spi_bus_add_device(m->host, &devcfg, &d->device) != ESP_OK, NULL, "spi bus device add failed");

	return (spi_device_t)d;
}

void spi_close(spi_device_t device)
{
	struct spi_device *d = (struct spi_device *)device;
	spi_bus_remove_device(d->device);
	free(d);
}

int spi_transfer(spi_device_t device, uint8_t *data, size_t size)
{
	struct spi_device *d = (struct spi_device *)device;
	struct spi_transaction_t tr;

	/* setup data */
	memset(&tr, 0, sizeof(tr));
	tr.tx_buffer = data;
	tr.length = size * 8;
	tr.rx_buffer = data;
	tr.rxlength = size * 8;

	/* transmit */
	ERROR_IF_R(spi_device_transmit(d->device, &tr) != ESP_OK, -1, "spi transmit failed");

	return 0;
}
