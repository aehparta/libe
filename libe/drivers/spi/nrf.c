/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#include <stdlib.h>
#include <string.h>
#include <libe/log.h>
#include <libe/os.h>
#include "nrf.h"


int nrf_open(struct nrf_device *nrf, struct spi_master *master, int ss, int ce)
{
	os_gpio_low(ce);
	os_gpio_output(ce);
	nrf->ce = ce;

	if (spi_open(&nrf->spi, master, ss)) {
		spi_close(&nrf->spi);
		return -1;
	}

	while (1) {
		uint8_t x = 0x5a;
		spi_transfer(&nrf->spi, &x, 1);
		printf("%02x\r\n", x);
		os_delay_ms(100);
	}


	nrf_setup(nrf);
	IF_R((nrf_read_reg(nrf, NRF_REG_CONFIG, NULL) & 0x0f) != 0x08, -1);

	return 0;
}

void nrf_close(struct nrf_device *nrf)
{
	nrf_disable_radio(nrf);
	spi_close(&nrf->spi);
}

int nrf_simple_cmd(struct nrf_device *nrf, uint8_t command)
{
	uint8_t cmd[] = { command };
	if (spi_transfer(&nrf->spi, cmd, sizeof(cmd))) {
		return -1;
	}
	return cmd[0];
}

int nrf_read_status(struct nrf_device *nrf)
{
	return nrf_simple_cmd(nrf, 0xff);
}

int nrf_read_reg(struct nrf_device *nrf, uint8_t reg, uint8_t *status)
{
	uint8_t cmd[] = { reg & 0x1f, 0x00 };
	if (spi_transfer(&nrf->spi, cmd, sizeof(cmd))) {
		return -1;
	}
	if (status) {
		*status = cmd[0];
	}
	return cmd[1];
}

int nrf_write_reg(struct nrf_device *nrf, uint8_t reg, uint8_t data)
{
	uint8_t cmd[] = { (reg & 0x1f) | 0x20, data & 0xff };
	if (spi_transfer(&nrf->spi, cmd, sizeof(cmd))) {
		return -1;
	}
	return cmd[0];
}

int nrf_enable_radio(struct nrf_device *nrf)
{
	return os_gpio_high(nrf->ce);
}

int nrf_disable_radio(struct nrf_device *nrf)
{
	return os_gpio_low(nrf->ce);
}

int nrf_set_address(struct nrf_device *nrf, uint8_t pipe, uint8_t a0, uint8_t a1, uint8_t a2, uint8_t a3, uint8_t a4)
{
	if (pipe < NRF_REG_RX_ADDR_P0 && pipe > NRF_REG_TX_ADDR) {
		return -1;
	} else if (pipe >= NRF_REG_RX_ADDR_P2 && pipe <= NRF_REG_RX_ADDR_P5) {
		/* single byte secondary pipes */
		uint8_t cmd[] = { pipe | 0x20, a0 };
		if (spi_transfer(&nrf->spi, cmd, sizeof(cmd))) {
			return -1;
		}
		return cmd[0];
	} else {
		/* full 5 byte primary channels (0, 1 and transmit) */
		uint8_t cmd[] = { pipe | 0x20, a0, a1, a2, a3, a4 };
		if (spi_transfer(&nrf->spi, cmd, sizeof(cmd))) {
			return -1;
		}
		return cmd[0];
	}
	return -1;
}

void nrf_setup(struct nrf_device *nrf)
{
	nrf_disable_radio(nrf);
	/* default config */
	nrf_write_reg(nrf, NRF_REG_CONFIG, 0x08);
	/* disable autoack */
	nrf_write_reg(nrf, NRF_REG_EN_AA, 0x00);
	/* data pipes: 0 and 1 */
	nrf_write_reg(nrf, NRF_REG_EN_RXADDR, 0x03);
	/* address width: 5 bytes */
	nrf_write_reg(nrf, NRF_REG_SETUP_AW, 0x03);
	/* disable retransmission */
	nrf_write_reg(nrf, NRF_REG_SETUP_RETR, 0x20);
	/* channel 70 */
	nrf_write_reg(nrf, NRF_REG_RF_CH, 70);
	/* 250 kbps, full power */
	nrf_write_reg(nrf, NRF_REG_RF_SETUP, 0x26);
	/* pipe 0 size */
	nrf_write_reg(nrf, NRF_REG_RX_PW_P0, 32);
	/* pipe 1 size */
	nrf_write_reg(nrf, NRF_REG_RX_PW_P1, 32);
	/* pipe 0 address */
	nrf_set_address(nrf, NRF_REG_RX_ADDR_P0, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7);
	/* pipe 1 address */
	nrf_set_address(nrf, NRF_REG_RX_ADDR_P1, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2);
	/* transmit address */
	nrf_set_address(nrf, NRF_REG_TX_ADDR, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7);
	/* reset these */
	nrf_write_reg(nrf, NRF_REG_DYNPD, 0);
	nrf_write_reg(nrf, NRF_REG_FEATURE, 0);
}

int nrf_mode_tx(struct nrf_device *nrf)
{
	return nrf_write_reg(nrf, NRF_REG_CONFIG, 0x0e);
}

int nrf_mode_rx(struct nrf_device *nrf)
{
	return nrf_write_reg(nrf, NRF_REG_CONFIG, 0x0f);
}

int nrf_flush_tx(struct nrf_device *nrf)
{
	int err = nrf_simple_cmd(nrf, 0xe1);
	nrf_write_reg(nrf, NRF_REG_STATUS, 0x30);
	return err;
}

int nrf_flush_rx(struct nrf_device *nrf)
{
	int err = nrf_simple_cmd(nrf, 0xe2);
	nrf_write_reg(nrf, NRF_REG_STATUS, 0x40);
	return err;
}

void nrf_set_power(struct nrf_device *nrf, uint8_t power)
{
	uint8_t r = nrf_read_reg(nrf, NRF_REG_RF_SETUP, NULL) & ~0x6;
	nrf_write_reg(nrf, NRF_REG_RF_SETUP, r | ((power & 0x3) << 1));
}

void nrf_set_channel(struct nrf_device *nrf, uint8_t channel)
{
	nrf_write_reg(nrf, NRF_REG_RF_CH, channel);
}

void nrf_set_speed(struct nrf_device *nrf, uint8_t speed)
{
	uint8_t r = nrf_read_reg(nrf, NRF_REG_RF_SETUP, NULL) & ~0x28;
	if (speed == NRF_SPEED_250k) {
		r |= 0x20;
	} else if (speed == NRF_SPEED_2M) {
		r |= 0x08;
	}
	nrf_write_reg(nrf, NRF_REG_RF_SETUP, r);
}

int nrf_recv(struct nrf_device *nrf, void *data)
{
	/* check status */
	int status = nrf_read_status(nrf);

	if (status < 0) {
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
	if (spi_transfer(&nrf->spi, cmd, sizeof(cmd))) {
		return -1;
	}
	memcpy(data, cmd + 1, 32);
	nrf_flush_rx(nrf);
	return 32;
}

int nrf_send(struct nrf_device *nrf, void *data)
{
	/* write data to tx buffer first */
	uint8_t cmd[33] = {
		/* tx buffer write command */
		0xa0,
	};
	memcpy(cmd + 1, data, 32);
	if (spi_transfer(&nrf->spi, cmd, sizeof(cmd))) {
		return -1;
	}
	/* switch to transmit mode */
	nrf_disable_radio(nrf);
	nrf_mode_tx(nrf);
	nrf_enable_radio(nrf);
	/* wait data to be transmitted */
	while (!(nrf_read_status(nrf) & 0x20));
	/* switch back to listen mode */
	nrf_disable_radio(nrf);
	nrf_flush_tx(nrf);
	nrf_mode_rx(nrf);
	nrf_enable_radio(nrf);
	return 32;
}
