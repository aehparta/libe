/*
 * linux specific gpio.
 */

#ifndef _TARGET_LINUX_GPIO_H_
#define _TARGET_LINUX_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

void gpio_enable(uint8_t pin, bool direction);
void gpio_set(uint8_t pin, uint8_t state);
uint8_t gpio_read(uint8_t pin);

#ifdef __cplusplus
}
#endif

#endif /* _TARGET_RPI_GPIO_H_ */
