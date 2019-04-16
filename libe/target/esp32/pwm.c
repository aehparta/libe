/*
 * Basic pwm driver for esp32
 */

#ifdef USE_PWM

#include <driver/mcpwm.h>
#include <libe/pwm.h>

static uint32_t pwm_frequency = 1000;
static uint16_t pwm_channels = 0;


int pwm_init(void *context, uint32_t frequency)
{
	if (frequency < 1) {
		frequency = 1000;
	}
	pwm_frequency = frequency;
	return 0;
}

int pwm_init_channel(uint8_t channel, uint8_t pin, float duty)
{
	switch (channel) {
	case 0:
		mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, pin);
		break;
	case 1:
		mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, pin);
		break;
	case 2:
		mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1A, pin);
		break;
	case 3:
		mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1B, pin);
		break;
	case 4:
		mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM2A, pin);
		break;
	case 5:
		mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM2B, pin);
		break;
	case 6:
		mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM0A, pin);
		break;
	case 7:
		mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM0B, pin);
		break;
	case 8:
		mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM1A, pin);
		break;
	case 9:
		mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM1B, pin);
		break;
	case 10:
		mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM2A, pin);
		break;
	case 11:
		mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM2B, pin);
		break;
	default:
		return -1;
	}

	mcpwm_config_t cfg;
	cfg.frequency = pwm_frequency;
	cfg.cmpr_a = 0;
	cfg.cmpr_b = 0;
	cfg.counter_mode = MCPWM_UP_COUNTER;
	cfg.duty_mode = MCPWM_DUTY_MODE_0;

	switch (channel) {
	case 0:
	case 1:
		mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &cfg);
		break;
	case 2:
	case 3:
		mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_1, &cfg);
		break;
	case 4:
	case 5:
		mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_2, &cfg);
		break;
	case 6:
	case 7:
		mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_0, &cfg);
		break;
	case 8:
	case 9:
		mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_1, &cfg);
		break;
	case 10:
	case 11:
		mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_2, &cfg);
		break;
	}

	pwm_channels |= (1 << channel);
	pwm_set_duty(channel, duty);

	return 0;
}

void pwm_set_duty(uint8_t channel, float duty)
{
	if (!(pwm_channels & (1 << channel))) {
		return;
	}

	duty *= 100.0;
	
	switch (channel) {
	case 0:
		mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, duty);
		break;
	case 1:
		mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, duty);
		break;
	case 2:
		mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_A, duty);
		break;
	case 3:
		mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_B, duty);
		break;
	case 4:
		mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_OPR_A, duty);
		break;
	case 5:
		mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_OPR_B, duty);
		break;
	case 6:
		mcpwm_set_duty(MCPWM_UNIT_1, MCPWM_TIMER_0, MCPWM_OPR_A, duty);
		break;
	case 7:
		mcpwm_set_duty(MCPWM_UNIT_1, MCPWM_TIMER_0, MCPWM_OPR_B, duty);
		break;
	case 8:
		mcpwm_set_duty(MCPWM_UNIT_1, MCPWM_TIMER_1, MCPWM_OPR_A, duty);
		break;
	case 9:
		mcpwm_set_duty(MCPWM_UNIT_1, MCPWM_TIMER_1, MCPWM_OPR_B, duty);
		break;
	case 10:
		mcpwm_set_duty(MCPWM_UNIT_1, MCPWM_TIMER_2, MCPWM_OPR_A, duty);
		break;
	case 11:
		mcpwm_set_duty(MCPWM_UNIT_1, MCPWM_TIMER_2, MCPWM_OPR_B, duty);
		break;
	}
}

#endif
