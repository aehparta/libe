/*
 * libe cross-platform library: non-volatile memory driver
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_NVM_H_
#define _LIBE_NVM_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int nvm_init(void *context, size_t size);
void nvm_quit(void);

uint8_t nvm_read_byte(void *address, uint8_t default_value);
void nvm_write_byte(void *address, uint8_t value);
uint16_t nvm_read_word(void *address, uint16_t default_value);
void nvm_write_word(void *address, uint16_t value);
uint32_t nvm_read_dword(void *address, uint32_t default_value);
void nvm_write_dword(void *address, uint32_t value);
float nvm_read_float(void *address, float default_value);
void nvm_write_float(void *address, float value);

#ifdef __cplusplus
}
#endif

#endif /* _LIBE_NVM_H_ */
