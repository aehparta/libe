/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef __NRF_H__
#define __NRF_H__

#include <stdint.h>
#include "spi.h"

#ifdef __cplusplus
extern "C" {
#endif

// #if defined(NRF_GPIO_CSN) && defined(NRF_GPIO_CE) && defined(NRF_GPIO_CONTEXT)
// /* defined already outside of the code */
// #elif TARGET_AVR
// /* avr */
// #define NRF_GPIO_CSN            0
// #define NRF_GPIO_CE             1
// #define NRF_GPIO_CONTEXT        B
// #elif TARGET_PIC8
// /* 8-bit pic */
// #define NRF_GPIO_CSN            0
// #define NRF_GPIO_CE             1
// #define NRF_GPIO_CONTEXT        C
// #elif TARGET_PIC16
// /* 16-bit pic */
// #define NRF_GPIO_CSN            0
// #define NRF_GPIO_CE             1
// #define NRF_GPIO_CONTEXT        B
// #elif TARGET_PIC32
// /* 32-bit pic */
// #define NRF_GPIO_CSN            0
// #define NRF_GPIO_CE             1
// #define NRF_GPIO_CONTEXT        NULL
// #elif TARGET_MSP430
// /* ti msp430 */
// #define NRF_GPIO_CSN            0
// #define NRF_GPIO_CE             1
// #define NRF_GPIO_CONTEXT        1
// #elif TARGET_RPI
// /* raspberry */
// #define NRF_GPIO_CSN            8
// #define NRF_GPIO_CE             25
// #define NRF_GPIO_CONTEXT        NULL
// #elif TARGET_X86
// /* x86 (through ftdi usb) */
// #define NRF_GPIO_CSN            0x08
// #define NRF_GPIO_CE             0x10
// #define NRF_GPIO_CONTEXT        NULL
// #elif TARGET_ESP32
// /* esp32 */
// #define NRF_GPIO_CSN            GPIO_NUM_15
// #define NRF_GPIO_CE             GPIO_NUM_27
// #define NRF_GPIO_CONTEXT        NULL
// #endif

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


int nrf_init(spi_master_t master, int ss, int ce);
void nrf_quit(void);
int nrf_simple_cmd(uint8_t command);
int nrf_read_status(void);
int nrf_read_reg(uint8_t reg, uint8_t *status);
int nrf_write_reg(uint8_t reg, uint8_t data);
int nrf_enable_radio(void);
int nrf_disable_radio(void);

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
int nrf_set_address(uint8_t pipe, uint8_t a0, uint8_t a1, uint8_t a2, uint8_t a3, uint8_t a4);

void nrf_setup(void);
int nrf_mode_tx(void);
int nrf_mode_rx(void);
int nrf_flush_tx(void);
int nrf_flush_rx(void);
void nrf_set_power(uint8_t power);
int nrf_recv(void *data);
int nrf_send(void *data);


#ifdef __cplusplus
}
#endif

#endif /* __NRF_H__ */