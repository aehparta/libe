/*
 * nrf24l01+ example ping
 *
 * Run nrf24l01-listen on some other device to get responses to ping packets.
 *
 * NOTE: This example will not run properly on all platforms, example avr,
 * since os_timef() cannot be properly implemented in those smaller platforms
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
	ERROR_IF_R(os_ftdi_use(OS_FTDI_GPIO_0_TO_63, 0, 0, NULL, NULL), -1, "unable to open ftdi device for gpio 0-63");
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
	// nrf24l01p_set_channel(&nrf, 10);
	/* change speed, default is 250k */
	// nrf24l01p_set_speed(&nrf, NRF24L01P_SPEED_2M);
	/* disable crc, default is 2 bytes */
	// nrf24l01p_set_crc(&nrf, 0);
	/* enable radio in listen mode */
	nrf24l01p_flush_rx(&nrf);
	nrf24l01p_set_standby(&nrf, false);

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
	os_time_t t = os_timef();
	int replied = 0;

	/* init */
	if (p_init(argc, argv)) {
		ERROR_MSG("initialization failed");
		p_exit(EXIT_FAILURE);
	}

	/* program loop */
	INFO_MSG("starting main program loop");
	while (1) {
		int i;
		uint8_t data[32];

		int ok = nrf24l01p_recv(&nrf, data);
		if (ok < 0) {
			CRIT_MSG("device disconnected?");
			break;
		} else if (ok > 0) {
			/* if packet is a "pong" packet, check time */
			for (i = 0; i < 32; i++) {
				if (data[i] != (31 - i)) {
					break;
				}
			}
			if (i == 32) {
#ifdef TARGET_PIC8
				/* in pic8 printing floats is _VERY_ memory expensive */
				printf("pong packet received, response time: %u ms\r\n", (unsigned int)((os_timef() - t) * 1000.0L));
#else
				printf("pong packet received, response time: %lf ms\r\n", (double)((os_timef() - t) * 1000.0L));
#endif
				replied = 1;
			}
		}

		/* send a test "ping" packet once per second and check response time */
		if ((t + 1.0) < os_timef()) {
			if (!replied) {
				printf(LDC_REDB "timeout" LDC_DEFAULT "\r\n");
			}
			for (i = 0; i < 32; i++) {
				data[i] = i;
			}
			nrf24l01p_send(&nrf, data);
			t = os_timef();
			replied = 0;
		}

		/* lets not waste all cpu */
		os_sleepf(0.001);
	}

	p_exit(EXIT_SUCCESS);
	return EXIT_SUCCESS;
}
