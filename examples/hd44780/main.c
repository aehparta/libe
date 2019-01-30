/*
 * Random testing area for development.
 */

#include <libe/os.h>
#include <libe/log.h>
#include <libe/drivers/gpio/hd44780.h>
#include <libe/i2c.h>
#include <libe/spi.h>
#include "../config.h"


#ifdef TARGET_ESP32
int app_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	os_init();
	log_init(NULL, 0);

#ifdef USE_FTDI
	ERROR_IF_R(os_ftdi_use(OS_FTDI_GPIO_0_TO_63, CFG_FTDI_VID, CFG_FTDI_PID, CFG_FTDI_DESC, CFG_FTDI_SERIAL), -1, "unable to open ftdi device for gpio 0-63");
#endif

	struct hd44780_device lcd1;
	hd44780_open(&lcd1, CFG_HD44780_D4, CFG_HD44780_D5, CFG_HD44780_D6, CFG_HD44780_D7, CFG_HD44780_EN, CFG_HD44780_RW, CFG_HD44780_RS);
	hd44780_write_str(&lcd1, "Hello there");
	hd44780_goto_xy(&lcd1, 0, 1);
	hd44780_write_str(&lcd1, "Just testing");
	hd44780_close(&lcd1);

	log_quit();
	os_quit();
	return 0;
}
