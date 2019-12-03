/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#include <stdlib.h>
#include <libe/libe.h>

#define BYTE_SWAP_BITS(b) (((b << 7) & 0x80) | ((b << 5) & 0x40) | ((b << 3) & 0x20) | ((b << 1) & 0x10) | ((b >> 1) & 0x08) | ((b >> 3) & 0x04) | ((b >> 5) & 0x02) | ((b >> 7) & 0x01))
static void btLeCrc(const uint8_t* data, uint8_t len, uint8_t* dst);
static void btLeWhiten(uint8_t* data, uint8_t len, uint8_t whitenCoeff);


int nrf24l01p_ble_open(struct nrf24l01p_ble_device *nrf, struct spi_master *master, int ss, int ce, uint8_t mac[6])
{
	IF_R(nrf24l01p_open(&nrf->nrf, master, ss, ce), -1);
	/* address width: 4 bytes */
	nrf24l01p_write_reg(&nrf->nrf, NRF24L01P_REG_SETUP_AW, 0x04);
	/* 1 Mbps */
	nrf24l01p_set_speed(&nrf->nrf, NRF24L01P_SPEED_1M);
	/* receiving data pipes: 0 */
	nrf24l01p_write_reg(&nrf->nrf, NRF24L01P_REG_EN_RXADDR, 0x01);
	/* pipe 0 address */
	nrf24l01p_set_address(&nrf->nrf, NRF24L01P_REG_RX_ADDR_P0, BYTE_SWAP_BITS(0x8e), BYTE_SWAP_BITS(0x89), BYTE_SWAP_BITS(0xbe), BYTE_SWAP_BITS(0xd6), 0x00);
	/* transmit address */
	nrf24l01p_set_address(&nrf->nrf, NRF24L01P_REG_TX_ADDR, BYTE_SWAP_BITS(0x8e), BYTE_SWAP_BITS(0x89), BYTE_SWAP_BITS(0xbe), BYTE_SWAP_BITS(0xd6), 0x00);
	/* default channel */
	nrf24l01p_ble_set_channel(nrf, 0);

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

void nrf24l01p_ble_send(struct nrf24l01p_ble_device *nrf, uint8_t* packet, uint8_t len)
{
	//length is of packet, including crc. pre-populate crc in packet with initial crc value!

	uint8_t i, dataLen = len - 3;

	btLeCrc(packet, dataLen, packet + dataLen);
	for (i = 0; i < 3; i++, dataLen++) packet[dataLen] = BYTE_SWAP_BITS(packet[dataLen]);
	btLeWhiten(packet, len, nrf->channel_whiten);
	for (i = 0; i < len; i++) packet[i] = BYTE_SWAP_BITS(packet[i]);

}


/* internals */


static void btLeCrc(const uint8_t* data, uint8_t len, uint8_t* dst)
{

	uint8_t v, t, d;

	while (len--) {

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

static void btLeWhiten(uint8_t* data, uint8_t len, uint8_t whitenCoeff)
{

	uint8_t  m;

	while (len--) {

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
