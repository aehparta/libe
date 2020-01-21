/*
 * libe mesh
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_MESH_H_
#define _LIBE_MESH_H_

#include <stdint.h>

#define MESH_A_BCAST        0xffffffff
#define MESH_A_GROUPS       0xffffff00

struct mesh {
	uint32_t dst;
	uint32_t src;

	uint8_t seq; /* sender specific packet sequence number */
    /* bits:
     *  - 0-1: ttl, 0 = init, 3 = drop
     *  - 2-3: encryption, 0 = no encryption, 1 = AES 128, 2 and 3 reserved
     *  - 4: mode, 0 = data, 1 = ack
     */
	uint8_t flags;
	uint8_t type;
	uint8_t crc; /* crc for data, before encryption */

	uint8_t data[16];
};


#endif /* _LIBE_MESH_H_ */
