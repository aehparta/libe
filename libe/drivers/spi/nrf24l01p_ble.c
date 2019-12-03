/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#include <stdlib.h>
#include <libe/libe.h>

#define BYTE_SWAP_BITS(b) (((b << 7) & 0x80) | ((b << 5) & 0x40) | ((b << 3) & 0x20) | ((b << 1) & 0x10) | ((b >> 1) & 0x08) | ((b >> 3) & 0x04) | ((b >> 5) & 0x02) | ((b >> 7) & 0x01))
static void btLeCrc(uint8_t* data, uint8_t size, uint8_t* dst);
static void btLeWhiten(uint8_t* data, uint8_t size, uint8_t whitenCoeff);


int nrf24l01p_ble_open(struct nrf24l01p_ble_device *nrf, struct spi_master *master, int ss, int ce, uint8_t mac[6])
{
	IF_R(nrf24l01p_open(&nrf->nrf, master, ss, ce), -1);
	/* disable crc and always stay in transmit mode, nothing can be received anyways */
	nrf24l01p_write_reg(&nrf->nrf, NRF24L01P_REG_CONFIG, 0x02);
	/* address width: 4 bytes */
	nrf24l01p_write_reg(&nrf->nrf, NRF24L01P_REG_SETUP_AW, 0x02);
	/* 1 Mbpsm */
	nrf24l01p_write_reg(&nrf->nrf, NRF24L01P_REG_RF_SETUP, 0x06);
	/* receiving data pipes: 0 */
	nrf24l01p_write_reg(&nrf->nrf, NRF24L01P_REG_EN_RXADDR, 0x01);
	/* pipe 0 address */
	nrf24l01p_set_address(&nrf->nrf, NRF24L01P_REG_RX_ADDR_P0, BYTE_SWAP_BITS(0x8e), BYTE_SWAP_BITS(0x89), BYTE_SWAP_BITS(0xbe), BYTE_SWAP_BITS(0xd6), 0x00);
	/* transmit address */
	nrf24l01p_set_address(&nrf->nrf, NRF24L01P_REG_TX_ADDR, BYTE_SWAP_BITS(0x8e), BYTE_SWAP_BITS(0x89), BYTE_SWAP_BITS(0xbe), BYTE_SWAP_BITS(0xd6), 0x00);
	/* default channel */
	nrf24l01p_ble_set_channel(nrf, 0);

	/* save mac */
	memcpy(&nrf->mac, mac, 6);

	return 0;
}

void nrf24l01p_ble_close(struct nrf24l01p_ble_device *nrf)
{
	nrf24l01p_close(&nrf->nrf);
}

void nrf24l01p_ble_set_channel(struct nrf24l01p_ble_device *nrf, uint8_t channel)
{
	switch (channel) {
	case 0:
		nrf24l01p_set_channel(&nrf->nrf, 2);
		nrf->channel_whiten = BYTE_SWAP_BITS(37) | 2;
		break;
	case 1:
		nrf24l01p_set_channel(&nrf->nrf, 26);
		nrf->channel_whiten = BYTE_SWAP_BITS(38) | 2;
		break;
	case 2:
		nrf24l01p_set_channel(&nrf->nrf, 80);
		nrf->channel_whiten = BYTE_SWAP_BITS(39) | 2;
		break;
	}
}

void nrf24l01p_ble_hop(struct nrf24l01p_ble_device *nrf)
{
	switch (nrf->channel_whiten) {
	case (BYTE_SWAP_BITS(37) | 2):
		nrf24l01p_ble_set_channel(nrf, 1);
		break;
	case (BYTE_SWAP_BITS(38) | 2):
		nrf24l01p_ble_set_channel(nrf, 2);
		break;
	default:
	case (BYTE_SWAP_BITS(39) | 2):
		nrf24l01p_ble_set_channel(nrf, 0);
		break;
	}
}

int nrf24l01p_ble_adv_single(struct nrf24l01p_ble_device *nrf, uint8_t type, void *data, uint8_t size)
{
	uint8_t l = 0;
	uint8_t cmd[33];
	memset(cmd, 0, sizeof(cmd));

	/* tx buffer write command */
	cmd[l++] = 0xa0;
	/* we use 0x40 to say it is a non-connectable undirected advertisement and address we're sending is random (not assigned) */
	cmd[l++] = 0x40;
	/* length including mac, excluding crc */
	cmd[l++] = 6 + 3 + 2 + size;
	/* mac */
	cmd[l++] = nrf->mac[0];
	cmd[l++] = nrf->mac[1];
	cmd[l++] = nrf->mac[2];
	cmd[l++] = nrf->mac[3];
	cmd[l++] = nrf->mac[4];
	cmd[l++] = nrf->mac[5];
	/* flags (LE-only, limited discovery mode) */
	cmd[l++] = 0x02;
	cmd[l++] = 0x01;
	cmd[l++] = 0x05;
	/* data length */
	cmd[l++] = 1 + size;
	/* data type */
	cmd[l++] = type;
	/* data */
	memcpy(cmd + l, data, size);
	l += size;
	/* crc initial value */
	cmd[l++] = 0x55;
	cmd[l++] = 0x55;
	cmd[l++] = 0x55;

	/* calculate crc */
	btLeCrc(cmd + 1, l - 4, cmd + l - 3);
	for (uint8_t i = 0; i < 3; i++) {
		cmd[l - 3 + i] = BYTE_SWAP_BITS(cmd[l - 3 + i]);
	}

	/* do whitening */
	btLeWhiten(cmd + 1, l - 1, nrf->channel_whiten);
	for (uint8_t i = 1; i < l; i++) {
		cmd[i] = BYTE_SWAP_BITS(cmd[i]);
	}

	/* write data to tx buffer */
	IF_R(spi_transfer(&nrf->nrf.spi, cmd, sizeof(cmd)), -1);
	/* enable radio */
	nrf24l01p_enable_radio(&nrf->nrf);
	/* wait data to be transmitted */
	while (!(nrf24l01p_read_status(&nrf->nrf) & 0x20));
	/* disable radio */
	nrf24l01p_disable_radio(&nrf->nrf);
	nrf24l01p_flush_tx(&nrf->nrf);

	return size;
}


/* internals */


static void btLeCrc(uint8_t *data, uint8_t size, uint8_t *dst)
{
	uint8_t v, t, d;

	while (size--) {

		d = *data++;
		for (v = 0; v < 8; v++, d >>= 1) {

			t = dst[0] >> 7;

			dst[0] <<= 1;
			if (dst[1] & 0x80) dst[0] |= 1;
			dst[1] <<= 1;
			if (dst[2] & 0x80) dst[1] |= 1;
			dst[2] <<= 1;


			if (t != (d & 1)) {

				dst[2] ^= 0x5B;
				dst[1] ^= 0x06;
			}
		}
	}
}

static void btLeWhiten(uint8_t* data, uint8_t size, uint8_t whitenCoeff)
{

	uint8_t  m;

	while (size--) {

		for (m = 1; m; m <<= 1) {

			if (whitenCoeff & 0x80) {

				whitenCoeff ^= 0x11;
				(*data) ^= m;
			}
			whitenCoeff <<= 1;
		}
		data++;
	}
}
