/*
 * libe cross-platform library
 *
 * XTEA encryption.
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifndef _CRYPT_XTEA_H_
#define _CRYPT_XTEA_H_





/*
 * NOTE:
 * This implementation has not been properly evaluated against others since there are so few.
 * I only left this here for the time being. It is anyways a direct copy paste from wikipedia.
 */





#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * XTEA block size is 8 bytes.
 * See: https://en.wikipedia.org/wiki/XTEA
 */

void xtea_encrypt(void *block, void *key);
void xtea_decrypt(void *block, void *key);

#ifdef __cplusplus
}
#endif

#endif /* _CRYPT_XTEA_H_ */
