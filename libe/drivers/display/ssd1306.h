/*
 * Display driver: SSD1306
 */

#ifndef _DISPLAY_SSD1306_H_
#define _DISPLAY_SSD1306_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define SSD1306_ADDR1   0x3c
#define SSD1306_ADDR2   0x3d

/* SSD1306 commands */
#define SSD1306_SETLOWCOLUMN        0x00
#define SSD1306_SETHIGHCOLUMN       0x10
#define SSD1306_MEMORYMODE          0x20
#define SSD1306_COLUMNADDR          0x21
#define SSD1306_PAGEADDR            0x22
#define SSD1306_SETSTARTLINE        0x40
#define SSD1306_DEFAULT_ADDRESS     0x78
#define SSD1306_SETCONTRAST         0x81
#define SSD1306_CHARGEPUMP          0x8D
#define SSD1306_SEGREMAP            0xA0
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON        0xA5
#define SSD1306_NORMALDISPLAY       0xA6
#define SSD1306_INVERTDISPLAY       0xA7
#define SSD1306_SETMULTIPLEX        0xA8
#define SSD1306_DISPLAYOFF          0xAE
#define SSD1306_DISPLAYON           0xAF
#define SSD1306_SETPAGE             0xB0
#define SSD1306_COMSCANINC          0xC0
#define SSD1306_COMSCANDEC          0xC8
#define SSD1306_SETDISPLAYOFFSET    0xD3
#define SSD1306_SETDISPLAYCLOCKDIV  0xD5
#define SSD1306_SETPRECHARGE        0xD9
#define SSD1306_SETCOMPINS          0xDA
#define SSD1306_SETVCOMDETECT       0xDB
#define SSD1306_SWITCHCAPVCC        0x02
#define SSD1306_NOP                 0xE3

#define SSD1306_HORIZONTAL_ADDRESSING_MODE  0x00
#define SSD1306_VERTICAL_ADDRESSING_MODE    0x01
#define SSD1306_PAGE_ADDRESSING_MODE        0x02


int8_t ssd1306_i2c_open(struct display *display, void *context, uint8_t address, int16_t w, int16_t h);
int32_t ssd1306_i2c_opt(struct display *display, struct opt *opt);
void ssd1306_i2c_pixel(struct display *display, int16_t x, int16_t y, uint32_t color);


#ifdef __cplusplus
}
#endif

#endif /* _DISPLAY_SSD1306_H_ */
