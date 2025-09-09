/*
 * Display driver: SSD1681 (e-paper)
 */

#include <libe/libe.h>

static inline void ssd1681_gpio_cmd(struct display *display)
{
    gpio_low(display->driver_bits[0]);
}

static inline void ssd1681_gpio_data(struct display *display)
{
    gpio_high(display->driver_bits[0]);
}

static inline void ssd1681_reset(struct display *display)
{
    uint8_t reset = display->driver_bits[1];
    gpio_low(reset);
    os_delay_ms(10);
    gpio_high(reset);
    os_delay_ms(10);
}

static inline uint8_t ssd1681_is_busy(struct display *display)
{
    uint8_t busy = display->driver_bits[2];
    if (busy == 0xFF) return 0; /* assume not busy if not provided */
    return gpio_read(busy);
}

static inline void ssd1681_wait_busy(struct display *display)
{
    uint32_t guard_ms = 3000; /* 3s guard to avoid infinite loop on unsupported panels */
    while (ssd1681_is_busy(display) && guard_ms--) {
        os_delay_ms(1);
    }
}

static inline void ssd1681_write_cmd(struct display *display, uint8_t cmd)
{
    ssd1681_gpio_cmd(display);
    spi_transfer(&display->spi, &cmd, 1);
}

static inline void ssd1681_write_data(struct display *display, const uint8_t *data, size_t len)
{
    if (!len) return;
    ssd1681_gpio_data(display);
    spi_transfer(&display->spi, (uint8_t *)data, len);
}

int8_t ssd1681_open(struct display *display, struct spi_master *master, uint8_t ce, uint8_t dc, uint8_t reset, uint8_t busy, int16_t w, int16_t h)
{
    display->driver_bits[0] = dc;
    display->driver_bits[1] = reset;
    display->driver_bits[2] = busy; /* optional, 0xFF to ignore */
    gpio_output(dc);
    gpio_low(dc);
    gpio_output(reset);
    gpio_high(reset);
    if (busy != 0xFF) {
        gpio_input(busy);
    }

    if (spi_open(&display->spi, master, ce)) {
        spi_close(&display->spi);
        return -1;
    }

    display->w = w ? w : 200;  /* common for 1.54" panels: 200x200 */
    display->h = h ? h : 200;
    display->clip_x1 = 0;
    display->clip_y1 = 0;
    display->clip_x2 = display->w - 1;
    display->clip_y2 = display->h - 1;

    display->close = NULL;
    display->opt = ssd1681_opt;
    display->pixel = ssd1681_pixel;

    display->hline = draw_generic_hline;
    display->vline = draw_generic_vline;
    display->rect = draw_generic_rect;
    display->fill = draw_generic_fill;

    display->update = ssd1681_update;

    /* Hardware reset */
    ssd1681_reset(display);
    ssd1681_wait_busy(display);

    /* Basic init (minimal, conservative defaults) */
    uint8_t tmp[4];

    ssd1681_write_cmd(display, SSD1681_SW_RESET);
    ssd1681_wait_busy(display);

    /* Driver output control: set MUX (height-1) and scan */
    tmp[0] = (uint8_t)((display->h - 1) & 0xFF);
    tmp[1] = (uint8_t)(((display->h - 1) >> 8) & 0xFF);
    tmp[2] = 0x00; /* GD = 0, SM = 0, TB = 0 */
    ssd1681_write_cmd(display, SSD1681_DRIVER_OUTPUT_CONTROL);
    ssd1681_write_data(display, tmp, 3);

    /* Data entry mode: X+, Y+ */
    tmp[0] = SSD1681_DATA_ENTRY_X_INC | SSD1681_DATA_ENTRY_Y_INC;
    ssd1681_write_cmd(display, SSD1681_DATA_ENTRY_MODE_SETTING);
    ssd1681_write_data(display, tmp, 1);

    /* Set RAM X start/end (in bytes: 0..(width/8 - 1)) */
    uint8_t x_end = (uint8_t)((display->w - 1) >> 3);
    tmp[0] = 0x00;
    tmp[1] = x_end;
    ssd1681_write_cmd(display, SSD1681_SET_RAM_X_ADDRESS_START_END);
    ssd1681_write_data(display, tmp, 2);

    /* Set RAM Y start/end */
    uint16_t y_end = (uint16_t)(display->h - 1);
    tmp[0] = 0x00; /* start low */
    tmp[1] = 0x00; /* start high */
    tmp[2] = (uint8_t)(y_end & 0xFF);
    tmp[3] = (uint8_t)((y_end >> 8) & 0xFF);
    ssd1681_write_cmd(display, SSD1681_SET_RAM_Y_ADDRESS_START_END);
    ssd1681_write_data(display, tmp, 4);

    /* Set RAM X,Y counters to 0 */
    tmp[0] = 0x00;
    ssd1681_write_cmd(display, SSD1681_SET_RAM_X_ADDRESS_COUNTER);
    ssd1681_write_data(display, tmp, 1);
    tmp[0] = 0x00;
    tmp[1] = 0x00;
    ssd1681_write_cmd(display, SSD1681_SET_RAM_Y_ADDRESS_COUNTER);
    ssd1681_write_data(display, tmp, 2);

    return 0;
}

int8_t ssd1681_opt(struct display *display, uint8_t opt, void *value)
{
    switch (opt) {
    case DISPLAY_OPT_SET_BUFFER:
        display->buffer = value;
        return 0;
    }

    return -1;
}

void ssd1681_pixel(struct display *display, int16_t x, int16_t y, uint32_t color)
{
    if (display->clip_x1 > x || display->clip_x2 < x || display->clip_y1 > y || display->clip_y2 < y) {
        return;
    }
    /* 1bpp, 0 = black, 1 = white. Invert so color!=0 means black pixel cleared (0) */
    uint8_t *p = display->buffer + (x >> 3) + ((display->w >> 3) * y);
    uint8_t mask = 0x80 >> (x & 0x07);
    if (color) {
        *p &= (uint8_t)~mask; /* black */
    } else {
        *p |= mask; /* white */
    }
}

void ssd1681_update(struct display *display)
{
    /* Set RAM write pointer to origin */
    uint8_t tmp[2];
    tmp[0] = 0x00;
    ssd1681_write_cmd(display, SSD1681_SET_RAM_X_ADDRESS_COUNTER);
    ssd1681_write_data(display, tmp, 1);
    tmp[0] = 0x00; tmp[1] = 0x00;
    ssd1681_write_cmd(display, SSD1681_SET_RAM_Y_ADDRESS_COUNTER);
    ssd1681_write_data(display, tmp, 2);

    /* Write frame buffer */
    ssd1681_write_cmd(display, SSD1681_WRITE_RAM);
    ssd1681_write_data(display, display->buffer, (display->w >> 3) * display->h);

    /* Trigger display refresh (using a generic update sequence) */
    tmp[0] = 0xC7; /* pattern: enable clock, CP, display, load LUT */
    ssd1681_write_cmd(display, SSD1681_DISPLAY_UPDATE_CONTROL_2);
    ssd1681_write_data(display, tmp, 1);
    ssd1681_write_cmd(display, SSD1681_MASTER_ACTIVATION);
    ssd1681_wait_busy(display);
}


