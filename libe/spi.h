/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_SPI_H_
#define _LIBE_SPI_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* pre-define master and device for following includes, these cannot be fully declared here yet */
struct spi_master;
struct spi_device;

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
#elif TARGET_LINUX
#include "target/linux/spidev.h"
#ifdef USE_FTDI
#include "target/linux/spiftdi.h"
#endif /* USE_FTDI */
#endif /* TARGET_LINUX */

#ifdef USE_SPI_BITBANG
#include "drivers/spi/bitbang.h"
#endif


struct spi_master {
	int8_t (*open)(struct spi_device *device, struct spi_master *master, uint8_t ss);
	void (*close)(struct spi_device *device);
	int8_t (*transfer)(struct spi_device *device, uint8_t *data, size_t size);

	/* drivers that use pins dynamically */
#ifdef SPI_MASTER_NEED_PINS
	uint8_t sclk;
	uint8_t mosi;
	uint8_t miso;
#endif
	/* linux specific */
#ifdef SPI_MASTER_NEED_FD
	int fd;
#endif
	/* drivers that need to save frequency to be used with transfer */
#ifdef SPI_MASTER_NEED_FREQUENCY
	uint32_t frequency;
#endif
	/* esp32 specific */
#ifdef TARGET_ESP32
	spi_host_device_t host;
#endif
	/* xc8 compiler for 8-bit PICs will crash if struct is empty */
#ifdef TARGET_PIC8
	uint8_t placeholder;
#endif
};

struct spi_device {
#ifdef SPI_DEVICE_NEED_MASTER
	struct spi_master *master;
#endif
#ifdef SPI_DEVICE_NEED_SS
	uint8_t ss;
#endif
	/* esp32 specific */
#ifdef TARGET_ESP32
	spi_device_handle_t device;
#endif
	/* generic driver bits */
#ifdef SPI_DEVICE_NEED_BYTES
	uint8_t driver_bits[SPI_DEVICE_NEED_BYTES];
#endif
	/* options callback */
#ifdef SPI_DEVICE_NEED_OPT_CB
	int8_t (*opt)(struct spi_device *device, uint8_t opt, void *value);
#endif
};


#if !defined(spi_master_open) && !defined(SPI_DRIVER)

/* define default spi driver: use ftdi? */
#if defined(USE_FTDI)
#define SPI_DRIVER spiftdi
/* define default spi driver: use spidev? */
#elif defined(TARGET_LINUX)
#define SPI_DRIVER spidev
/* define default spi driver: use integrated? */
#elif !defined(spi_master_open) && defined(SPI_HAS_INTEGRATED)
#define SPI_DRIVER spii
/* define default spi driver: use bitbang? */
#elif !defined(spi_master_open) && defined(USE_SPI_BITBANG)
#define SPI_DRIVER spibb
#endif

#endif

/* default spi driver if given */
#if !defined(spi_master_open) && defined(SPI_DRIVER)
#define SPI_DRIVER_MACRO1(driver, func) driver ## func
#define SPI_DRIVER_MACRO2(driver, func) SPI_DRIVER_MACRO1(driver, func)
#define spi_master_open SPI_DRIVER_MACRO2(SPI_DRIVER, _master_open)
#define spi_master_close SPI_DRIVER_MACRO2(SPI_DRIVER, _master_close)
#define spi_open SPI_DRIVER_MACRO2(SPI_DRIVER, _open)
#define spi_close SPI_DRIVER_MACRO2(SPI_DRIVER, _close)
#define spi_transfer SPI_DRIVER_MACRO2(SPI_DRIVER, _transfer)
#endif

// int spi_master_open(struct spi_master *master, void *context, uint32_t frequency, uint8_t miso, uint8_t mosi, uint8_t sclk);
// void spi_master_close(struct spi_master *master);

// int spi_open(struct spi_device *device, struct spi_master *master, uint8_t ss);
// void spi_close(struct spi_device *device);
// int spi_transfer(struct spi_device *device, uint8_t *data, size_t size);


/* drivers */
#ifdef USE_DRIVER_NRF24L01P
#include <libe/drivers/spi/nrf24l01p.h>
#include <libe/drivers/spi/nrf24l01p_ble.h>
#endif
#ifdef USE_DRIVER_MCP356X
#include <libe/drivers/spi/mcp356x.h>
#endif


#ifdef __cplusplus
}
#endif

#endif /* _LIBE_SPI_H_ */
