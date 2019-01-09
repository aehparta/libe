/*
 * 16-bit pic specific os routines as macros.
 */

#ifndef _TARGET_OS_H_
#define _TARGET_OS_H_

#define FCY                 F_CPU
#include <xc.h>
#include <libpic30.h>

#define OS_GPIO_AS_MACROS_OR_INLINE     1
#define OS_DELAY_AS_MACROS_OR_INLINE    1

#define _BV(_PIN)           (1 << (_PIN))

#define os_delay_ms(x)      __delay_ms(x)
#define os_delay_us(x)      __delay_us(x)

int8_t os_gpio_enable(uint8_t pin, bool direction);
int8_t os_gpio_set(uint8_t pin, bool state);
uint8_t os_gpio_read(uint8_t pin);

#define NAN                 -1e13
#define isfinite(x)         (x > -0.9e13)
#define M_E                 2.7182818284590452354

#endif /* _TARGET_OS_H_ */
