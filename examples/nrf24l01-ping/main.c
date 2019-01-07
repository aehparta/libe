/*
 * nrf24l01+ example ping
 *
 * Run nrf24l01-listen on some other device to get responses to ping packets.
 *
 * NOTE: This example will not run properly on all platforms, example avr,
 * since os_timef() cannot be properly implemented in those smaller platforms
 */

#include <libe/os.h>
#include <libe/debug.h>
#include <libe/nrf.h>
#include "../config.h"


struct spi_master master;
struct nrf_device nrf;


int p_init(int argc, char *argv[])
{
	/* very low level platform initialization */
	os_init();
	/* debug/log init */
	log_init(NULL, 0);

	/* initialize spi master */
#ifdef USE_FTDI
	/* open ft232h type device and try to see if it has a nrf24l01+ connected to it through mpsse-spi */
	struct ftdi_context *context = ftdi_open(0x0403, 0x6014, 0, NULL, NULL, 1);
#else
	void *context = CFG_SPI_CONTEXT;
#endif
	ERROR_IF_R(spi_master_open(
	               &master, /* must give pre-allocated spi master as pointer */
	               context, /* context depends on platform */
	               CFG_SPI_FREQUENCY,
	               CFG_SPI_MISO,
	               CFG_SPI_MOSI,
	               CFG_SPI_SCLK
	           ), -1, "failed to open spi master");

	/* nrf initialization */
	ERROR_IF_R(nrf_open(&nrf, &master, CFG_NRF_SS, CFG_NRF_CE), -1, "nrf24l01+ failed to initialize");
	/* change channel, default is 70 */
	nrf_set_channel(&nrf, 7);
	/* change speed, default is 250k */
	// nrf_set_speed(&nrf, NRF_SPEED_2M);
	/* enable radio in listen mode */
	nrf_mode_rx(&nrf);
	nrf_flush_rx(&nrf);
	nrf_enable_radio(&nrf);

	return 0;
}

void p_exit(int retval)
{
	nrf_close(&nrf);
	spi_master_close(&master);
	log_quit();
	os_quit();
	exit(retval);
}

int main(int argc, char *argv[])
{
	os_time_t t = os_timef();
	int replied = 0;

	/* init */
	if (p_init(argc, argv)) {
		ERROR_MSG("initialization failed");
		p_exit(EXIT_FAILURE);
	}

	/* program loop */
	DEBUG_MSG("starting main program loop");
	while (1) {
		int i;
		uint8_t data[32];

		int ok = nrf_recv(&nrf, data);
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
				printf("pong packet received, response time: %lf ms\r\n", (double)((os_timef() - t) * 1000.0L));
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
			nrf_send(&nrf, data);
			t = os_timef();
			replied = 0;
		}

		/* lets not waste all cpu */
		os_sleepf(0.001);
	}

	p_exit(EXIT_SUCCESS);
	return EXIT_SUCCESS;
}
