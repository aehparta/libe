/*
 * AVR internal ADC driver
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifdef USE_ADC

#include <libe/libe.h>


int8_t adci_open(struct adc *adc, void *context)
{
	/* adc reference voltage to internal 2.56 V */
	ADMUX = (1 << REFS1) | (1 << REFS0);
	/* enable adc, set adc clock prescaler to full 128 and clear interrupt flag */
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADIF);
	return 0;
}

uint32_t adci_sps(struct adc *adc, uint32_t frequency)
{
	uint32_t div;

	if (frequency > 0) {
		div = F_CPU / 13 / frequency;
		if (div <= 2) {
			ADCSRA = (ADCSRA & 0xf8) | 0x01;
		} else if (div <= 4) {
			ADCSRA = (ADCSRA & 0xf8) | 0x02;
		} else if (div <= 8) {
			ADCSRA = (ADCSRA & 0xf8) | 0x03;
		} else if (div <= 16) {
			ADCSRA = (ADCSRA & 0xf8) | 0x04;
		} else if (div <= 32) {
			ADCSRA = (ADCSRA & 0xf8) | 0x05;
		} else if (div <= 64) {
			ADCSRA = (ADCSRA & 0xf8) | 0x06;
		} else {
			ADCSRA = (ADCSRA & 0xf8) | 0x07;
		}
	}

	div = ADCSRA & 0x07;
	if (div == 0) {
		div = 1;
	}
	div = 1 << div;

	return F_CPU / div / 13;
}

uint32_t adci_read_raw(struct adc *adc, uint8_t channel)
{
	ADMUX = (ADMUX & 0xe0) | (channel & 0x1f);
	ADCSRA |= 0x40;
	/* wait for conversion to complete */
	while (!(ADCSRA & (1 << ADIF)));
	/* read result */
	uint32_t r = ADCL;
	r |= ADCH << 8;
	/* clear interrupt */
	ADCSRA |= (1 << ADIF);
	return r;
}

#endif
