/*
 * PWM
 */

#ifndef _LIBE_PWM_H_
#define _LIBE_PWM_H_

#include <stdint.h>

/* pre-define for following includes, these cannot be fully declared here yet */
struct pwm;

#ifdef TARGET_AVR
// #include "target/avr/pwm.h"
#elif TARGET_ESP32
#include "target/esp32/pwm.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct pwm {
#ifdef PWM_NEED_8BIT_CHANNEL_USAGE
	uint8_t ch_used;
#endif
#ifdef PWM_NEED_FREQUENCY
	uint32_t frequency;
#endif
#ifdef PWM_NEED_RESOLUTION
	uint8_t resolution;
#endif
#ifdef PWM_NEED_MAX_VALUE
	uint32_t max_value;
#endif
};

int pwm_open(struct pwm *pwm, void *context, uint32_t frequency, uint8_t resolution);
void pwm_close(struct pwm *pwm);

int pwm_channel_enable(struct pwm *pwm, uint8_t channel, uint8_t pin);
void pwm_set_duty(struct pwm *pwm, uint8_t channel, uint32_t duty);
void pwm_set_dutyf(struct pwm *pwm, uint8_t channel, float duty);


#ifdef __cplusplus
}
#endif

#endif /* _LIBE_PWM_H_ */
