/*
 * Random testing area for development.
 */

#include <libe/libe.h>

#ifdef TARGET_PIC8
// #pragma config FEXTOSC = OFF
#pragma config WDTE = OFF
// #pragma config DEBUG = OFF
// #pragma config BOREN = OFF
// #pragma config LPBOREN = OFF
#endif

static uint8_t key[16];
static rc5_context_t rc5;
static aes_128_context_t aes;
static uint8_t data_aes[16];
static uint8_t data_rc5[8];

int main(void)
{
	os_init();
	log_init();

	gpio_output(GPIOA0);
	gpio_low(GPIOA0);

	memset(key, 0x5a, 16);
	memset(data_aes, 0xa5, 16);
	memset(data_rc5, 0xa5, 8);
	aes_128_init(&aes, key);
	rc5_init(&rc5, key);

	while (1) {
		os_delay_ms(100);
		gpio_high(GPIOA0);
		aes_128_encrypt(&aes, data_aes);
		gpio_low(GPIOA0);

		os_delay_ms(100);
		gpio_high(GPIOA0);
		rc5_encrypt(&rc5, data_rc5);
		gpio_low(GPIOA0);
	}

	/* free */
	log_quit();
	os_quit();
	return 0;
}
