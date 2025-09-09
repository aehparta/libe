/*
 * Display driver: SSD1306
 */

#include <libe/libe.h>


int8_t ssd1306_i2c_open(struct display *display, void *context, uint8_t address, int16_t w, int16_t h)
{
    uint8_t ssd1306_init_commands[] = {
        SSD1306_DISPLAYOFF, // display off
        SSD1306_MEMORYMODE,
        SSD1306_HORIZONTAL_ADDRESSING_MODE, // Page Addressing mode
        SSD1306_COMSCANDEC,                 // Scan from 127 to 0 (Reverse scan)
        SSD1306_SETSTARTLINE | 0x00,        // First line to start scanning from
        SSD1306_SETCONTRAST,
        0x7f,                    // contrast value to 0x7F according to datasheet
        SSD1306_SEGREMAP | 0x00, // Use reverse mapping. 0x00 - is normal mapping
        SSD1306_NORMALDISPLAY,
        SSD1306_SETMULTIPLEX,
        63, // Reset to default MUX. See datasheet
        SSD1306_SETDISPLAYOFFSET,
        0x00, // no offset
        SSD1306_SETDISPLAYCLOCKDIV,
        0x80, // set to default ratio/osc frequency
        SSD1306_SETPRECHARGE,
        0x22, // switch precharge to 0x22 // 0xF1
        SSD1306_SETCOMPINS,
        0x12, // set divide ratio
        SSD1306_SETVCOMDETECT,
        0x20, // vcom deselect to 0x20 // 0x40
        SSD1306_CHARGEPUMP,
        0x14, // Enable charge pump
        SSD1306_DISPLAYALLON_RESUME,
        SSD1306_DISPLAYON
    };

    address = address < 1 ? SSD1306_ADDR1 : address;
    IF_R(i2c_open(&display->i2c, context, address), -1);


    display->w = w ? w : 128;
    display->h = h ? h : 64;
    display->clip_x1 = 0;
    display->clip_y1 = 0;
    display->clip_x2 = display->w - 1;
    display->clip_y2 = display->h - 1;

    display->close = NULL;
    display->opt = draw_generic_opt;
    display->pixel = ssd1306_i2c_pixel;

    display->hline = draw_generic_hline;
    display->vline = draw_generic_vline;
    display->rect = draw_generic_rect;
    display->fill = draw_generic_fill;

    display->update = ssd1306_i2c_update;

    /* display init sequence */
    IF_R(i2c_write(&display->i2c, ssd1306_init_commands, sizeof(ssd1306_init_commands)), -1);

    return 0;
}

void ssd1306_i2c_pixel(struct display *display, int16_t x, int16_t y, uint32_t color)
{
    if (display->clip_x1 > x || display->clip_x2 < x || display->clip_y1 > y || display->clip_y2 < y) {
        return;
    }
    uint8_t *p = display->buffer + (x + 1) + ((display->w + 1) * (y >> 3));
    uint8_t mask = (1 << (y & 0x07));
    if (color) {
        *p = *p | mask;
    } else {
        *p = *p & ~mask;
    }
}

void ssd1306_i2c_update(struct display *display)
{
    i2c_write_reg_byte(&display->i2c, 0x00, SSD1306_SETLOWCOLUMN);
    i2c_write_reg_byte(&display->i2c, 0x00, SSD1306_SETHIGHCOLUMN);

    for (uint8_t page = 0; page < (display->h >> 3); page++) {
        i2c_write_reg_byte(&display->i2c, 0x00, SSD1306_SETPAGE | page);
        uint8_t l = display->w + 1;
        uint8_t *p = display->buffer + (l * page);
        p[0] = 0x40;
        i2c_write(&display->i2c, p, l);
    }
}
