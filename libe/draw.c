/*
 * Display drivers: generic draw
 */

#ifdef USE_DISPLAY

#include <libe/libe.h>


int8_t draw_generic_opt(struct display *display, uint8_t opt, void *value)
{
    switch (opt) {
    case DISPLAY_OPT_SET_BUFFER:
        display->buffer = value;
        return 0;
    }

    return -1;
}

void draw_generic_pixel_monochrome(struct display *display, int16_t x, int16_t y, uint32_t color)
{
    if (display->clip_x1 > x || display->clip_x2 < x || display->clip_y1 > y || display->clip_y2 < y) {
        return;
    }
    uint8_t *p = display->buffer + x + (display->w * (y >> 3));
    uint8_t mask = (1 << (y & 0x07));
    if (color) {
        *p = *p | mask;
    } else {
        *p = *p & ~mask;
    }
}

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
