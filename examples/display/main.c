/*
 * Scan i2c bus for devices.
 */

#include <libe/libe.h>
#ifdef TARGET_LINUX
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#include "../config.h"

uint8_t *test(void)
{
	static uint8_t b[1024];
	return b;
}

#ifdef TARGET_ESP32
int app_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	void *context = CFG_I2C_CONTEXT;
	struct i2c_master i2c;
	struct display display;
	struct opt opt;
	// uint8_t buffer[129 * 64 / 8];
	uint8_t *buffer = test();
	
	/* base init */
	os_init();
	log_init();

	/* open display */
#ifdef USE_I2C
#ifdef TARGET_LINUX
	ERROR_IF_R(argc < 2, 1, "Give i2c device as and argument\nExample: ./display-x86.elf /dev/i2c-3");
	context = argv[1];
#endif
	ERROR_IF_R(i2c_master_open(&i2c, context, CFG_I2C_FREQUENCY, CFG_I2C_SCL, CFG_I2C_SDA), 1, "unable to open i2c device");
	ERROR_IF_R(ssd1306_i2c_open(&display, &i2c, 0, 0, 0), 1, "unable to open ssd1306 display");
	opt.opt = DISPLAY_OPT_SET_BUFFER;
	opt.p = buffer;
	display_opt(&display, &opt);
#endif
#if defined(USE_SDL2) && !defined(USE_I2C)
	ERROR_IF_R(display_sdl2_open(&display, NULL, 0, 0, 0), 1, "unable to open sdl2 display");
	opt.opt = DISPLAY_OPT_SET_SCALING;
	opt.f32 = 10;
	display_opt(&display, &opt);
#endif


	// memset(buffer, 0x00, sizeof(buffer));

	draw_fill(&display, 0, 0, 128, 64, 0x000000);

	draw_pixel(&display, 1, 1, 0xffffff);

	draw_hline(&display, 20, 20, 30, 0xff0000);
	draw_vline(&display, 25, 25, 10, 0x0ff000);

	draw_rect(&display, 15, 5, 40, 40, 0x00ff00);

	eui_str_draw(&display, &g_sFontFixed6x8, "Hello world!", -1, 10, 10, true);
	eui_str_draw(&display, &g_sFontCm30, "Hello world!", -1, 10, 20, false);

	display_update(&display);
	
	// os_sleepi(5);

	display_close(&display);

	/* close i2c */
	// i2c_master_close(&i2c);

	return 0;
}
