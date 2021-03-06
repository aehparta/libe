/*
 * libe cross-platform library
 *
 * XXTEA encryption.
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include "xxtea.h"


#define DELTA 0x9e3779b9
#define MX (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (k[(p&3)^e] ^ z)))


void xxtea_encrypt(void *data, size_t size, void *key)
{
	uint32_t *k = key, *v = data;
	uint32_t y, z, sum;
	unsigned p, rounds, e;
	size = size >> 2;
	rounds = 6 + 52 / size;
	sum = 0;
	z = v[size - 1];
	do {
		sum += DELTA;
		e = (sum >> 2) & 3;
		for (p = 0; p < size - 1; p++) {
			y = v[p + 1];
			z = v[p] += MX;
		}
		y = v[0];
		z = v[size - 1] += MX;
	} while (--rounds);
}

void xxtea_decrypt(void *data, size_t size, void *key)
{
	uint32_t *k = key, *v = data;
	uint32_t y, z, sum;
	unsigned p, rounds, e;
	size = size >> 2;
	rounds = 6 + 52 / size;
	sum = rounds * DELTA;
	y = v[0];
	do {
		e = (sum >> 2) & 3;
		for (p = size - 1; p > 0; p--) {
			z = v[p - 1];
			y = v[p] -= MX;
		}
		z = v[size - 1];
		y = v[0] -= MX;
		sum -= DELTA;
	} while (--rounds);
}
