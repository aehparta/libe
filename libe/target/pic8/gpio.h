/*
 * pic8 specific gpio.
 */

#ifndef _TARGET_PIC_GPIO_H_
#define _TARGET_PIC_GPIO_H_

#include "gpio_pps.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef GPIO_HAS_PPS
void gpio_pps(uint8_t pin, uint8_t action);
#endif

/* helper macros for pin settings */
#ifdef PORTA
#define _PIN_IF_A(pin, condition, reg) \
    if ((pin & 0xf8) == 0x00) {        \
        if (condition) {               \
            reg##A |= _BV(pin & 7);    \
        } else {                       \
            reg##A &= ~_BV(pin & 7);   \
        }                              \
    }
#else
#define _PIN_IF_A(pin, condition, reg) \
    if (0) {}
#endif
#ifdef PORTB
#define _PIN_IF_B(pin, condition, reg) \
    if ((pin & 0xf8) == 0x08) {        \
        if (condition) {               \
            reg##B |= _BV(pin & 7);    \
        } else {                       \
            reg##B &= ~_BV(pin & 7);   \
        }                              \
    }
#else
#define _PIN_IF_B(pin, condition, reg) \
    if (0) {}
#endif
#ifdef PORTC
#define _PIN_IF_C(pin, condition, reg) \
    if ((pin & 0xf8) == 0x10) {        \
        if (condition) {               \
            reg##C |= _BV(pin & 7);    \
        } else {                       \
            reg##C &= ~_BV(pin & 7);   \
        }                              \
    }
#else
#define _PIN_IF_C(pin, condition, reg) \
    if (0) {}
#endif
#ifdef PORTD
#define _PIN_IF_D(pin, condition, reg) \
    if ((pin & 0xf8) == 0x18) {        \
        if (condition) {               \
            reg##D |= _BV(pin & 7);    \
        } else {                       \
            reg##D &= ~_BV(pin & 7);   \
        }                              \
    }
#else
#define _PIN_IF_D(pin, condition, reg) \
    if (0) {}
#endif
#ifdef PORTE
#define _PIN_IF_E(pin, condition, reg) \
    if ((pin & 0xf8) == 0x20) {        \
        if (condition) {               \
            reg##E |= _BV(pin & 7);    \
        } else {                       \
            reg##E &= ~_BV(pin & 7);   \
        }                              \
    }
#else
#define _PIN_IF_E(pin, condition, reg) \
    if (0) {}
#endif
#ifdef PORTF
#define _PIN_IF_F(pin, condition, reg) \
    if ((pin & 0xf8) == 0x28) {        \
        if (condition) {               \
            reg##F |= _BV(pin & 7);    \
        } else {                       \
            reg##F &= ~_BV(pin & 7);   \
        }                              \
    }
#else
#define _PIN_IF_F(pin, condition, reg) \
    if (0) {}
#endif

#define gpio_enable(pin, direction)                                                                                                                                                                   \
    do {                                                                                                                                                                                              \
        if (0) {                                                                                                                                                                                      \
        } else                                                                                                                                                                                        \
            _PIN_IF_A(pin, !direction, TRIS)                                                                                                                                                          \
        else _PIN_IF_B(pin, !direction, TRIS) else _PIN_IF_C(pin, !direction, TRIS) else _PIN_IF_D(pin, !direction, TRIS) else _PIN_IF_E(pin, !direction, TRIS) else _PIN_IF_F(pin, !direction, TRIS) \
    } while (0)

#ifdef LATA
/* pics that have LATx */
#define gpio_set(pin, state)                                                                                                                                            \
    do {                                                                                                                                                                \
        if (0) {                                                                                                                                                        \
        } else                                                                                                                                                          \
            _PIN_IF_A(pin, state, LAT)                                                                                                                                  \
        else _PIN_IF_B(pin, state, LAT) else _PIN_IF_C(pin, state, LAT) else _PIN_IF_D(pin, state, LAT) else _PIN_IF_E(pin, state, LAT) else _PIN_IF_F(pin, state, LAT) \
    } while (0)
#else
/* pics that do not have LATx */
#define gpio_set(pin, state)                                                                                                                                                 \
    do {                                                                                                                                                                     \
        if (0) {                                                                                                                                                             \
        } else                                                                                                                                                               \
            _PIN_IF_A(pin, state, PORT)                                                                                                                                      \
        else _PIN_IF_B(pin, state, PORT) else _PIN_IF_C(pin, state, PORT) else _PIN_IF_D(pin, state, PORT) else _PIN_IF_E(pin, state, PORT) else _PIN_IF_F(pin, state, PORT) \
    } while (0)
#endif

static inline uint8_t gpio_read(uint8_t pin)
{
    if (0) {
    }
#ifdef PORTA
    else if ((pin & 0xf8) == 0x00) {
        return PORTA & _BV(pin & 7);
    }
#endif
#ifdef PORTB
    else if ((pin & 0xf8) == 0x08) {
        return PORTB & _BV(pin & 7);
    }
#endif
#ifdef PORTC
    else if ((pin & 0xf8) == 0x10) {
        return PORTC & _BV(pin & 7);
    }
#endif
#ifdef PORTD
    else if ((pin & 0xf8) == 0x18) {
        return PORTD & _BV(pin & 7);
    }
#endif
#ifdef PORTE
    else if ((pin & 0xf8) == 0x20) {
        return PORTE & _BV(pin & 7);
    }
#endif
#ifdef PORTF
    else if ((pin & 0xf8) == 0x28) {
        return PORTF & _BV(pin & 7);
    }
#endif
    return 0;
}

#ifdef WPUA
#define gpio_pullup(pin, enable)                                                                                                                                             \
    do {                                                                                                                                                                     \
        if (0) {                                                                                                                                                             \
        } else                                                                                                                                                               \
            _PIN_IF_A(pin, enable, WPU)                                                                                                                                      \
        else _PIN_IF_B(pin, enable, WPU) else _PIN_IF_C(pin, enable, WPU) else _PIN_IF_D(pin, enable, WPU) else _PIN_IF_E(pin, enable, WPU) else _PIN_IF_F(pin, enable, WPU) \
    } while (0)
#else
#define gpio_pullup(pin, enable)
#endif

#ifdef ODCONA
#define gpio_open_drain(pin, enable)                                                                                                                                                   \
    do {                                                                                                                                                                               \
        if (0) {                                                                                                                                                                       \
        } else                                                                                                                                                                         \
            _PIN_IF_A(pin, enable, ODCON)                                                                                                                                              \
        else _PIN_IF_B(pin, enable, ODCON) else _PIN_IF_C(pin, enable, ODCON) else _PIN_IF_D(pin, enable, ODCON) else _PIN_IF_E(pin, enable, ODCON) else _PIN_IF_F(pin, enable, ODCON) \
    } while (0)
#else
#define gpio_open_drain(pin, enable)
#endif


#ifdef __cplusplus
}
#endif

#endif /* _TARGET_PIC_GPIO_H_ */
