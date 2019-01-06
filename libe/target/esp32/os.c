/*
 * libe cross-platform library: operating system/target specific routines
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include <unistd.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <libe/os.h>
#include <libe/debug.h>
#include <sdkconfig.h>


int os_init(void)
{
	srand((unsigned int)(os_timef() * 1000));
	return 0;
}

time_t os_timei(void)
{
	return (time_t)os_timef();
}

os_time_t os_timef(void)
{
	return (os_time_t)xTaskGetTickCount() * portTICK_PERIOD_MS / 1000;
}

void os_sleepi(time_t t)
{
	os_sleepf((os_time_t)t);
}

void os_sleepf(os_time_t t)
{
	vTaskDelay((TickType_t)(t * 1000 / portTICK_PERIOD_MS));
}

int os_gpio_enable(uint8_t pin, bool direction)
{
	gpio_pad_select_gpio(pin);
	gpio_set_direction(pin, direction ? GPIO_MODE_OUTPUT : GPIO_MODE_INPUT);
	return 0;
}


int os_gpio_output(uint8_t pin)
{
	return os_gpio_enable(pin, OS_GPIO_OUTPUT);
}


int os_gpio_input(uint8_t pin)
{
	return os_gpio_enable(pin, OS_GPIO_INPUT);
}


int os_gpio_set(uint8_t pin, bool state)
{
	gpio_set_level(pin, state ? 1 : 0);
	return 0;
}


int os_gpio_high(uint8_t pin)
{
	return os_gpio_set(pin, true);
}


int os_gpio_low(uint8_t pin)
{
	return os_gpio_set(pin, false);
}

int os_gpio_read(uint8_t pin)
{
	return gpio_get_level(pin);
}