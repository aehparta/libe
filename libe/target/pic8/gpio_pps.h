/*
 * pic8 specific gpio.
 */

#ifndef _TARGET_PIC_GPIO_PPS_H_
#define _TARGET_PIC_GPIO_PPS_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(RA0PPS) || defined(RB0PPS) || defined(RC0PPS)
#define GPIO_HAS_PPS

#ifdef MCU_VARIANT_16F183XX
#define GPIO_PPS_TX 0x14
#elif MCU_VARIANT_16F184XX
#define GPIO_PPS_TX 0x0f
#define GPIO_PPS_I2C_1_SDA 0x14
#define GPIO_PPS_I2C_1_SCL 0x13
#define GPIO_PPS_I2C_2_SDA 0x16
#define GPIO_PPS_I2C_2_SCL 0x15
#endif

#endif

#ifdef __cplusplus
}
#endif

#endif /* _TARGET_PIC_GPIO_PPS_H_ */
