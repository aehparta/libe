/*
 * ESP32 specific os routines.
 */

#ifndef _TARGET_ESP32_OS_H_
#define _TARGET_ESP32_OS_H_

#include <esp_int_wdt.h>

int8_t os_init(void);
void os_quit(void);
void os_restart(void);
#define os_wdt_reset()

#define os_delay_ms(x)      os_sleepf((os_time_t)x / 1000.0)
#define os_delay_us(x)      os_sleepf((os_time_t)x / 1000.0 / 1000.0)

#endif /* _TARGET_ESP32_OS_H_ */
