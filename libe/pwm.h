/*
 * PWM
 */

#ifndef _PWM_H_
#define _PWM_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int pwm_init(void *context, uint32_t frequency);
int pwm_init_channel(uint8_t channel, uint8_t pin, float duty);
void pwm_set_duty(uint8_t channel, float duty);


#ifdef __cplusplus
}
#endif

#endif /* _PWM_H_ */
