/*
 * AVR specific os routines as macros.
 */

#ifndef _TARGET_OS_H_
#define _TARGET_OS_H_

#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>

#ifdef __cplusplus
extern "C" {
#endif

#define OS_DELAY_AS_MACROS_OR_INLINE    1

int os_init(void);
#define os_quit()

#define os_wdt_reset() wdt_reset()

#define os_delay_ms(x)      _delay_ms(x)
#define os_delay_us(x)      _delay_us(x)

#ifdef __cplusplus
}
#endif

#endif /* _TARGET_OS_H_ */
