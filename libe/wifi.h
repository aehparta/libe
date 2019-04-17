/*
 * libe cross-platform library
 *
 * Authors: Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_WIFI_H_
#define _LIBE_WIFI_H_

#include <stdint.h>
#include <stdbool.h>
#ifdef TARGET_ESP32
#include "target/esp32/wifi.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

int wifi_init(void);
void wifi_quit(void);
bool wifi_connected(void);

#ifdef __cplusplus
}
#endif

#endif /* _LIBE_WIFI_H_ */
