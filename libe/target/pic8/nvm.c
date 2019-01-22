/*
 * Non-volatile memory handler for pic 8-bit
 */

#include <libe/os.h>
#include <libe/nvm.h>


#ifdef EEADR

int nvm_init(void *context, size_t size)
{
	return 0;
}

void nvm_quit(void)
{
}

uint8_t nvm_read_byte(void *address, uint8_t default_value)
{
	static volatile __bit gie;
	uint16_t addr = (uint16_t)address;
	/* save current interrupt state and then disable interrupts */
	gie = INTCONbits.GIE;
	INTCONbits.GIE = 0;
	/* read eeprom */
	EEADR = (uint8_t)addr;
#ifdef EEADRH
	EEADRH = (uint8_t)(addr >> 8);
#endif
	uint8_t value = EEDATA;
	/* restore interrupt state */
	INTCONbits.GIE = gie;
	if (value == 0xff) {
		return default_value;
	}
	return value;
}

void nvm_write_byte(void *address, uint8_t value)
{
}

uint16_t nvm_read_word(void *address, uint16_t default_value)
{
	uint16_t value = (uint16_t)nvm_read_byte(address, 0xff) |
	                 ((uint16_t)nvm_read_byte(address + 1, 0xff) << 8);
	if (value == 0xffff) {
		return default_value;
	}
	return value;
}

void nvm_write_word(void *address, uint16_t value)
{
}

uint32_t nvm_read_dword(void *address, uint32_t default_value)
{
	uint32_t value = (uint32_t)nvm_read_byte(address, 0xff) |
	                 ((uint32_t)nvm_read_byte(address + 1, 0xff) << 8) |
	                 ((uint32_t)nvm_read_byte(address + 2, 0xff) << 16) |
	                 ((uint32_t)nvm_read_byte(address + 3, 0xff) << 24);
	if (value == 0xffffffff) {
		return default_value;
	}
	return value;
}

void nvm_write_dword(void *address, uint32_t value)
{
}

float nvm_read_float(void *address, float default_value)
{
	uint32_t v = nvm_read_dword(address, 0xffffffff);
	if (v == 0xffffffff) {
		return default_value;
	}
	float *value = (float *)&v;
	return *value;
}

void nvm_write_float(void *address, float value)
{
}

#endif
