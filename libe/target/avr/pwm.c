/*
 * Basic pwm driver for avr
 */

#ifdef USE_PWM

#include <stdint.h>
#include <libe/pwm.h>
#include <libe/os.h>
#include <libe/log.h>

static uint8_t pwm_prescaling = 0x05;
static uint8_t pwm_channels = 0;


int pwm_init(void *context, uint32_t frequency)
{
	uint16_t prescalers[] = { 0, 1, 8, 64, 256, 1024, 0 };
	float diff;
	uint8_t i;

	if (frequency < 1) {
		frequency = 1000;
	}
	/* calculate closest prescaler */
	for (diff = 1e9, i = 1; prescalers[i]; i++) {
		float d = (float)fabs((float)F_CPU / 255.0 / (float)prescalers[i] - (float)frequency) / (float)frequency;
		if (diff > d) {
			diff = d;
			pwm_prescaling = i;
		}
	}

	DEBUG_MSG("pwm frequency: %f, prescaler: %d", (float)F_CPU / 255.0 / (float)prescalers[pwm_prescaling], prescalers[pwm_prescaling]);

	return 0;
}

int pwm_init_channel(uint8_t channel, uint8_t pin, float duty)
{
	/* use fast pwm mode, 8-bits, non-inverting */
	switch (channel) {
	case 0:
		os_gpio_output(30);
		os_gpio_low(30);
		TCCR0A = (TCCR0A & 0x30) | 0x83;
		TCCR0B = pwm_prescaling;
		OCR0A = 255.0 * duty;
		break;
	case 1:
		os_gpio_output(29);
		os_gpio_low(29);
		TCCR0A = (TCCR0A & 0xc0) | 0x23;
		TCCR0B = pwm_prescaling;
		OCR0B = 255.0 * duty;
		break;
	default:
		return -1;
	}

	pwm_channels |= (1 << channel);

	return 0;
}

void pwm_set_duty(uint8_t channel, float duty)
{
	if (!(pwm_channels & (1 << channel))) {
		return;
	}

	switch (channel) {
	case 0:
		OCR0A = 255.0 * duty;
		break;
	case 1:
		OCR0B = 255.0 * duty;
		break;
	}
}

#endif
