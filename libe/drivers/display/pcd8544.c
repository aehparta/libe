/*
 * Display driver: PCD8544
 */

#include <libe/libe.h>


int8_t pcd8544_open(struct display *display, struct spi_master *master, uint8_t ce, uint8_t dc, uint8_t reset, int16_t w, int16_t h)
{
    display->driver_bits[0] = dc;
    display->driver_bits[1] = reset;
    gpio_output(dc);
    gpio_low(dc);
    gpio_output(reset);
    gpio_low(reset);

    if (spi_open(&display->spi, master, ce)) {
        spi_close(&display->spi);
        return -1;
    }

    display->w = w ? w : 84;
    display->h = h ? h : 48;
    display->clip_x1 = 0;
    display->clip_y1 = 0;
    display->clip_x2 = display->w - 1;
    display->clip_y2 = display->h - 1;

    display->close = NULL;
    display->opt = pcd8544_opt;
    display->pixel = pcd8544_pixel;

    display->hline = draw_generic_hline;
    display->vline = draw_generic_vline;
    display->rect = draw_generic_rect;
    display->fill = draw_generic_fill;

    display->update = pcd8544_update;

    /* display init sequence */


    uint8_t init_commands[] = {
        0x21, // Function set
        0xB0, // Set Vop (contrast)
        0x04, // Temp coefficent
        0x13, // Bias mode
        0x20, // Function set
        0x0C, // Display control
    };


    return 0;
}

int8_t pcd8544_opt(struct display *display, uint8_t opt, void *value)
{
    switch (opt) {
    case DISPLAY_OPT_SET_BUFFER:
        display->buffer = value;
        return 0;
    }

    return -1;
}

void pcd8544_pixel(struct display *display, int16_t x, int16_t y, uint32_t color)
{
    uint8_t *p = display->buffer + (x + 1) + ((display->w + 1) * (y >> 3));
    uint8_t mask = (1 << (y & 0x07));
    if (color) {
        *p = *p | mask;
    } else {
        *p = *p & ~mask;
    }
}

void pcd8544_update(struct display *display)
{
}
