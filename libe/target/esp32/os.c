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


int8_t os_init(void)
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
