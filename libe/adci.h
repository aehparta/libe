/*
 * Internal ADC driver base definitions
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_ADCI_H_
#define _LIBE_ADCI_H_

#include <stdint.h>

#ifdef TARGET_AVR
#include "target/avr/adci.h"
#endif

struct adc {
	uint8_t placeholder;
};

int8_t adci_open(struct adc *adc, void *context);
uint32_t adci_frequency(struct adc *adc, uint32_t frequency);
uint32_t adci_read_raw(struct adc *adc, uint8_t channel);


#endif /* _LIBE_ADCI_H_ */
