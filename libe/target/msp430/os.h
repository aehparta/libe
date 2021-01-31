/*
 * msp430 specific os routines as macros.
 */

#include <msp430.h>

#ifndef _TARGET_MSP430_OS_H_
#define _TARGET_MSP430_OS_H_

#ifdef __cplusplus
extern "C" {
#endif

#define OS_DELAY_AS_MACROS_OR_INLINE    1
#define os_delay_us(t) __delay_cycles(F_CPU * (unsigned long)t / 1000000UL)
#define os_delay_ms(t) __delay_cycles(F_CPU * (unsigned long)t / 1000UL)

int8_t os_init(void);
#define os_quit()
#define os_wdt_reset()

#ifdef __cplusplus
}
#endif

#endif /* _TARGET_MSP430_OS_H_ */
