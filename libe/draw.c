/*
 * Display drivers: generic draw
 */

#ifdef USE_DISPLAY

#include <libe/libe.h>

void draw_generic_hline(struct display *display, int16_t x, int16_t y, int16_t length, uint32_t color)
{
	for (length--; length >= 0; length--) {
		display->pixel(display, x + length, y, color);
	}
}

void draw_generic_vline(struct display *display, int16_t x, int16_t y, int16_t length, uint32_t color)
{
	for (length--; length >= 0; length--) {
		display->pixel(display, x, y + length, color);
	}
}

void draw_generic_rect(struct display *display, int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color)
{
	display->hline(display, x, y, w, color);
	display->vline(display, x, y, h, color);
	display->hline(display, x, y + h - 1, w, color);
	display->vline(display, x + w - 1, y, h, color);
}

void draw_generic_fill(struct display *display, int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color)
{
	for (h--; h >= 0; h--) {
		display->hline(display, x, y + h, w, color);
	}
}

#endif
