/*
 * libe cross-platform library: device communication link
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include "comm.h"
#include "debug.h"
#include "nrf.h"
#include "config.h"
#ifdef USE_FTDI
#include "cmd-common.h"
#endif


#ifdef USE_NRF
static spi_master_t comm_spi_master = NULL;
static bool comm_use_nrf = false;
#endif
#ifdef USE_BROADCAST
#include "broadcast.h"
static bool comm_use_broadcast = false;
#endif
#ifdef USE_FTDI
struct ftdi_context *comm_ftdi = NULL;
#endif


int comm_init(void)
{
	int count = 0;

#ifdef USE_NRF
	void *context = CFG_SPI_CONTEXT;
#ifdef USE_FTDI
	context = comm_ftdi = common_ftdi_init();
#endif
	comm_spi_master = spi_master_open(context, CFG_SPI_FREQUENCY, CFG_SPI_MISO, CFG_SPI_MOSI, CFG_SPI_SCLK);
	if (!comm_spi_master) {
		WARN_MSG("spi initialization failed, skipping nrf-radio messaging");
	} else if (nrf_init(comm_spi_master, CFG_NRF_SS, CFG_NRF_CE)) {
		WARN_MSG("nrf initialization failed");
	} else {
		comm_use_nrf = true;
		count++;
		NOTICE_MSG("using nrf-radio messaging");
	}
#endif

#ifdef USE_BROADCAST
	if (!broadcast_init(0)) {
		comm_use_broadcast = true;
		count++;
		NOTICE_MSG("using broadcast messaging");
	}
#endif

#ifdef USE_NRF
	/* enable radio communication */
	if (comm_use_nrf) {
		nrf_mode_rx();
		nrf_flush_rx();
		nrf_enable_radio();
	}
#endif

	/* check that atleast one communication channel was initialized */
	ERROR_IF_R(count < 1, -1, "no communication channel initialized, unable to communicate");

	return 0;
}


void comm_quit(void)
{
#ifdef USE_NRF
	if (comm_use_nrf) {
		nrf_quit();
	}
	if (comm_spi_master) {
		spi_master_close(comm_spi_master);
		comm_spi_master = NULL;
	}
#endif
#ifdef USE_BROADCAST
	if (comm_use_broadcast) {
		broadcast_quit();
	}
#endif
#ifdef USE_FTDI
	if (comm_ftdi) {
		ftdi_free(comm_ftdi);
		comm_ftdi = NULL;
	}
#endif
}


int comm_recv(void *data, int size)
{
#ifdef USE_NRF
	/* nrf first */
	if (comm_use_nrf && size >= 32) {
		int n = nrf_recv(data);
		if (n > 0) {
			return n;
		} else if (n < 0) {
			ERROR_MSG("receive failed from nrf");
		}
	}
#endif
#ifdef USE_BROADCAST
	/* broadcast */
	if (comm_use_broadcast) {
		int n = broadcast_recv(data, size);
		if (n > 0) {
			return n;
		} else if (n < 0) {
			ERROR_MSG("receive failed from broadcast");
		}
	}
#endif
	return 0;
}


int comm_send(void *data, int size)
{
#ifdef USE_NRF
	/* nrf first */
	if (comm_use_nrf && size >= 32) {
		if (nrf_send(data) < 0) {
			ERROR_MSG("send failed to nrf");
		}
	}
#endif
#ifdef USE_BROADCAST
	/* broadcast */
	if (comm_use_broadcast) {
		if (broadcast_send(data, size) < 0) {
			ERROR_MSG("send failed to broadcast");
		}
	}
#endif

	return size;
}

#ifdef USE_NRF
void comm_disable_nrf(void)
{
	comm_use_nrf = false;
}

bool comm_using_nrf(void)
{
	return comm_use_nrf;
}
#else
bool comm_using_nrf(void)
{
	return false;
}
#endif

#ifdef USE_BROADCAST
void comm_disable_broadcast(void)
{
	comm_use_broadcast = false;
}

bool comm_using_broadcast(void)
{
	return comm_use_broadcast;
}
#else
bool comm_using_broadcast(void)
{
	return false;
}
#endif
