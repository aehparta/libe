/*
 * Options
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_OPT_H_
#define _LIBE_OPT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define OPT_FLAG_SET            0x00
#define OPT_FLAG_GET            0x01
#define OPT_FLAG_MASK_SET_GET   0x01

#define OPT_START_GENERIC       0x00
#define OPT_START_MASTER        0x20
#define OPT_START_DEVICE        0x40
#define OPT_START_DISPLAY       0x60
#define OPT_START_UNKNOWN2      0x80
#define OPT_START_UNKNOWN3      0xa0
#define OPT_START_UNKNOWN4      0xc0
#define OPT_START_UNKNOWN5      0xe0

enum {
	OPT_UNKNOWN = OPT_START_GENERIC,
};

#define opt_set(object, option, value) ((object)->opt ? (object)->opt(object, option, value, OPT_FLAG_SET) : -1)
#define opt_get(object, option, value) ((object)->opt ? (object)->opt(object, option, value, OPT_FLAG_GET) : -1)

#define opt_set_u8(object, option, value) do { if ((object)->opt) { uint8_t value_t = value; (object)->opt(object, option, &value_t, OPT_FLAG_SET); } } while (0)
#define opt_set_i8(object, option, value) do { if ((object)->opt) { int8_t value_t = value; (object)->opt(object, option, &value_t, OPT_FLAG_SET); } } while (0)
#define opt_set_u16(object, option, value) do { if ((object)->opt) { uint16_t value_t = value; (object)->opt(object, option, &value_t, OPT_FLAG_SET); } } while (0)
#define opt_set_i16(object, option, value) do { if ((object)->opt) { int16_t value_t = value; (object)->opt(object, option, &value_t, OPT_FLAG_SET); } } while (0)
#define opt_set_u32(object, option, value) do { if ((object)->opt) { uint32_t value_t = value; (object)->opt(object, option, &value_t, OPT_FLAG_SET); } } while (0)
#define opt_set_i32(object, option, value) do { if ((object)->opt) { int32_t value_t = value; (object)->opt(object, option, &value_t, OPT_FLAG_SET); } } while (0)
#define opt_set_float(object, option, value) do { if ((object)->opt) { float value_t = value; (object)->opt(object, option, &value_t, OPT_FLAG_SET); } } while (0)

#ifdef __cplusplus
}
#endif

#endif /* _LIBE_OPT_H_ */
