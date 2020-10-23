/*
 * Random testing area for development.
 */

#include <libe/libe.h>


#ifdef TARGET_PIC8
#pragma config FEXTOSC = OFF
#pragma config WDTE = OFF
#pragma config DEBUG = OFF
#pragma config BOREN = OFF
#pragma config LPBOREN = OFF
#endif


int main(void)
{
	os_init();
	log_init();

	gpio_input(GPIOC6);
	TRISCbits.TRISC6 = 1;
	ANSELCbits.ANSC6 = 1;

	ADCON1 = 0xe0; /* result is right justified */
	ADACT = 0x00;
	ADCON0 = 0x5b; /* RC6 as input, adc enabled and start conversion */

	/* thing */
	while (1) {
		while (ADCON0 & 0x02);
		printf("hello %02x%02x\r\n", ADRESH, ADRESL);
		ADCON0 = 0x5b;
		os_delay_ms(300);
	}

	/* free */
	log_quit();
	os_quit();
	return 0;
}
