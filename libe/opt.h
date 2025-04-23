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

#define OPT_START_GENERIC  0x00
#define OPT_START_MASTER   0x20
#define OPT_START_DEVICE   0x40
#define OPT_START_DISPLAY  0x60
#define OPT_START_UNKNOWN1 0x80
#define OPT_START_UNKNOWN2 0xa0
#define OPT_START_UNKNOWN3 0xc0
#define OPT_START_CUSTOM   0xe0

enum {
    OPT_UNKNOWN = OPT_START_GENERIC,
};

/* opt function:
 * int8_t (*opt)(<struct> *p, uint8_t opt, void *value)
 */

#define optctl(object, option, value) ((object)->opt ? (object)->opt(object, option, value) : -1)

#define optwr_u8(object, option, value)              \
    do {                                             \
        if ((object)->opt) {                         \
            uint8_t value_t = value;                 \
            (object)->opt(object, option, &value_t); \
        }                                            \
    } while (0)
#define optwr_i8(object, option, value)              \
    do {                                             \
        if ((object)->opt) {                         \
            int8_t value_t = value;                  \
            (object)->opt(object, option, &value_t); \
        }                                            \
    } while (0)
#define optwr_u16(object, option, value)             \
    do {                                             \
        if ((object)->opt) {                         \
            uint16_t value_t = value;                \
            (object)->opt(object, option, &value_t); \
        }                                            \
    } while (0)
#define optwr_i16(object, option, value)             \
    do {                                             \
        if ((object)->opt) {                         \
            int16_t value_t = value;                 \
            (object)->opt(object, option, &value_t); \
        }                                            \
    } while (0)
#define optwr_u32(object, option, value)             \
    do {                                             \
        if ((object)->opt) {                         \
            uint32_t value_t = value;                \
            (object)->opt(object, option, &value_t); \
        }                                            \
    } while (0)
#define optwr_i32(object, option, value)             \
    do {                                             \
        if ((object)->opt) {                         \
            int32_t value_t = value;                 \
            (object)->opt(object, option, &value_t); \
        }                                            \
    } while (0)
#define optwr_float(object, option, value)           \
    do {                                             \
        if ((object)->opt) {                         \
            float value_t = value;                   \
            (object)->opt(object, option, &value_t); \
        }                                            \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif /* _LIBE_OPT_H_ */
