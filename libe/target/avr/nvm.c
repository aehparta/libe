/*
 * Non-volatile memory handler for avr.
 */

#ifdef USE_NVM

#include <math.h>
#include <avr/eeprom.h>
#include <libe/nvm.h>

int nvm_init(void *context, size_t size)
{
	return 0;
}

void nvm_quit(void)
{

}

uint8_t nvm_read_byte(void *address, uint8_t default_value)
{
	uint8_t value = eeprom_read_byte(address);
	if (value == 0xff) {
		return default_value;
	}
	return value;
}

void nvm_write_byte(void *address, uint8_t value)
{
	eeprom_write_byte(address, value);
}

uint16_t nvm_read_word(void *address, uint16_t default_value)
{
	uint16_t value = eeprom_read_word(address);
	if (value == 0xffff) {
		return default_value;
	}
	return value;
}

void nvm_write_word(void *address, uint16_t value)
{
	eeprom_write_word(address, value);
}

uint32_t nvm_read_dword(void *address, uint32_t default_value)
{
	uint32_t value = eeprom_read_dword(address);
	if (value == 0xffffffff) {
		return default_value;
	}
	return value;
}

void nvm_write_dword(void *address, uint32_t value)
{
	eeprom_write_dword(address, value);
}

float nvm_read_float(void *address, float default_value)
{
	float value = eeprom_read_float(address);
	if (!isfinite(value)) {
		return default_value;
	}
	return value;
}

void nvm_write_float(void *address, float value)
{
	eeprom_write_float(address, value);
}

#endif
