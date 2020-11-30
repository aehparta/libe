/*
 * Display driver: SSD1306
 */

#ifndef _DISPLAY_SDL2_H_
#define _DISPLAY_SDL2_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

int8_t display_sdl2_open(struct display *display, void *context, uint8_t address, int16_t w, int16_t h);
void display_sdl2_close(struct display *display);
int8_t display_sdl2_opt(struct display *display, uint8_t opt, void *value);
void display_sdl2_pixel(struct display *display, int16_t x, int16_t y, uint32_t color);
void display_sdl2_update(struct display *display);


#ifdef __cplusplus
}
#endif

#endif /* _DISPLAY_SDL2_H_ */
