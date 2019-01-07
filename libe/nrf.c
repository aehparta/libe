/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#include <stdlib.h>
#include <string.h>
#ifdef TARGET_AVR
#include <avr/io.h>
#endif
#ifdef USE_FTDI
#include <libftdi1/ftdi.h>
#endif
#include "nrf.h"
#include "debug.h"
#include "os.h"


/* nrf device context */
struct nrf_device {
	struct spi_device *spi;
	uint8_t ce;
};


nrf_device_t nrf_init(spi_master_t master, int ss, int ce)
{
	// struct nrf_device *nrf = malloc();

#ifndef USE_FTDI
	os_gpio_output(ce);
#endif
	nrf_disable_radio();

	spi_handle = spi_open(master, ss);
	if (!spi_handle) {
		ERROR_MSG("failed to open spi device");
		spi_close(spi_handle);
		return -1;
	}

	nrf_setup();
	int err = nrf_read_reg(NRF_REG_CONFIG, NULL);
	ERROR_IF_R((err & 0x0f) != 0x08, -1, "nrf config register value invalid after initialization");

	return 0;
}

void nrf_quit()
{
	if (spi_handle) {
		nrf_disable_radio();
		spi_close(spi_handle);
	}
}

int nrf_simple_cmd(uint8_t command)
{
	uint8_t cmd[] = { command };
	if (spi_transfer(spi_handle, cmd, sizeof(cmd))) {
		return -1;
	}
	return cmd[0];
}

int nrf_read_status()
{
	return nrf_simple_cmd(0xff);
}

int nrf_read_reg(uint8_t reg, uint8_t *status)
{
	uint8_t cmd[] = { reg & 0x1f, 0x00 };
	if (spi_transfer(spi_handle, cmd, sizeof(cmd))) {
		return -1;
	}
	if (status) {
		*status = cmd[0];
	}
	return cmd[1];
}

int nrf_write_reg(uint8_t reg, uint8_t data)
{
	uint8_t cmd[] = { (reg & 0x1f) | 0x20, data & 0xff };
	if (spi_transfer(spi_handle, cmd, sizeof(cmd))) {
		return -1;
	}
	return cmd[0];
}

int nrf_enable_radio()
{
#ifdef USE_FTDI
	return spi_ftdi_set(spi_handle, nrf_ce);
#else
	return os_gpio_high(nrf_ce);
#endif
	return 0;
}

int nrf_disable_radio()
{
#ifdef USE_FTDI
	return spi_ftdi_clr(spi_handle, nrf_ce);
#else
	return os_gpio_low(nrf_ce);
#endif
	return 0;
}

int nrf_set_address(uint8_t pipe, uint8_t a0, uint8_t a1, uint8_t a2, uint8_t a3, uint8_t a4)
{
	if (pipe < NRF_REG_RX_ADDR_P0 && pipe > NRF_REG_TX_ADDR) {
		return -1;
	} else if (pipe >= NRF_REG_RX_ADDR_P2 && pipe <= NRF_REG_RX_ADDR_P5) {
		/* single byte secondary pipes */
		uint8_t cmd[] = { pipe | 0x20, a0 };
		if (spi_transfer(spi_handle, cmd, sizeof(cmd))) {
			return -1;
		}
		return cmd[0];
	} else {
		/* full 5 byte primary channels (0, 1 and transmit) */
		uint8_t cmd[] = { pipe | 0x20, a0, a1, a2, a3, a4 };
		if (spi_transfer(spi_handle, cmd, sizeof(cmd))) {
			return -1;
		}
		return cmd[0];
	}
	return -1;
}

void nrf_setup()
{
	nrf_disable_radio();
	/* default config */
	nrf_write_reg(NRF_REG_CONFIG, 0x08);
	/* disable autoack */
	nrf_write_reg(NRF_REG_EN_AA, 0x00);
	/* data pipes: 0 and 1 */
	nrf_write_reg(NRF_REG_EN_RXADDR, 0x03);
	/* address width: 5 bytes */
	nrf_write_reg(NRF_REG_SETUP_AW, 0x03);
	/* disable retransmission */
	nrf_write_reg(NRF_REG_SETUP_RETR, 0x20);
	/* channel 70 */
	nrf_write_reg(NRF_REG_RF_CH, 0x46);
	/* 250 kbps, full power */
	nrf_write_reg(NRF_REG_RF_SETUP, 0x27);
	/* pipe 0 size */
	nrf_write_reg(NRF_REG_RX_PW_P0, 32);
	/* pipe 1 size */
	nrf_write_reg(NRF_REG_RX_PW_P1, 32);
	/* pipe 0 address */
	nrf_set_address(NRF_REG_RX_ADDR_P0, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7);
	/* pipe 1 address */
	nrf_set_address(NRF_REG_RX_ADDR_P1, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2);
	/* transmit address */
	nrf_set_address(NRF_REG_TX_ADDR, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7);
	/* reset these */
	nrf_write_reg(NRF_REG_DYNPD, 0);
	nrf_write_reg(NRF_REG_FEATURE, 0);
}

int nrf_mode_tx()
{
	return nrf_write_reg(NRF_REG_CONFIG, 0x0e);
}

int nrf_mode_rx()
{
	return nrf_write_reg(NRF_REG_CONFIG, 0x0f);
}

int nrf_flush_tx()
{
	int err = nrf_simple_cmd(0xe1);
	nrf_write_reg(NRF_REG_STATUS, 0x30);
	return err;
}

int nrf_flush_rx()
{
	int err = nrf_simple_cmd(0xe2);
	nrf_write_reg(NRF_REG_STATUS, 0x40);
	return err;
}

void nrf_set_power(uint8_t power)
{
	nrf_write_reg(NRF_REG_RF_SETUP, 0x21 | ((power & 0x3) << 1));
}

int nrf_recv(void *data)
{
	/* check status */
	int status = nrf_read_status();
	// DEBUG_MSG("%02x", status);
	if (status < 0) {
		ERROR_MSG("nrf_read_status() failed");
		return -1;
	}
	if ((status & 0xe) == 0xe) {
		return 0;
	}
	/* has data */
	uint8_t cmd[33] = {
		/* rx buffer read command */
		0x61,
	};
	if (spi_transfer(spi_handle, cmd, sizeof(cmd))) {
		ERROR_MSG("nrf spi receive transfer failed");
		return -1;
	}
	memcpy(data, cmd + 1, 32);
	nrf_flush_rx();
	return 32;
}

int nrf_send(void *data)
{
	/* write data to tx buffer first */
	uint8_t cmd[33] = {
		/* tx buffer write command */
		0xa0,
	};
	memcpy(cmd + 1, data, 32);
	if (spi_transfer(spi_handle, cmd, sizeof(cmd))) {
		ERROR_MSG("nrf spi send transfer failed");
		return -1;
	}
	/* switch to transmit mode */
	nrf_disable_radio();
	nrf_mode_tx();
	nrf_enable_radio();
	/* wait data to be transmitted */
	while (!(nrf_read_status() & 0x20));
	/* switch back to listen mode */
	nrf_disable_radio();
	nrf_flush_tx();
	nrf_mode_rx();
	nrf_enable_radio();
	return 32;
}
