/*
 * PWM driver for ESP32.
 */

#include <libe/libe.h>
#include <driver/ledc.h>

#ifndef PWM_DEFAULT_FREQUENCY
#define PWM_DEFAULT_FREQUENCY 1000
#endif


int pwm_open(struct pwm *pwm, void *context, uint32_t frequency, uint8_t resolution)
{
	if (frequency < 1) {
		frequency = PWM_DEFAULT_FREQUENCY;
	}
	if (resolution  < 1) {
		/* 8 bits as default */
		resolution = LEDC_TIMER_8_BIT;
	} else if (resolution > LEDC_TIMER_20_BIT) {
		resolution = LEDC_TIMER_20_BIT;
	}

	memset(pwm, 0, sizeof(*pwm));

	ledc_timer_config_t ledc_timer = {
		.duty_resolution = resolution,
		.freq_hz = frequency,
		.speed_mode = LEDC_HIGH_SPEED_MODE,
		.timer_num = LEDC_TIMER_0,
		.clk_cfg = LEDC_AUTO_CLK,
	};
	IF_R(ledc_timer_config(&ledc_timer) != ESP_OK, -1);
	ledc_fade_func_install(0);

	pwm->max_value = (1 << resolution) - 1;

	return 0;
}

void pwm_close(struct pwm *pwm)
{
	for (uint8_t i = 1; i; i = i << 1) {
		if (pwm->ch_used & i) {
			ledc_stop(LEDC_HIGH_SPEED_MODE, i, 0);
		}
	}
	pwm->ch_used = 0;
}

int pwm_channel_enable(struct pwm *pwm, uint8_t channel, uint8_t pin)
{
	if (channel > 7) {
		/* channel 0-7 supported */
		return -1;
	}

	ledc_channel_config_t ch_cfg = {
		.channel = channel,
		.duty = 0,
		.gpio_num = pin,
		.speed_mode = LEDC_HIGH_SPEED_MODE,
		.hpoint = 0,
		.timer_sel = LEDC_TIMER_0
	};
	ledc_channel_config(&ch_cfg);

	pwm->ch_used |= (1 << channel);

	return 0;
}

void pwm_set_duty(struct pwm *pwm, uint8_t channel, uint32_t duty)
{
	if (pwm->max_value < duty) {
		duty = pwm->max_value;
	}
	ledc_set_duty_and_update(LEDC_HIGH_SPEED_MODE, channel, duty , 0);
}

void pwm_set_dutyf(struct pwm *pwm, uint8_t channel, float duty)
{
	if (duty > 1) {
		duty = 1;
	} else if (duty < 0) {
		duty = 0;
	}
	ledc_set_duty_and_update(LEDC_HIGH_SPEED_MODE, channel, (uint32_t)((float)pwm->max_value * duty), 0);
}
