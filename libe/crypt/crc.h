/*
 * libe cross-platform library
 *
 * CRC checksum.
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifndef _CRC_H_
#define _CRC_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t crc8_dallas(void *data, int len);

// uint16_t crc16(void *data, int size);

// uint32_t crc32(void *data, int size);

#ifdef __cplusplus
}
#endif

#endif /* _CRC_H_ */
