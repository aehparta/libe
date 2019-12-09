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

	struct spi_master master;
	struct nrf24l01p_device nrf;

	spi_master_open(&master, NULL, 0, GPIOA2, GPIOA1, GPIOA0);
	ERROR_IF(nrf24l01p_open(&nrf, &master, GPIOA4, GPIOA5), "failed to open");
	nrf24l01p_set_standby(&nrf, true);
	nrf24l01p_set_power_down(&nrf, true);

	DEBUG_MSG("doing stuff");
	while (1) {
		os_wdt_reset();
	}

	return 0;
}
