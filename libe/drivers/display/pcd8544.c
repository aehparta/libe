/*
 * Display driver: PCD8544
 */

#include <libe/libe.h>

#define WRITE_CTRL(byte)                      \
    do {                                      \
        uint8_t data[1] = { byte };           \
        gpio_low(display->driver_bits[0]);    \
        spi_transfer(&display->spi, data, 1); \
    } while (0)
#define WRITE_DATA(byte)                      \
    do {                                      \
        uint8_t data[1] = { byte };           \
        gpio_high(display->driver_bits[0]);   \
        spi_transfer(&display->spi, data, 1); \
    } while (0)


int8_t pcd8544_open(struct display *display, struct spi_master *master, uint8_t ce, uint8_t dc, uint8_t rst, int16_t w, int16_t h)
{
    uint8_t pdc8544_init_commands[] = {
        0x21, /* extended function set */
        0xc8, /* voltage */
        0x06, /* temperature coefficient */
        0x13, /* bias */
        0xc0, /* vop */

        0x20, /* normal function set */
        0x0c, /* switch on display and set to normal mode */
        0x40, /* page address or y */
        0x80, /* column address or x */
        0x0c, /* display on, normal mode */
    };

    display->driver_bits[0] = dc;
    display->driver_bits[1] = rst;
    gpio_output(dc);
    gpio_low(dc);
    gpio_output(rst);
    gpio_low(rst);

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
    display->opt = draw_generic_opt;
    display->pixel = draw_generic_pixel_monochrome;

    display->hline = draw_generic_hline;
    display->vline = draw_generic_vline;
    display->rect = draw_generic_rect;
    display->fill = draw_generic_fill;

    display->update = pcd8544_update;

    /* reset */
    os_delay_us(1);
    gpio_high(rst);
    os_delay_us(1);

    /* display init sequence */
    gpio_low(display->driver_bits[0]);
    spi_transfer(&display->spi, pdc8544_init_commands, sizeof(pdc8544_init_commands));

    return 0;
}

void pcd8544_update(struct display *display)
{
    gpio_high(display->driver_bits[0]);
    spi_transfer(&display->spi, display->buffer, display->w * display->h / 8);
}
