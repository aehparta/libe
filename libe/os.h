/*
 * libe cross-platform library
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifndef _OS_H_
#define _OS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#define OS_GPIO_INPUT   false
#define OS_GPIO_OUTPUT  true

#ifdef TARGET_AVR
#include <util/delay.h>
#include "target/avr/os.h"
#elif TARGET_PIC8
#include "target/pic8/os.h"
#elif TARGET_PIC16
#include "target/pic16/os.h"
#elif TARGET_PIC32
#include "target/pic32/os.h"
#elif TARGET_MSP430
#include "target/msp430/os.h"
#elif TARGET_X86
#include <signal.h>
#include "cmd-common.h"
#ifdef USE_FTDI
#include "target/x86/ftdi.h"
#endif
#elif TARGET_RPI
#include <signal.h>
#include "cmd-common.h"
#endif

#ifdef TARGET_AVR
typedef double os_time_t;
#elif TARGET_PIC8
typedef double os_time_t;
#else
typedef long double os_time_t;
#endif

int os_init(void);
#define os_quit()

/**
 * Get absolute monotonic time with one second resolution.
 * Zero is some unknown time in the past, usually system startup.
 * @return time_t (uint32_t usually)
 */
time_t os_timei(void);

/**
 * Get absolute monotonic time with system specific resolution as floating point number.
 * Zero is some unknown time in the past, usually system startup.
 * @return os_time_t (long double or double usually)
 */
os_time_t os_timef(void);

/**
 * Sleep with multiple of seconds, resolution depends on system.
 * @param t seconds
 */
void os_sleepi(time_t t);

/**
 * Sleep specified amount of time. Resolution depends on system.
 * @param t seconds as floating point number
 */
void os_sleepf(os_time_t t);

const char *os_get_revision(void);


#ifndef OS_GPIO_AS_MACROS_OR_INLINE
int os_gpio_enable(uint8_t pin, bool direction);
int os_gpio_output(uint8_t pin);
int os_gpio_input(uint8_t pin);
int os_gpio_set(uint8_t pin, bool state);
int os_gpio_high(uint8_t pin);
int os_gpio_low(uint8_t pin);
int os_gpio_read(uint8_t pin);
#endif

#ifndef OS_DELAY_AS_MACROS_OR_INLINE
#define os_delay_ms(x)      os_sleepf((os_time_t)x / 1000.0)
#define os_delay_us(x)      os_sleepf((os_time_t)x / 1000.0 / 1000.0)
#endif


#ifdef __cplusplus
}
#endif

#endif /* _OS_H_ */
