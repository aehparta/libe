/*
 * libe cross-platform library: operating system/target specific routines
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include <unistd.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <libe/libe.h>
#include <sdkconfig.h>


int os_init(void)
{
	srand((unsigned int)(os_timef() * 1000));
	return 0;
}

void os_quit(void)
{

}

time_t os_timei(void)
{
	return (time_t)os_timef();
}

os_time_t os_timef(void)
{
	return (os_time_t)esp_timer_get_time() / 1e6L;
}

void os_sleepi(time_t t)
{
	os_sleepf((os_time_t)t);
}

void os_sleepf(os_time_t t)
{
	vTaskDelay((TickType_t)(t * 1000 / portTICK_PERIOD_MS));
}

int8_t os_gpio_enable(uint8_t pin, bool direction)
{
	gpio_pad_select_gpio(pin);
	gpio_set_direction(pin, direction ? GPIO_MODE_OUTPUT : GPIO_MODE_INPUT);
	return 0;
}

int8_t os_gpio_set(uint8_t pin, bool state)
{
	gpio_set_level(pin, state ? 1 : 0);
	return 0;
}

int8_t os_gpio_read(uint8_t pin)
{
	return gpio_get_level(pin);
}

int8_t os_gpio_pullup(uint8_t pin)
{
	return gpio_set_pull_mode(pin, GPIO_PULLUP_ONLY);
}

int8_t os_gpio_pulldown(uint8_t pin)
{
	return gpio_set_pull_mode(pin, GPIO_PULLDOWN_ONLY);
}

int8_t os_gpio_floating(uint8_t pin)
{
	return gpio_set_pull_mode(pin, GPIO_FLOATING);
}
