/*
 * Display example.
 */

#include <libe/libe.h>
#ifdef TARGET_LINUX
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#ifdef USE_DRIVER_SDL2
#include <SDL2/SDL.h>
#endif
#include "../config.h"

#define WIDTH 200
#define HEIGHT 200


#ifdef TARGET_ESP32
int app_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    struct display display;

    /* base init */
    os_init();
    log_init();

    /* display: SSD1306 */
#ifdef USE_DRIVER_SSD1306
    void *context = CFG_I2C_CONTEXT;
    uint8_t buffer[SSD1306_BUFFER_SIZE];
    struct i2c_master i2c;
#ifdef TARGET_LINUX
    ERROR_IF_R(argc < 2, 1, "Give i2c device as and argument\nExample: ./display-x86.elf /dev/i2c-3");
    context = argv[1];
#endif
    ERROR_IF_R(i2c_master_open(&i2c, context, CFG_I2C_FREQUENCY, CFG_I2C_SCL, CFG_I2C_SDA), 1, "unable to open i2c device");
    ERROR_IF_R(ssd1306_i2c_open(&display, &i2c, 0, WIDTH, HEIGHT), 1, "unable to open ssd1306 display");
    optctl(&display, DISPLAY_OPT_SET_BUFFER, buffer);
#endif
#if defined(USE_DRIVER_SDL2)
    ERROR_IF_R(display_sdl2_open(&display, NULL, 0, 0, 0), 1, "unable to open sdl2 display");
    optwr_float(&display, DISPLAY_OPT_SET_SCALING, 10);
#endif

    /* display: PCD8544 */
#ifdef USE_DRIVER_PCD8544
    void *context = CFG_SPI_CONTEXT;
    uint8_t buffer[PCD8544_BUFFER_SIZE];
    struct spi_master master;
#ifdef USE_FTDI
    ERROR_IF_R(os_ftdi_use(OS_FTDI_GPIO_0_TO_63, 0x0403, 0x6014, NULL, NULL), -1, "unable to open ftdi device for gpio 0-63");
    os_ftdi_set_mpsse(CFG_SPI_SCLK);
#endif
    ERROR_IF_R(spi_master_open(
                   &master,         /* must give pre-allocated spi master as pointer */
                   CFG_SPI_CONTEXT, /* context depends on platform */
                   CFG_SPI_FREQUENCY,
                   CFG_SPI_MISO,
                   CFG_SPI_MOSI,
                   CFG_SPI_SCLK),
               -1,
               "failed to open spi master");
    ERROR_IF_R(pcd8544_open(&display, &master, CFG_PCD8544_CE, CFG_PCD8544_DC, CFG_PCD8544_RESET, WIDTH, HEIGHT), 1, "unable to open pcd8544 display");
    optctl(&display, DISPLAY_OPT_SET_BUFFER, buffer);
#endif

    /* display: SSD1681 */
#ifdef USE_DRIVER_SSD1681
    void *context = CFG_SPI_CONTEXT;
    uint8_t buffer[SSD1681_BUFFER_SIZE(WIDTH, HEIGHT)];
    struct spi_master master;
#ifdef USE_FTDI
    ERROR_IF_R(os_ftdi_use(OS_FTDI_GPIO_0_TO_63, 0x0403, 0x6014, NULL, NULL), -1, "unable to open ftdi device for gpio 0-63");
    os_ftdi_set_mpsse(CFG_SPI_SCLK);
#endif
    ERROR_IF_R(spi_master_open(
                   &master,         /* must give pre-allocated spi master as pointer */
                   CFG_SPI_CONTEXT, /* context depends on platform */
                   CFG_SPI_FREQUENCY,
                   CFG_SPI_MISO,
                   CFG_SPI_MOSI,
                   CFG_SPI_SCLK),
               -1,
               "failed to open spi master");
    ERROR_IF_R(ssd1681_open(&display, &master, CFG_SSD1681_CS, CFG_SSD1681_DC, CFG_SSD1681_RESET, CFG_SSD1681_BUSY, WIDTH, HEIGHT), 1, "unable to open pcd8544 display");
    optctl(&display, DISPLAY_OPT_SET_BUFFER, buffer);
#endif

    // memset(buffer, 0x00, sizeof(buffer));

    draw_fill(&display, 0, 0, WIDTH, HEIGHT, 0x000000);

    draw_pixel(&display, 1, 1, 0xffffff);

    draw_hline(&display, 20, 20, 30, 0xff0000);
    draw_vline(&display, 25, 25, 10, 0x0ff000);

    draw_rect(&display, 15, 5, 40, 40, 0x00ff00);

    draw_string(&display, &g_sFontFixed6x8, "Hello world!", -1, 10, 10, true);

    draw_string(&display, &g_sFontCm30, "Hello world!", -1, 10, 20, false);

    while (1) {
        char str[32];
        sprintf(str, "%3.3f", (float)os_timef());
        draw_string(&display, &g_sFontFixed6x8, str, -1, 0, 50, true);

        display_update(&display);
        os_sleepf(2);

#ifdef USE_DRIVER_SDL2
        SDL_Event event;
        memset(&event, 0, sizeof(event));
        SDL_PollEvent(&event);
        DEBUG_MSG("%d", event.type);
        if (event.type == SDL_QUIT) {
            break;
        }
#endif
    }

    display_close(&display);
#ifdef USE_I2C
    i2c_master_close(&i2c);
#endif
#ifdef USE_SPI
    spi_master_close(&master);
#endif

    return 0;
}
