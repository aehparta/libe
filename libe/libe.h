/*
 * libe cross-platform library: Logging
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_LIBE_H_
#define _LIBE_LIBE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* generic option structure */
struct opt {
	uint8_t opt;
	union {
		uint8_t u8;
		int8_t i8;
		uint16_t u16;
		int16_t i16;
		uint32_t u32;
		int32_t i32;
		float f32;
		void *p;
	};
};

#define OPT_START_GENERIC   0x00
#define OPT_START_MASTER    0x20
#define OPT_START_DEVICE    0x40
#define OPT_START_DISPLAY   0x60
#define OPT_START_UNKNOWN2  0x80
#define OPT_START_UNKNOWN3  0xa0
#define OPT_START_UNKNOWN4  0xc0
#define OPT_START_UNKNOWN5  0xe0

enum {
	OPT_UNKNOWN = OPT_START_GENERIC,
};

#include "os.h"
#include "log.h"
#include "wifi.h"
#include "nvm.h"
#include "aes.h"
#include "error.h"
/* gpio */
#include "gpio.h"
/* i2c drivers */
#include "i2c.h"
/* spi drivers */
#include "spi.h"
/* internal adc drivers */
#include "adci.h"
/* display */
#include "display.h"

#ifdef __cplusplus
}
#endif

#endif /* _LIBE_LIBE_H_ */
