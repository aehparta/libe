/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_NRF24L01P_H_
#define _LIBE_NRF24L01P_H_

#include <stdint.h>
#include <libe/libe.h>

#ifdef __cplusplus
extern "C" {
#endif

/* transmit speeds */
#define NRF24L01P_SPEED_250k          0
#define NRF24L01P_SPEED_1M            1
#define NRF24L01P_SPEED_2M            2

/* power */
#define NRF24L01P_POWER_LOW           0 /* -18dBm */
#define NRF24L01P_POWER_MEDIUM        1 /* -12dBm */
#define NRF24L01P_POWER_HIGH          2 /* -6dBm */
#define NRF24L01P_POWER_FULL          3 /* 0dBm */

/* nRF24L01+ register addresses */
#define NRF24L01P_REG_CONFIG          0x00
#define NRF24L01P_REG_EN_AA           0x01
#define NRF24L01P_REG_EN_RXADDR       0x02
#define NRF24L01P_REG_SETUP_AW        0x03
#define NRF24L01P_REG_SETUP_RETR      0x04
#define NRF24L01P_REG_RF_CH           0x05
#define NRF24L01P_REG_RF_SETUP        0x06
#define NRF24L01P_REG_STATUS          0x07
#define NRF24L01P_REG_OBSERVE_TX      0x08
#define NRF24L01P_REG_RPD             0x09
#define NRF24L01P_REG_RX_ADDR_P0      0x0a
#define NRF24L01P_REG_RX_ADDR_P1      0x0b
#define NRF24L01P_REG_RX_ADDR_P2      0x0c
#define NRF24L01P_REG_RX_ADDR_P3      0x0d
#define NRF24L01P_REG_RX_ADDR_P4      0x0e
#define NRF24L01P_REG_RX_ADDR_P5      0x0f
#define NRF24L01P_REG_TX_ADDR         0x10
#define NRF24L01P_REG_RX_PW_P0        0x11
#define NRF24L01P_REG_RX_PW_P1        0x12
#define NRF24L01P_REG_RX_PW_P2        0x13
#define NRF24L01P_REG_RX_PW_P3        0x14
#define NRF24L01P_REG_RX_PW_P4        0x15
#define NRF24L01P_REG_RX_PW_P5        0x16
#define NRF24L01P_REG_FIFO_STATUS     0x17
#define NRF24L01P_REG_DYNPD           0x1c
#define NRF24L01P_REG_FEATURE         0x1d


/* nrf24l01+ device context */
struct nrf24l01p_device {
	struct spi_device spi;
	uint8_t ce;
};


int8_t nrf24l01p_open(struct nrf24l01p_device *nrf, struct spi_master *master, uint8_t ss, uint8_t ce);
void nrf24l01p_close(struct nrf24l01p_device *nrf);
int8_t nrf24l01p_simple_cmd(struct nrf24l01p_device *nrf, uint8_t command);
int8_t nrf24l01p_read_status(struct nrf24l01p_device *nrf);
int8_t nrf24l01p_read_reg(struct nrf24l01p_device *nrf, uint8_t reg, uint8_t *status);
int8_t nrf24l01p_write_reg(struct nrf24l01p_device *nrf, uint8_t reg, uint8_t data);

void nrf24l01p_setup(struct nrf24l01p_device *nrf);
int8_t nrf24l01p_mode_tx(struct nrf24l01p_device *nrf);
int8_t nrf24l01p_mode_rx(struct nrf24l01p_device *nrf);
int8_t nrf24l01p_flush_tx(struct nrf24l01p_device *nrf);
int8_t nrf24l01p_flush_rx(struct nrf24l01p_device *nrf);

/**
 * Set pipe address.
 * Pipes 0, 1 and transmit need same amount of bytes as set in SETUP_AW.
 * Other pipes need only 1 (first) byte to be valid.
 *
 * @param  pipe NRF24L01P_REG_RX_ADDR_Px or NRF24L01P_REG_TX_ADDR
 * @param  a0   address byte 0 (LSB)
 * @param  a1   address byte 1
 * @param  a2   address byte 2
 * @param  a3   address byte 3
 * @param  a4   address byte 4 (MSB)
 * @return      nrf chip status register when ok, -1 if error
 */
int8_t nrf24l01p_set_address(struct nrf24l01p_device *nrf, uint8_t pipe, uint8_t a0, uint8_t a1, uint8_t a2, uint8_t a3, uint8_t a4);

int8_t nrf24l01p_set_standby(struct nrf24l01p_device *nrf, bool standby);
int8_t nrf24l01p_set_power_down(struct nrf24l01p_device *nrf, bool pd);

int8_t nrf24l01p_set_tx_power(struct nrf24l01p_device *nrf, uint8_t power);
int8_t nrf24l01p_set_channel(struct nrf24l01p_device *nrf, uint8_t channel);
int8_t nrf24l01p_set_speed(struct nrf24l01p_device *nrf, uint8_t speed);

int8_t nrf24l01p_set_speed(struct nrf24l01p_device *nrf, uint8_t speed);

int8_t nrf24l01p_set_crc(struct nrf24l01p_device *nrf, uint8_t crc);

int8_t nrf24l01p_recv(struct nrf24l01p_device *nrf, void *data);
int8_t nrf24l01p_send(struct nrf24l01p_device *nrf, void *data);


#ifdef __cplusplus
}
#endif

#endif /* _LIBE_NRF24L01P_H_ */