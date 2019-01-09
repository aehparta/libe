/*
 * 8-bit pic specific os routines as macros.
 */

#ifndef _TARGET_OS_H_
#define _TARGET_OS_H_

#define _XTAL_FREQ          F_CPU
#include <xc.h>

#define OS_GPIO_AS_MACROS_OR_INLINE     1
#define OS_DELAY_AS_MACROS_OR_INLINE    1

#define _BV(_PIN)           (1 << (_PIN))

#define os_delay_ms(x)      __delay_ms(x)
#define os_delay_us(x)      __delay_us(x)

int8_t os_gpio_enable(uint8_t pin, bool direction);
int8_t os_gpio_set(uint8_t pin, bool state);
uint8_t os_gpio_read(uint8_t pin);


#endif /* _TARGET_OS_H_ */
