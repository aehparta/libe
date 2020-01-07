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
	struct i2c_master i2c;
	struct i2c_device dev;
	uint8_t b[16];

	os_init();
	log_init();

	ERROR_IF_R(i2c_master_open(&i2c, NULL, 0, 0, 0), 1, "unable to open i2c master");
	ERROR_IF_R(i2c_open(&dev, &i2c, 0x08), 1, "unable to open act4751");

	b[0] = 0x0a;
	b[1] = 0x31;
	b[2] = 0x17;
	i2c_write(&dev, b, 3);

	b[0] = 0x05;
	b[1] = 0x03;
	i2c_write(&dev, b, 2);

	while (1) {
		os_wdt_reset();

		b[0] = 0x00;
		i2c_write(&dev, b, 1);
		i2c_read(&dev, b, 16);
		
		// printf("%02x %02x %02x %02x\r\n", b[0], b[1], b[2], b[3]);
		HEX_DUMP(b, 16);

		os_delay_ms(1000);
	}

	return 0;
}
