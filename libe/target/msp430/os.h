/*
 * msp430 specific os routines as macros.
 */

#include <msp430.h>

#ifndef _TARGET_MSP430_OS_H
#define _TARGET_MSP430_OS_H

#ifdef __cplusplus
extern "C" {
#endif

int os_init(void);
#define os_quit()

#define _BV(_PIN)           (1 << (_PIN))

// #define OS_GPIO_AS_MACROS_OR_INLINE     1
#define OS_DELAY_AS_MACROS_OR_INLINE    1

inline void os_delay_us(uint16_t t)
{
	while (t) {
		t--;
	}
}

inline void os_delay_ms(uint16_t t)
{
	while (t) {
		os_delay_us(999);
		t--;
	}
}

// #define os_gpio_enable(pin, dir)
// #define os_gpio_set(pin, state)
// #define os_gpio_read(pin)


#ifdef __cplusplus
}
#endif

#endif /* _TARGET_MSP430_OS_H */
