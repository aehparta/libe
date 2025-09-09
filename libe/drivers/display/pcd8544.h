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


int8_t pcd8544_open(struct display *display, struct spi_master *master, uint8_t ce, uint8_t dc, uint8_t rst, int16_t w, int16_t h);
void pcd8544_update(struct display *display);


#ifdef __cplusplus
}
#endif

#endif /* _DISPLAY_PCD8544_H_ */
