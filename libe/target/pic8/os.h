/*
 * 8-bit pic specific os routines as macros.
 */

#ifndef _TARGET_OS_H_
#define _TARGET_OS_H_

#ifdef __cplusplus
extern "C" {
#endif

#define _XTAL_FREQ          F_CPU
#include <xc.h>

int8_t os_init(void);
#define os_quit()
#define os_wdt_reset() __asm("clrwdt")

#define _BV(_PIN)           (1 << (_PIN))

#define os_delay_ms(x)      __delay_ms(x)
#define os_delay_us(x)      __delay_us(x)

#if defined(RA0PPS) || defined(RB0PPS) || defined(RC0PPS)
#define HAS_PPS
void os_pin_pps(uint8_t pin, uint8_t action);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _TARGET_OS_H_ */
