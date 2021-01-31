/*
 * Simple button helper macros for gpio attached buttons.
 */

#ifndef _LIBE_BUTTON_H_
#define _LIBE_BUTTON_H_
#ifdef USE_GPIO

#ifdef __cplusplus
extern "C" {
#endif

#define button_init(pin)            os_time_t _button_gpio_t_##pin = -1; bool _button_gpio_state_##pin = false; gpio_input(pin); gpio_pullup(pin, true);

#define button_pressed(pin, timeout) \
	_button_gpio_state_##pin = gpio_read(pin) ? false : true; \
	if (!_button_gpio_state_##pin) { _button_gpio_t_##pin = -1; } \
	else if (_button_gpio_t_##pin < 0) { _button_gpio_t_##pin = os_timef() + timeout / 1000; } \
	else if (_button_gpio_t_##pin < os_timef() && _button_gpio_t_##pin > 0)

#define button_repeat(pin, repeat)  do { if (repeat > 0) { _button_gpio_t_##pin += repeat / 1000; } else { _button_gpio_t_##pin = 0; } } while(0)

#ifdef __cplusplus
}
#endif

#endif
#endif /* _LIBE_BUTTON_H_ */
