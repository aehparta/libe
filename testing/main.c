/*
 * Random testing area for development.
 */

#include <libe/libe.h>


#ifdef TARGET_ESP32
int app_main(int argc, char *argv[])
#else
int main(void)
#endif
{
	struct i2c_master master;
	struct i2c_device dev;
	
	i2c_master_open(&master, NULL, 0, 0, 0);

	os_delay_ms(1);

	i2c_open(&dev, &master, 0x50);
	os_delay_us(50);
	i2c_write_byte(&dev, 0xa5);
	i2c_close(&dev);

	while(1);

	// os_init();
	// log_init();

	// os_gpio_output(x);
	// os_gpio_set(0, 1);
	// LATA |= 2;

	// log_quit();
	// os_quit();
	return 0;
}
