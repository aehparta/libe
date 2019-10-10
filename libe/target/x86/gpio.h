/*
 * x86 specific gpio.
 */

#ifndef _TARGET_GPIO_H_
#define _TARGET_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_FTDI
#define gpio_enable(pin, direction) os_ftdi_gpio_enable(pin, direction)
#define gpio_set(pin, state) os_ftdi_gpio_set(pin, state)
#define gpio_read(pin) os_ftdi_gpio_read(pin)
#else
#warning "no gpio available, empty macros being used for gpio_*() functions"
#define gpio_enable(pin, direction)
#define gpio_set(pin, state)
#define gpio_read(pin)
#endif

#ifdef __cplusplus
}
#endif

#endif /* _TARGET_GPIO_H_ */
