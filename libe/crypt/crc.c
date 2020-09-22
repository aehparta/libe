/*
 * libe cross-platform library
 *
 * CRC checksum.
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include "crc.h"

// uint8_t crc8(void *data, int len)
// {
// 	uint8_t crc = 0x00, *p = data;
// 	return crc;
// }

uint8_t crc8_dallas(void *data, int len)
{
	uint8_t crc = 0, *p = data, x;

	for (int i = 0; i < len; i++) {
		x = crc ^ p[i];
		crc = 0;
		if (x & 1) crc ^= 0x5e;
		if (x & 2) crc ^= 0xbc;
		if (x & 4) crc ^= 0x61;
		if (x & 8) crc ^= 0xc2;
		if (x & 0x10) crc ^= 0x9d;
		if (x & 0x20) crc ^= 0x23;
		if (x & 0x40) crc ^= 0x46;
		if (x & 0x80) crc ^= 0x8c;
	}

	return crc;
}
