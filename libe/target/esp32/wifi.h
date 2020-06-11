/*
 * WiFi support.
 */

#ifndef _TARGET_ESP32_WIFI_H_
#define _TARGET_ESP32_WIFI_H_

#include <stdbool.h>

int wifi_smartconfig(bool force);
bool wifi_smartconfig_in_progress(void);

#endif /* _TARGET_ESP32_WIFI_H_ */
