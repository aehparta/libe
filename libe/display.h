/*
 * Display drivers
 */

#ifndef _DISPLAY_DISPLAY_H_
#define _DISPLAY_DISPLAY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <libe/libe.h>

enum {
	DISPLAY_OPT_SET_BUFFER,
	DISPLAY_OPT_GET_BUFFER,
	DISPLAY_OPT_GET_W,
	DISPLAY_OPT_GET_H,
};

struct display {
	union {
		void *context;
#ifdef USE_I2C
		struct i2c_device i2c;
#endif
#ifdef USE_SPI
		struct spi_device spi;
#endif
	};

	int16_t w;
	int16_t h;
	uint8_t *buffer;
	uint8_t driver_bits[4];

	void (*close)(struct display *display);
	int32_t (*opt)(struct display *display, struct opt *opt);
	void (*pixel)(struct display *display, int16_t x, int16_t y, uint32_t color);
	void (*hline)(struct display *display, int16_t x, int16_t y, int16_t length, uint32_t color);
	void (*vline)(struct display *display, int16_t x, int16_t y, int16_t length, uint32_t color);
	void (*rect)(struct display *display, int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color);
	void (*fill)(struct display *display, int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color);
};

/* basic draw routines */
#include "draw.h"

/* drivers */
#include "drivers/display/ssd1306.h"


#ifdef __cplusplus
}
#endif

#endif /* _DISPLAY_DISPLAY_H_ */
