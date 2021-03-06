/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifdef USE_DRIVER_NRF24L01P /* because esp32 */

#include <stdlib.h>
#include <string.h>
#include <libe/libe.h>

#ifndef NRF24L01P_HARDCODED_CE
#define NRF24L01P_HARDCODED_CE (nrf->ce)
#endif


int8_t nrf24l01p_open(struct nrf24l01p_device *nrf, struct spi_master *master, uint8_t ss, uint8_t ce)
{
	nrf->ce = ce;
	gpio_output(NRF24L01P_HARDCODED_CE);
	gpio_low(NRF24L01P_HARDCODED_CE);

#ifdef NRF24L01P_HARDCODED_IRQ
	gpio_input(NRF24L01P_HARDCODED_IRQ);
#endif

	if (spi_open(&nrf->spi, master, ss)) {
		spi_close(&nrf->spi);
		return -1;
	}

	/* base setup */
	nrf24l01p_set_standby(nrf, true);
	/* default config: enable crc (2 bytes), power down, rx mode, mask interrupts except tx */
	nrf24l01p_write_reg(nrf, NRF24L01P_REG_CONFIG, 0x5d);
	/* disable autoack */
	nrf24l01p_write_reg(nrf, NRF24L01P_REG_EN_AA, 0x00);
	/* data pipes: 0 and 1 */
	nrf24l01p_write_reg(nrf, NRF24L01P_REG_EN_RXADDR, 0x03);
	/* address width: 5 bytes */
	nrf24l01p_write_reg(nrf, NRF24L01P_REG_SETUP_AW, 0x03);
	/* disable retransmission */
	nrf24l01p_write_reg(nrf, NRF24L01P_REG_SETUP_RETR, 0x00);
	/* channel 70 */
	nrf24l01p_write_reg(nrf, NRF24L01P_REG_RF_CH, 70);
	/* 250 kbps, full power */
	nrf24l01p_write_reg(nrf, NRF24L01P_REG_RF_SETUP, 0x26);
	/* pipe 0 size */
	nrf24l01p_write_reg(nrf, NRF24L01P_REG_RX_PW_P0, 32);
	/* pipe 1 size */
	nrf24l01p_write_reg(nrf, NRF24L01P_REG_RX_PW_P1, 32);
	/* pipe 0 address */
	nrf24l01p_set_address(nrf, NRF24L01P_REG_RX_ADDR_P0, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7);
	/* pipe 1 address */
	nrf24l01p_set_address(nrf, NRF24L01P_REG_RX_ADDR_P1, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2);
	/* transmit address */
	nrf24l01p_set_address(nrf, NRF24L01P_REG_TX_ADDR, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7);
	/* reset these */
	nrf24l01p_write_reg(nrf, NRF24L01P_REG_DYNPD, 0);
	nrf24l01p_write_reg(nrf, NRF24L01P_REG_FEATURE, 0);

	/* wait a bit */
	// os_delay_ms(100);

	/* check status */
	IF_R((nrf24l01p_read_reg(nrf, NRF24L01P_REG_CONFIG, NULL) & 0x0f) != 0x0d, -1);
	nrf24l01p_set_power_down(nrf, false);
	nrf24l01p_set_standby(nrf, false);
	nrf24l01p_flush_rx(nrf);
	nrf24l01p_flush_tx(nrf);

	return 0;
}

void nrf24l01p_close(struct nrf24l01p_device *nrf)
{
	nrf24l01p_set_standby(nrf, true);
	nrf24l01p_set_power_down(nrf, true);
	spi_close(&nrf->spi);
}

uint8_t nrf24l01p_simple_cmd(struct nrf24l01p_device *nrf, uint8_t command)
{
	uint8_t cmd[] = { command };
	IF_R(spi_transfer(&nrf->spi, cmd, sizeof(cmd)), 0xff);
	return cmd[0];
}

int8_t nrf24l01p_read_status(struct nrf24l01p_device *nrf)
{
	return (int8_t)nrf24l01p_simple_cmd(nrf, 0xff);
}

uint8_t nrf24l01p_read_reg(struct nrf24l01p_device *nrf, uint8_t reg, uint8_t *status)
{
	uint8_t cmd[] = { reg & 0x1f, 0x00 };
	IF_R(spi_transfer(&nrf->spi, cmd, sizeof(cmd)), 0xff);
	if (status) {
		*status = cmd[0];
	}
	return cmd[1];
}

uint8_t nrf24l01p_write_reg(struct nrf24l01p_device *nrf, uint8_t reg, uint8_t data)
{
	uint8_t cmd[] = { (reg & 0x1f) | 0x20, data };
	IF_R(spi_transfer(&nrf->spi, cmd, sizeof(cmd)), 0xff);
	return cmd[0];
}

uint8_t nrf24l01p_mode_tx(struct nrf24l01p_device *nrf)
{
	uint8_t r = nrf24l01p_read_reg(nrf, NRF24L01P_REG_CONFIG, NULL) & 0xfe;
	return nrf24l01p_write_reg(nrf, NRF24L01P_REG_CONFIG, r);
}

uint8_t nrf24l01p_mode_rx(struct nrf24l01p_device *nrf)
{
	uint8_t r = nrf24l01p_read_reg(nrf, NRF24L01P_REG_CONFIG, NULL) | 0x01;
	return nrf24l01p_write_reg(nrf, NRF24L01P_REG_CONFIG, r);
}

uint8_t nrf24l01p_flush_tx(struct nrf24l01p_device *nrf)
{
	uint8_t err = nrf24l01p_simple_cmd(nrf, 0xe1);
	nrf24l01p_write_reg(nrf, NRF24L01P_REG_STATUS, 0x30);
	return err;
}

uint8_t nrf24l01p_flush_rx(struct nrf24l01p_device *nrf)
{
	uint8_t err = nrf24l01p_simple_cmd(nrf, 0xe2);
	nrf24l01p_write_reg(nrf, NRF24L01P_REG_STATUS, 0x40);
	return err;
}

uint8_t nrf24l01p_set_address(struct nrf24l01p_device *nrf, uint8_t pipe, uint8_t a0, uint8_t a1, uint8_t a2, uint8_t a3, uint8_t a4)
{
	if (pipe < NRF24L01P_REG_RX_ADDR_P0 && pipe > NRF24L01P_REG_TX_ADDR) {
		return 0xff;
	} else if (pipe >= NRF24L01P_REG_RX_ADDR_P2 && pipe <= NRF24L01P_REG_RX_ADDR_P5) {
		/* single byte secondary pipes */
		uint8_t cmd[] = { pipe | 0x20, a0 };
		IF_R(spi_transfer(&nrf->spi, cmd, sizeof(cmd)), 0xff);
		return cmd[0];
	} else {
		/* full 5 byte primary channels (0, 1 and transmit) */
		uint8_t cmd[] = { pipe | 0x20, a0, a1, a2, a3, a4 };
		IF_R(spi_transfer(&nrf->spi, cmd, sizeof(cmd)), 0xff);
		return cmd[0];
	}
	return 0xff;
}

uint8_t nrf24l01p_set_standby(struct nrf24l01p_device *nrf, bool standby)
{
	if (standby) {
		gpio_low(NRF24L01P_HARDCODED_CE);
	} else {
		gpio_high(NRF24L01P_HARDCODED_CE);
	}
	return 0;
}

uint8_t nrf24l01p_set_power_down(struct nrf24l01p_device *nrf, bool pd)
{
	uint8_t r = (nrf24l01p_read_reg(nrf, NRF24L01P_REG_CONFIG, NULL) & 0xfd) | (pd ? 0x00 : 0x02);
	return nrf24l01p_write_reg(nrf, NRF24L01P_REG_CONFIG, r);
}

uint8_t nrf24l01p_set_tx_power(struct nrf24l01p_device *nrf, uint8_t power)
{
	uint8_t r = nrf24l01p_read_reg(nrf, NRF24L01P_REG_RF_SETUP, NULL) & ~0x6;
	return nrf24l01p_write_reg(nrf, NRF24L01P_REG_RF_SETUP, r | (uint8_t)((power & 0x3) << 1));
}

uint8_t nrf24l01p_set_channel(struct nrf24l01p_device *nrf, uint8_t channel)
{
	return nrf24l01p_write_reg(nrf, NRF24L01P_REG_RF_CH, channel);
}

uint8_t nrf24l01p_set_speed(struct nrf24l01p_device *nrf, uint8_t speed)
{
	uint8_t r = nrf24l01p_read_reg(nrf, NRF24L01P_REG_RF_SETUP, NULL) & ~0x28;
	if (speed == NRF24L01P_SPEED_250k) {
		r |= 0x20;
	} else if (speed == NRF24L01P_SPEED_2M) {
		r |= 0x08;
	}
	return nrf24l01p_write_reg(nrf, NRF24L01P_REG_RF_SETUP, r);
}

uint8_t nrf24l01p_set_crc(struct nrf24l01p_device *nrf, uint8_t crc)
{
	uint8_t r = (nrf24l01p_read_reg(nrf, NRF24L01P_REG_CONFIG, NULL) & 0xfd) | (crc ? 0x08 : 0x00) | (crc == 2 ? 0x04 : 0x00);
	return nrf24l01p_write_reg(nrf, NRF24L01P_REG_CONFIG, r);
}

int8_t nrf24l01p_recv(struct nrf24l01p_device *nrf, void *data)
{
	/* check status */
	int8_t status = nrf24l01p_read_status(nrf);

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
	IF_R(spi_transfer(&nrf->spi, cmd, sizeof(cmd)), -1);
	memcpy(data, cmd + 1, 32);
	nrf24l01p_flush_rx(nrf);
	return 32;
}

int8_t nrf24l01p_send(struct nrf24l01p_device *nrf, void *data)
{
	/* switch to transmit mode */
	nrf24l01p_set_standby(nrf, true);
	nrf24l01p_mode_tx(nrf);
	nrf24l01p_set_standby(nrf, false);
	/* write data to tx buffer first */
	IF_R(nrf24l01p_tx_wr(nrf, data) < 0, -1);
	/* switch back to listen mode */
	nrf24l01p_set_standby(nrf, true);
	nrf24l01p_mode_rx(nrf);
	nrf24l01p_set_standby(nrf, false);
	return 32;
}

int8_t nrf24l01p_tx_wr(struct nrf24l01p_device *nrf, void *data)
{
	uint8_t cmd[33];

	/* wait for empty tx fifo */
	// do {
	// 	cmd[0] = 0xff;
	// 	spi_transfer(&nrf->spi, cmd, 1);
	// } while (cmd[0] & 0x01);

	/* transfer data */
	cmd[0] = 0xa0; /* write tx buffer */
	memcpy(cmd + 1, data, 32);
	IF_R(spi_transfer(&nrf->spi, cmd, sizeof(cmd)), -1);

	/* wait for send */
#ifdef NRF24L01P_HARDCODED_IRQ
	while (gpio_read(NRF24L01P_HARDCODED_IRQ));
#else
	do {
		cmd[0] = 0xff;
		spi_transfer(&nrf->spi, cmd, 1);
	} while (!(cmd[0] & 0x20));
#endif
	/* clear interrupt */
	nrf24l01p_write_reg(nrf, NRF24L01P_REG_STATUS, 0x20);

	return 32;
}

#endif
