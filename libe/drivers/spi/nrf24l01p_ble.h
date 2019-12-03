/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_NRF24L01P_BLE_H_
#define _LIBE_NRF24L01P_BLE_H_

#include "nrf24l01p.h"

#ifdef __cplusplus
extern "C" {
#endif

/* nrf24l01+ BLE device context */
struct nrf24l01p_ble_device {
	struct nrf24l01p_device nrf;
	uint8_t mac[6];
	uint8_t channel_whiten;
};


int nrf24l01p_ble_open(struct nrf24l01p_ble_device *nrf, struct spi_master *master, int ss, int ce, uint8_t mac[6]);
void nrf24l01p_ble_close(struct nrf24l01p_ble_device *nrf);
void nrf24l01p_ble_set_channel(struct nrf24l01p_ble_device *nrf, uint8_t channel);
void nrf24l01p_ble_send(struct nrf24l01p_ble_device *nrf, uint8_t* packet, uint8_t len);


#ifdef __cplusplus
}
#endif

#endif /* _LIBE_NRF24L01P_BLE_H_ */