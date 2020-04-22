/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_SPI_H_
#define _LIBE_SPI_H_
#ifdef USE_SPI


#include <stdint.h>
#include "os.h"
#ifdef USE_SPI_BITBANG
#include "drivers/spi/bitbang.h"
#else
#ifdef TARGET_AVR
#include "target/avr/spi.h"
#elif TARGET_PIC8
#include "target/pic8/spi.h"
#elif TARGET_PIC16
#include "target/pic16/spi.h"
#elif TARGET_PIC32
#include "target/pic32/spi.h"
#elif TARGET_MSP430
#include "target/msp430/spi.h"
#elif TARGET_ESP32
#include "target/esp32/spi.h"
#elif TARGET_X86
#include "target/x86/spi.h"
#elif TARGET_RPI
#include "target/linux/spi.h"
#elif TARGET_OPI
#include "target/linux/spi.h"
#endif
#endif /* else USE_SPI_BITBANG */

#ifdef __cplusplus
extern "C" {
#endif

int spi_master_open(struct spi_master *master, void *context, uint32_t frequency, uint8_t miso, uint8_t mosi, uint8_t sclk);
void spi_master_close(struct spi_master *master);

int spi_open(struct spi_device *device, struct spi_master *master, uint8_t ss);
void spi_close(struct spi_device *device);
int spi_transfer(struct spi_device *device, uint8_t *data, size_t size);

#include <libe/drivers/spi/nrf24l01p.h>
#include <libe/drivers/spi/nrf24l01p_ble.h>

#ifdef __cplusplus
}
#endif

#endif /* USE_SPI */
#endif /* _LIBE_SPI_H_ */
