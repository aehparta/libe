/*
 * esp32 gpio.
 */

void os_gpio_enable(uint8_t pin, bool direction)
{
	gpio_pad_select_gpio(pin);
	gpio_set_direction(pin, direction ? GPIO_MODE_OUTPUT : GPIO_MODE_INPUT);
	return 0;
}

void os_gpio_set(uint8_t pin, uint8_t state)
{
	gpio_set_level(pin, state ? 1 : 0);
	return 0;
}

uint8_t os_gpio_read(uint8_t pin)
{
	return gpio_get_level(pin);
}

void os_gpio_pullup(uint8_t pin, bool enable)
{
	return gpio_set_pull_mode(pin, GPIO_PULLUP_ONLY);
}

void os_gpio_pulldown(uint8_t pin, bool enable)
{
	return gpio_set_pull_mode(pin, GPIO_PULLDOWN_ONLY);
}

