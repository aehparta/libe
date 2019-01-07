/*
 * libe cross-platform library
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifndef _FTDI_H_
#define _FTDI_H_
#ifdef USE_FTDI

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <libftdi1/ftdi.h>
struct ftdi_context *ftdi_open(uint16_t vid, uint16_t pid, int interface, const char *description, const char *serial, int reset);

#ifdef __cplusplus
}
#endif

#endif
#endif /* _FTDI_H_ */
