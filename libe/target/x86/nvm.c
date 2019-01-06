/*
 * Non-volatile memory handler for x86.
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <libe/nvm.h>


uint8_t *nvm_data = NULL;


int nvm_init(void *context, size_t size)
{
	nvm_data = malloc(size);
	memset(nvm_data, 0xff, size);
	return 0;
}

void nvm_quit(void)
{
	if (nvm_data) {
		free(nvm_data);
		nvm_data = NULL;
	}
}

uint8_t nvm_read_byte(void *address, uint8_t default_value)
{
	uint8_t *p = &nvm_data[(uint64_t)address];

	// if (address == (void *)NVM_A_TYPE) {
	// 	/* special case: default_value to electrical */
	// 	return PEER_TYPE_HEATER_ELECTRICAL;
	// } else
	// 
	if (address >= (void *)4 && address <= ((void *)4 + 7)) {
		/* special case: default_value to 12345678 */
		return '1' + ((uint64_t)address - (uint64_t)4);
	}

	return *p != 0xff ? *p : default_value;
}

void nvm_write_byte(void *address, uint8_t value)
{
	uint8_t *p = &nvm_data[(uint64_t)address];
	*p = value;
}

uint16_t nvm_read_word(void *address, uint16_t default_value)
{
	uint16_t *p = (uint16_t *)&nvm_data[(uint64_t)address];
	return *p != 0xffff ? *p : default_value;
}

void nvm_write_word(void *address, uint16_t value)
{
	uint16_t *p = (uint16_t *)&nvm_data[(uint64_t)address];
	*p = value;
}

uint32_t nvm_read_dword(void *address, uint32_t default_value)
{
	uint32_t *p = (uint32_t *)&nvm_data[(uint64_t)address];

	// if (address == (void *)NVM_A_SERIAL_NUMBER) {
	// 	/* special case: defaul to random */
	// 	return rand() % 1000 + 50000;
	// }

	return *p != 0xffffffff ? *p : default_value;
}

void nvm_write_dword(void *address, uint32_t value)
{
	uint32_t *p = (uint32_t *)&nvm_data[(uint64_t)address];
	*p = value;
}

float nvm_read_float(void *address, float default_value)
{
	float *p = (float *)&nvm_data[(uint64_t)address];
	return isfinite(*p) ? *p : default_value;
}

void nvm_write_float(void *address, float value)
{
	float *p = (float *)&nvm_data[(uint64_t)address];
	*p = value;
}
