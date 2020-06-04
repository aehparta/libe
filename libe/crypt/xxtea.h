/*
 * libe cross-platform library
 *
 * XXTEA encryption.
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifndef _CRYPT_XXTEA_H_
#define _CRYPT_XXTEA_H_





/*
 * NOTE:
 * This implementation has not been properly evaluated against others since there are so few.
 * I only left this here for the time being. It is anyways a direct copy paste from wikipedia.
 */





#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * XXTEA data size must be at least 64 bits (8 bytes) and multiples of 32 bits (4 bytes).
 * See: https://en.wikipedia.org/wiki/XXTEA
 */

void xxtea_encrypt(void *data, size_t size, void *key);
void xxtea_decrypt(void *data, size_t size, void *key);

#ifdef __cplusplus
}
#endif

#endif /* _CRYPT_XXTEA_H_ */
