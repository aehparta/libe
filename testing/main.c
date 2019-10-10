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
	char data[2] = { 0, 0 };

	os_delay_ms(100);
	
	i2c_master_open(&master, NULL, 0, 0, 0);

	i2c_open(&dev, &master, 0x55);
	i2c_write_byte(&dev, 0);
	i2c_close(&dev);

	i2c_open(&dev, &master, 0x66);
	i2c_write(&dev, data, 2);
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
