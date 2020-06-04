/*
 * libe cross-platform library
 *
 * RC5 encryption.
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifndef _CRYPT_RC5_H_
#define _CRYPT_RC5_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RC5_KEY_LEN         16
#define RC5_ROUNDS          12
#define RC5_WORD            uint32_t
#define RC5_TABLE_SIZE      (2 * (RC5_ROUNDS + 1))

typedef struct {
	RC5_WORD S[RC5_TABLE_SIZE];
} rc5_context_t;

/*
 * This implementation is RC5-32-12-16.
 * Block size is 8 bytes.
 */

void rc5_init(rc5_context_t *rc5, void *key);
void rc5_encrypt(rc5_context_t *rc5, void *block);
void rc5_decrypt(rc5_context_t *rc5, void *block);
// void rc5_decrypt(void *block, void *key);

#ifdef __cplusplus
}
#endif

#endif /* _CRYPT_RC5_H_ */
