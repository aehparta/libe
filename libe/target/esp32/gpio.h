/*
 * ESP32 specific gpio.
 */

#ifndef _TARGET_ESP32_GPIO_H_
#define _TARGET_ESP32_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <driver/gpio.h>

#define gpio_enable(pin, direction) do { gpio_pad_select_gpio(pin); gpio_set_direction(pin, direction ? GPIO_MODE_OUTPUT : GPIO_MODE_INPUT); } while(0);
#define gpio_set(pin, state) gpio_set_level(pin, state ? 1 : 0);
#define gpio_read(pin) gpio_get_level(pin)
// gpio_set_pull_mode(pin, GPIO_PULLUP_ONLY);

#ifdef __cplusplus
}
#endif

#endif /* _TARGET_ESP32_GPIO_H_ */
