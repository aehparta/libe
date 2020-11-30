/*
 * Display drivers
 */

#ifndef _DISPLAY_DISPLAY_H_
#define _DISPLAY_DISPLAY_H_
#ifdef USE_DISPLAY

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <libe/libe.h>

enum {
	DISPLAY_OPT_GET_BUFFER = OPT_START_DISPLAY,
	DISPLAY_OPT_SET_BUFFER,
	DISPLAY_OPT_GET_W,
	DISPLAY_OPT_SET_W,
	DISPLAY_OPT_GET_H,
	DISPLAY_OPT_SET_H,
	DISPLAY_OPT_GET_SCALING,
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

#ifdef USE_DRIVER_SDL2
	uint8_t scaling;
#endif

	uint8_t driver_bits[4];

	void (*close)(struct display *display);
	int8_t (*opt)(struct display *display, uint8_t opt, void *value);
	void (*pixel)(struct display *display, int16_t x, int16_t y, uint32_t color);
	void (*hline)(struct display *display, int16_t x, int16_t y, int16_t length, uint32_t color);
	void (*vline)(struct display *display, int16_t x, int16_t y, int16_t length, uint32_t color);
	void (*rect)(struct display *display, int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color);
	void (*fill)(struct display *display, int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color);
	void (*update)(struct display *display);
};

#define display_close(display) do { if ((display)->close) { (display)->close(display); } } while(0)
#define display_update(display) do { if ((display)->update) { (display)->update(display); } } while(0)

/* basic draw routines */
#include "draw.h"

/* drivers */
#include "drivers/display/ssd1306.h"
#include "drivers/display/sdl2.h"


#ifdef __cplusplus
}
#endif

#endif /* USE_DISPLAY */
#endif /* _DISPLAY_DISPLAY_H_ */
