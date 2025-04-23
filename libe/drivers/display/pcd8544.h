/*
 * Display driver: PCD8544
 */

#ifndef _DISPLAY_PCD8544_H_
#define _DISPLAY_PCD8544_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define PCD8544_BUFFER_SIZE (84 * 48 / 8)


int8_t pcd8544_open(struct display *display, struct spi_master *master, uint8_t ce, uint8_t dc, uint8_t reset, int16_t w, int16_t h);
int8_t pcd8544_opt(struct display *display, uint8_t opt, void *value);
void pcd8544_pixel(struct display *display, int16_t x, int16_t y, uint32_t color);
void pcd8544_update(struct display *display);


#ifdef __cplusplus
}
#endif

#endif /* _DISPLAY_PCD8544_H_ */
