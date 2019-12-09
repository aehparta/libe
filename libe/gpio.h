/*
 * libe cross-platform library
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_GPIO_H_
#define _LIBE_GPIO_H_
#ifdef USE_GPIO

#ifdef __cplusplus
extern "C" {
#endif

#ifdef TARGET_AVR
#include "target/avr/gpio.h"
#elif TARGET_PIC8
#include "target/pic8/gpio.h"
#elif TARGET_PIC16
#include "target/pic16/gpio.h"
#elif TARGET_PIC32
#include "target/pic32/gpio.h"
#elif TARGET_MSP430
#include "target/msp430/gpio.h"
#define time_t uint32_t
#elif TARGET_ESP32
#include "target/esp32/gpio.h"
#elif TARGET_X86
#include <signal.h>
#include "target/x86/gpio.h"
#elif TARGET_RPI
#include <signal.h>
#include "target/rpi/gpio.h"
#endif

#define gpio_input(pin) gpio_enable(pin, 0)
#define gpio_output(pin) gpio_enable(pin, 1)
#define gpio_low(pin) gpio_set(pin, 0)
#define gpio_high(pin) gpio_set(pin, 1)

#ifndef gpio_pullup
#define gpio_pullup(pin, state)
#endif

#ifndef gpio_open_drain
#define gpio_open_drain(pin, state)
#endif

#define GPIOA0      0
#define GPIOA1      1
#define GPIOA2      2
#define GPIOA3      3
#define GPIOA4      4
#define GPIOA5      5
#define GPIOA6      6
#define GPIOA7      7

#define GPIOB0      8
#define GPIOB1      9
#define GPIOB2      10
#define GPIOB3      11
#define GPIOB4      12
#define GPIOB5      13
#define GPIOB6      14
#define GPIOB7      15

#define GPIOC0      16
#define GPIOC1      17
#define GPIOC2      18
#define GPIOC3      19
#define GPIOC4      20
#define GPIOC5      21
#define GPIOC6      22
#define GPIOC7      23

#define GPIOD0      24
#define GPIOD1      25
#define GPIOD2      26
#define GPIOD3      27
#define GPIOD4      28
#define GPIOD5      29
#define GPIOD6      30
#define GPIOD7      31

#define GPIOE0      32
#define GPIOE1      33
#define GPIOE2      34
#define GPIOE3      35
#define GPIOE4      36
#define GPIOE5      37
#define GPIOE6      38
#define GPIOE7      39

#define GPIOF0      40
#define GPIOF1      41
#define GPIOF2      42
#define GPIOF3      43
#define GPIOF4      44
#define GPIOF5      45
#define GPIOF6      46
#define GPIOF7      47

#define GPIOG0      48
#define GPIOG1      49
#define GPIOG2      50
#define GPIOG3      51
#define GPIOG4      52
#define GPIOG5      53
#define GPIOG6      54
#define GPIOG7      55

#define GPIOH0      56
#define GPIOH1      57
#define GPIOH2      58
#define GPIOH3      59
#define GPIOH4      60
#define GPIOH5      61
#define GPIOH6      62
#define GPIOH7      63

/* gpio drivers */
#include "drivers/gpio/hd44780.h"


#ifdef __cplusplus
}
#endif

#endif /* USE_GPIO */
#endif /* _LIBE_GPIO_H_ */
