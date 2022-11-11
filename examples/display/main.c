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

	/* open display */
#ifdef USE_DRIVER_SSD1306
	void *context = CFG_I2C_CONTEXT;
	uint8_t buffer[SSD1306_BUFFER_SIZE];
	struct i2c_master i2c;
#ifdef TARGET_LINUX
	ERROR_IF_R(argc < 2, 1, "Give i2c device as and argument\nExample: ./display-x86.elf /dev/i2c-3");
	context = argv[1];
#endif
	ERROR_IF_R(i2c_master_open(&i2c, context, CFG_I2C_FREQUENCY, CFG_I2C_SCL, CFG_I2C_SDA), 1, "unable to open i2c device");
	ERROR_IF_R(ssd1306_i2c_open(&display, &i2c, 0, 0, 0), 1, "unable to open ssd1306 display");
	optctl(&display, DISPLAY_OPT_SET_BUFFER, buffer);
#endif
#if defined(USE_DRIVER_SDL2)
	ERROR_IF_R(display_sdl2_open(&display, NULL, 0, 0, 0), 1, "unable to open sdl2 display");
	optwr_float(&display, DISPLAY_OPT_SET_SCALING, 10);
#endif


	// memset(buffer, 0x00, sizeof(buffer));

	draw_fill(&display, 0, 0, 128, 64, 0x000000);

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
		os_sleepf(0.2);

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

	return 0;
}
