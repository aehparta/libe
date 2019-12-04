/*
 * nrf24l01+ example ble advertiser
 *
 * Send BLE packets using nrf24l01+.
 */

#include <libe/libe.h>
#include "../config.h"


struct spi_master master;
struct nrf24l01p_ble_device nrf;
uint8_t mac[6] = { 'L', 'I', 'B', 'E', 'B', 'T' };


int p_init(int argc, char *argv[])
{
	/* very low level platform initialization */
	os_init();
	/* debug/log init */
	log_init();

	/* initialize spi master */
#ifdef USE_FTDI
	/* open ft232h type device and try to see if it has a nrf24l01+ connected to it through mpsse-spi */
	ERROR_IF_R(os_ftdi_use(OS_FTDI_GPIO_0_TO_63, 0x0403, 0x6014, NULL, NULL), -1, "unable to open ftdi device for gpio 0-63");
	os_ftdi_set_mpsse(CFG_SPI_SCLK);
#endif
	ERROR_IF_R(spi_master_open(
	               &master, /* must give pre-allocated spi master as pointer */
	               CFG_SPI_CONTEXT, /* context depends on platform */
	               CFG_SPI_FREQUENCY,
	               CFG_SPI_MISO,
	               CFG_SPI_MOSI,
	               CFG_SPI_SCLK
	           ), -1, "failed to open spi master");

	/* nrf initialization */
	ERROR_IF_R(nrf24l01p_ble_open(&nrf, &master, CFG_NRF_SS, CFG_NRF_CE, mac), -1, "nrf24l01+ failed to initialize");

	return 0;
}

void p_exit(int retval)
{
	nrf24l01p_ble_close(&nrf);
	spi_master_close(&master);
	log_quit();
	os_quit();
	exit(retval);
}

#ifdef TARGET_ESP32
int app_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	// uint8_t data[32] = { 0xb3, 0xc3, 0x1d, 0x7a, 0xbf, 0xb0, 0x7e, 0x0d, 0xee, 0x0c, 0x28, 0xf2, 0x79, 0x98, 0x55, 0xa5, 0x66, 0xa5, 0x77, 0x6b, 0xda, 0xf5 };

	/* init */
	if (p_init(argc, argv)) {
		ERROR_MSG("initialization failed");
		p_exit(EXIT_FAILURE);
	}

	/* program loop */
	INFO_MSG("starting main program loop");
	while (1) {
		uint8_t l = 0;
		uint8_t buf[18];
		memset(buf, 0, sizeof(buf));
		buf[l++] = 5;
		buf[l++] = 0x08;
		buf[l++] = 'l';
		buf[l++] = 'i';
		buf[l++] = 'b';
		buf[l++] = 'e';
		nrf24l01p_ble_adv(&nrf, buf, l);
		nrf24l01p_ble_hop(&nrf);

		/* lets not waste all cpu */
		os_sleepf(1.0);
	}

	p_exit(EXIT_SUCCESS);
	return EXIT_SUCCESS;
}
