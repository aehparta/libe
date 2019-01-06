/*
 * Basic pwm driver for msp430
 */

/* not implemented */

#include <libe/pwm.h>


int pwm_init(void *context, uint32_t frequency)
{
	return 0;
}

int pwm_init_channel(uint8_t channel, uint8_t pin, float duty)
{
	return -1;
}

void pwm_set_duty(uint8_t channel, float duty)
{

}
