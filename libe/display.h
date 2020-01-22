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
	DISPLAY_OPT_SET_BUFFER = OPT_START_DISPLAY,
	DISPLAY_OPT_GET_BUFFER,
	DISPLAY_OPT_GET_W,
	DISPLAY_OPT_GET_H,
	DISPLAY_OPT_SET_SCALING,
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
	int16_t clip_x1;
	int16_t clip_y1;
	int16_t clip_x2;
	int16_t clip_y2;

	uint8_t *buffer;

#ifdef USE_SDL2
	uint8_t scaling;
#endif

	uint8_t driver_bits[4];

	void (*close)(struct display *display);
	int32_t (*opt)(struct display *display, struct opt *opt);
	void (*pixel)(struct display *display, int16_t x, int16_t y, uint32_t color);
	void (*hline)(struct display *display, int16_t x, int16_t y, int16_t length, uint32_t color);
	void (*vline)(struct display *display, int16_t x, int16_t y, int16_t length, uint32_t color);
	void (*rect)(struct display *display, int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color);
	void (*fill)(struct display *display, int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color);
};

static inline void display_close(struct display *display)
{
	if (display->close) {
		display->close(display);
	}
}

static inline int8_t display_opt(struct display *display, struct opt *opt)
{
	if (display->opt) {
		return display->opt(display, opt);
	}
	return -1;
}

/* basic draw routines */
#include "draw.h"

/* drivers */
#include "drivers/display/ssd1306.h"
#include "drivers/display/sdl2.h"


#ifdef __cplusplus
}
#endif

#endif /* _DISPLAY_DISPLAY_H_ */
