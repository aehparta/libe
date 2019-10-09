/*
 * ESP32 specific os routines.
 */

#ifndef _TARGET_ESP32_OS_H_
#define _TARGET_ESP32_OS_H_

#include <driver/gpio.h>

int os_init(void);
void os_quit(void);
#define os_wdt_reset()

#define OS_GPIO_HAS_PULLUP
#define OS_GPIO_HAS_PULLDOWN
#define OS_GPIO_HAS_FLOATING

#endif /* _TARGET_ESP32_OS_H_ */
