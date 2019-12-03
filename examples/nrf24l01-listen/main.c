/*
 * nrf24l01+ example listener
 */

#include <libe/libe.h>
#include "../config.h"


struct spi_master master;
struct nrf24l01p_device nrf;


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
	ERROR_IF_R(nrf24l01p_open(&nrf, &master, CFG_NRF_SS, CFG_NRF_CE), -1, "nrf24l01+ failed to initialize");
	/* change channel, default is 70 */
	nrf24l01p_set_channel(&nrf, 10);
	/* change speed, default is 250k */
	nrf24l01p_set_speed(&nrf, NRF24L01P_SPEED_2M);
	/* enable radio in listen mode */
	nrf24l01p_mode_rx(&nrf);
	nrf24l01p_flush_rx(&nrf);
	nrf24l01p_enable_radio(&nrf);

	return 0;
}

void p_exit(int retval)
{
	nrf24l01p_close(&nrf);
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
	/* init */
	if (p_init(argc, argv)) {
		ERROR_MSG("initialization failed");
		p_exit(EXIT_FAILURE);
	}

	/* program loop */
	INFO_MSG("starting main program loop");
	while (1) {
		int i, ok;
		uint8_t data[32];

		ok = nrf24l01p_recv(&nrf, data);
		if (ok < 0) {
			CRIT_MSG("device disconnected?");
			break;
		} else if (ok > 0) {
			/* if packet is a "ping" packet, respond */
			for (i = 0; i < 32; i++) {
				if (data[i] != i) {
					break;
				}
			}
			if (i == 32) {
				printf("ping packet received, send reply\r\n");
				for (i = 0; i < 32; i++) {
					data[i] = 31 - i;
				}
				nrf24l01p_send(&nrf, data);
				continue;
			}

			/* print info */
			printf("packet received, data as ascii and hex dumps:\r\n");
			LOG_COLOR(LDC_DGRAYB);
			ASCII_DUMP(data, sizeof(data), 0);
			LOG_COLOR(LDC_YELLOWB);
			HEX_DUMP(data, sizeof(data));
			LOG_COLOR(LDC_DEFAULT);
		}

		/* lets not waste all cpu */
		os_sleepf(0.001);
	}

	p_exit(EXIT_SUCCESS);
	return EXIT_SUCCESS;
}
