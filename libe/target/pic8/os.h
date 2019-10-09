/*
 * 8-bit pic specific os routines as macros.
 */

#ifndef _TARGET_OS_H_
#define _TARGET_OS_H_

#define _XTAL_FREQ          F_CPU
#include <xc.h>

void *memcpy(void *dest, const void *src, size_t n);

// #define OS_GPIO_AS_MACROS_OR_INLINE     1
#define OS_DELAY_AS_MACROS_OR_INLINE 1
#define OS_GPIO_HAS_PULLUP 1
#define OS_GPIO_HAS_OPEN_DRAIN 1

int os_init(void);
#define os_quit()

#define os_wdt_reset() __asm("clrwdt")

#define _BV(_PIN)           (1 << (_PIN))

#define os_delay_ms(x)      __delay_ms(x)
#define os_delay_us(x)      __delay_us(x)

#if defined(RA0PPS) || defined(RB0PPS) || defined(RC0PPS)
#define OS_HAS_PPS
void os_pin_pps(uint8_t pin, uint8_t action);
#endif

#endif /* _TARGET_OS_H_ */
