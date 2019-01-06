/*
 * 16-bit pic specific os routines as macros.
 */

#define FCY                 F_CPU
#include <xc.h>
#include <libpic30.h>

#define _delay_ms(x)        __delay_ms(x)
#define _delay_us(x)        __delay_us(x)

#define _PIC_MACRO_OUT(X) LAT##X
#define _PIC_MACRO_DIR(X)  TRIS##X
#define _PIC_MACRO_INP(X)  PORT##X

#define OS_GPIO_AS_MACROS
#define os_gpio_enable(_PORT, _PIN, MODE) do { if (MODE) { _PIC_MACRO_DIR(_PORT) |= _BV(_PIN); } else { _PIC_MACRO_DIR(_PORT) &= ~_BV(_PIN); } } while(0)
#define os_gpio_output(_PORT, _PIN) _PIC_MACRO_DIR(_PORT) &= ~_BV(_PIN)
#define os_gpio_input(_PORT, _PIN) _PIC_MACRO_DIR(_PORT) |= _BV(_PIN)
#define os_gpio_set(_PORT, _PIN, STATE) do { if (STATE) { _PIC_MACRO_OUT(_PORT) |= _BV(_PIN); } else { _PIC_MACRO_OUT(_PORT) &= ~_BV(_PIN); } } while(0)
#define os_gpio_high(_PORT, _PIN) _PIC_MACRO_OUT(_PORT) |= _BV(_PIN)
#define os_gpio_low(_PORT, _PIN) _PIC_MACRO_OUT(_PORT) &= ~_BV(_PIN)
#define os_gpio_read(_PORT, _PIN) (_PIC_MACRO_INP(_PORT) & _BV(_PIN))

#define NAN                 -1e13
#define isfinite(x)         (x > -0.9e13)
#define M_E                 2.7182818284590452354
