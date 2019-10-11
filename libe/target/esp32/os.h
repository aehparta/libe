/*
 * ESP32 specific os routines.
 */

#ifndef _TARGET_ESP32_OS_H_
#define _TARGET_ESP32_OS_H_

#include <driver/gpio.h>

int os_init(void);
void os_quit(void);
#define os_wdt_reset()

#endif /* _TARGET_ESP32_OS_H_ */
