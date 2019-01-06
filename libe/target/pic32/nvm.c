/*
 * Non-volatile memory handler for pic 32-bit.
 */

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
	return default_value;
}

void nvm_write_byte(void *address, uint8_t value)
{
}

uint16_t nvm_read_word(void *address, uint16_t default_value)
{
	return default_value;
}

void nvm_write_word(void *address, uint16_t value)
{
}

uint32_t nvm_read_dword(void *address, uint32_t default_value)
{
	return default_value;
}

void nvm_write_dword(void *address, uint32_t value)
{
}

float nvm_read_float(void *address, float default_value)
{
	return default_value;
}

void nvm_write_float(void *address, float value)
{
}
