/*
 * libe cross-platform library
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_GPIO_H_
#define _LIBE_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#define GPIO_INPUT   0
#define GPIO_OUTPUT  1

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

#define gpio_output(pin) gpio_enable(pin, GPIO_OUTPUT)
#define gpio_input(pin) gpio_enable(pin, GPIO_INPUT)
#define gpio_high(pin) gpio_set(pin, 1)
#define gpio_low(pin) gpio_set(pin, 0)

/* gpio drivers */
#include "drivers/gpio/hd44780.h"


#ifdef __cplusplus
}
#endif

#endif /* _LIBE_GPIO_H_ */
