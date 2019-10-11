/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifdef USE_SPI

#include <stdlib.h>
#include <string.h>
#include <esp_heap_caps.h>
#include <libe/libe.h>


int spi_master_open(struct spi_master *master, void *context, uint32_t frequency, uint8_t miso, uint8_t mosi, uint8_t sclk)
{
	spi_bus_config_t buscfg;

	master->host = (spi_host_device_t)context;
	master->frequency = frequency < 1 ? 10e6 : frequency;

	/* initialize the SPI bus */
	memset(&buscfg, 0, sizeof(buscfg));
	buscfg.flags = SPICOMMON_BUSFLAG_MASTER;
	buscfg.miso_io_num = miso;
	buscfg.mosi_io_num = mosi;
	buscfg.sclk_io_num = sclk;
	buscfg.quadwp_io_num = -1;
	buscfg.quadhd_io_num = -1;
	if (spi_bus_initialize(master->host, &buscfg, 1) != ESP_OK) {
		error_last = "spi bus initialization failed";
		return -1;
	}

	return 0;
}

void spi_master_close(struct spi_master *master)
{
	spi_bus_free(master->host);
}

int spi_open(struct spi_device *device, struct spi_master *master, uint8_t ss)
{
	spi_device_interface_config_t devcfg;

	memset(&devcfg, 0, sizeof(devcfg));
	devcfg.clock_speed_hz = master->frequency;
	devcfg.mode = 0;
	devcfg.queue_size = 1;
	devcfg.spics_io_num = ss;

	if (spi_bus_add_device(master->host, &devcfg, &device->device) != ESP_OK) {
		error_last = "spi bus device add failed";
	}

	return 0;
}

void spi_close(struct spi_device *device)
{
	spi_bus_remove_device(device->device);
}

int spi_transfer(struct spi_device *device, uint8_t *data, size_t size)
{
	struct spi_transaction_t tr;

	/* setup data */
	memset(&tr, 0, sizeof(tr));
	tr.tx_buffer = data;
	tr.length = size * 8;
	tr.rx_buffer = data;
	tr.rxlength = size * 8;

	/* transmit */
	if (spi_device_transmit(device->device, &tr) != ESP_OK) {
		error_last = "spi transmit failed";
		return -1;
	}

	return 0;
}

#endif
