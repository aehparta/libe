/*
 * Display drivers: generic draw
 */

#ifndef _DISPLAY_DRAW_H_
#define _DISPLAY_DRAW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "draw_string.h"


void draw_generic_pixel(struct display *display, int16_t x, int16_t y, uint32_t color);
void draw_generic_hline(struct display *display, int16_t x, int16_t y, int16_t length, uint32_t color);
void draw_generic_vline(struct display *display, int16_t x, int16_t y, int16_t length, uint32_t color);
void draw_generic_rect(struct display *display, int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color);
void draw_generic_fill(struct display *display, int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color);

static inline void draw_pixel(struct display *display, int16_t x, int16_t y, uint32_t color)
{
	display->pixel(display, x, y, color);
}

static inline void draw_hline(struct display *display, int16_t x, int16_t y, int16_t length, uint32_t color)
{
	display->hline(display, x, y, length, color);
}

static inline void draw_vline(struct display *display, int16_t x, int16_t y, int16_t length, uint32_t color)
{
	display->vline(display, x, y, length, color);
}

static inline void draw_rect(struct display *display, int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color)
{
	display->rect(display, x, y, w, h, color);
}

static inline void draw_fill(struct display *display, int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color)
{
	display->fill(display, x, y, w, h, color);
}

#ifdef __cplusplus
}
#endif

#endif /* _DISPLAY_DRAW_H_ */
