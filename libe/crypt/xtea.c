/*
 * libe cross-platform library
 *
 * XTEA encryption.
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include "xtea.h"


#define DELTA 0x9E3779B9
#define NUM_ROUNDS 32
#define MX (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (k[(p&3)^e] ^ z)))


void xtea_encrypt(void *block, void *key)
{
	uint32_t *k = key, *v = block;
	unsigned int i;
	uint32_t v0 = v[0], v1 = v[1], sum = 0;
	for (i = 0; i < NUM_ROUNDS; i++) {
		v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
		sum += DELTA;
		v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum >> 11) & 3]);
	}
	v[0] = v0; v[1] = v1;
}

void xtea_decrypt(void *block, void *key)
{
	uint32_t *k = key, *v = block;
	unsigned int i;
	uint32_t v0 = v[0], v1 = v[1], sum = DELTA * NUM_ROUNDS;
	for (i = 0; i < NUM_ROUNDS; i++) {
		v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum >> 11) & 3]);
		sum -= DELTA;
		v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
	}
	v[0] = v0; v[1] = v1;
}
