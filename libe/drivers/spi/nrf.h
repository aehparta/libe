/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_NRF_H_
#define _LIBE_NRF_H_

#include <stdint.h>
#include <libe/spi.h>

#ifdef __cplusplus
extern "C" {
#endif

/* transmit speeds */
#define NRF_SPEED_250k          0
#define NRF_SPEED_1M            1
#define NRF_SPEED_2M            2

/* power */
#define NRF_POWER_LOW           0 /* -18dBm */
#define NRF_POWER_MEDIUM        1 /* -12dBm */
#define NRF_POWER_HIGH          2 /* -6dBm */
#define NRF_POWER_FULL          3 /* 0dBm */

/* nRF24L01+ register addresses */
#define NRF_REG_CONFIG          0x00
#define NRF_REG_EN_AA           0x01
#define NRF_REG_EN_RXADDR       0x02
#define NRF_REG_SETUP_AW        0x03
#define NRF_REG_SETUP_RETR      0x04
#define NRF_REG_RF_CH           0x05
#define NRF_REG_RF_SETUP        0x06
#define NRF_REG_STATUS          0x07
#define NRF_REG_OBSERVE_TX      0x08
#define NRF_REG_RPD             0x09
#define NRF_REG_RX_ADDR_P0      0x0a
#define NRF_REG_RX_ADDR_P1      0x0b
#define NRF_REG_RX_ADDR_P2      0x0c
#define NRF_REG_RX_ADDR_P3      0x0d
#define NRF_REG_RX_ADDR_P4      0x0e
#define NRF_REG_RX_ADDR_P5      0x0f
#define NRF_REG_TX_ADDR         0x10
#define NRF_REG_RX_PW_P0        0x11
#define NRF_REG_RX_PW_P1        0x12
#define NRF_REG_RX_PW_P2        0x13
#define NRF_REG_RX_PW_P3        0x14
#define NRF_REG_RX_PW_P4        0x15
#define NRF_REG_RX_PW_P5        0x16
#define NRF_REG_FIFO_STATUS     0x17
#define NRF_REG_DYNPD           0x1c
#define NRF_REG_FEATURE         0x1d


/* nrf device context */
struct nrf_device {
	struct spi_device spi;
	uint8_t ce;
};


int nrf_open(struct nrf_device *nrf, struct spi_master *master, int ss, int ce);
void nrf_close(struct nrf_device *nrf);
int nrf_simple_cmd(struct nrf_device *nrf, uint8_t command);
int nrf_read_status(struct nrf_device *nrf);
int nrf_read_reg(struct nrf_device *nrf, uint8_t reg, uint8_t *status);
int nrf_write_reg(struct nrf_device *nrf, uint8_t reg, uint8_t data);
int nrf_enable_radio(struct nrf_device *nrf);
int nrf_disable_radio(struct nrf_device *nrf);

/**
 * Set pipe address.
 * Pipes 0, 1 and transmit need same amount of bytes as set in SETUP_AW.
 * Other pipes need only 1 (first) byte to be valid.
 *
 * @param  pipe NRF_REG_RX_ADDR_Px or NRF_REG_TX_ADDR
 * @param  a0   address byte 0 (LSB)
 * @param  a1   address byte 1
 * @param  a2   address byte 2
 * @param  a3   address byte 3
 * @param  a4   address byte 4 (MSB)
 * @return      nrf chip status register when ok, -1 if error
 */
int nrf_set_address(struct nrf_device *nrf, uint8_t pipe, uint8_t a0, uint8_t a1, uint8_t a2, uint8_t a3, uint8_t a4);

void nrf_setup(struct nrf_device *nrf);
int nrf_mode_tx(struct nrf_device *nrf);
int nrf_mode_rx(struct nrf_device *nrf);
int nrf_flush_tx(struct nrf_device *nrf);
int nrf_flush_rx(struct nrf_device *nrf);

void nrf_set_power(struct nrf_device *nrf, uint8_t power);
void nrf_set_channel(struct nrf_device *nrf, uint8_t channel);
void nrf_set_speed(struct nrf_device *nrf, uint8_t speed);

int nrf_recv(struct nrf_device *nrf, void *data);
int nrf_send(struct nrf_device *nrf, void *data);


#ifdef __cplusplus
}
#endif

#endif /* _LIBE_NRF_H_ */