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
	os_init();
	log_init();

	printf("Hellou\r\n");

	// gpio_input(GPIOA2);
	// gpio_output(GPIOA1);
	// gpio_output(GPIOA0);
	// gpio_output(GPIOA4);
	// gpio_output(GPIOA5);

	// gpio_low(GPIOA5);
	// gpio_high(GPIOA4);
	// gpio_high(GPIOA1);
	// gpio_low(GPIOA0);

	// os_delay_ms(100);

	// gpio_low(GPIOA4);
	// os_delay_us(10);
	// for (int i = 0; i < 8; i++) {
	// 	gpio_high(GPIOA0);
	// 	os_delay_us(10);
	// 	printf("%d", gpio_read(GPIOA2));
	// 	gpio_low(GPIOA0);
	// 	os_delay_us(10);
	// }
	// gpio_high(GPIOA4);
	// printf("\r\n");

	// os_delay_ms(500);
	// return 0;

	uint8_t x[4] = { 0xff, 0xff, 0xff, 0xff };

	struct spi_master master;
	struct spi_device dev;

	spi_master_open(&master, NULL, 0, GPIOA2, GPIOA1, GPIOA0);
	spi_open(&dev, &master, GPIOA4);
	os_delay_ms(1);
	spi_transfer(&dev, x, 4);
	spi_close(&dev);
	spi_master_close(&master);

	printf("%02x %02x %02x %02x\r\n", x[0], x[1], x[2], x[3]);
	os_delay_ms(1000);

	return 0;
}
