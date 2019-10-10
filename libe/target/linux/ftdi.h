/*
 * libe cross-platform library: FTDI FTx232H device support
 *
 * Supports maximum of four of these devices in any combination:
 *  - FT232H
 *  - FT2232H
 *  - FT4232H
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifndef _TARGET_LINUX_FTDI_H_
#define _TARGET_LINUX_FTDI_H_
#ifdef USE_FTDI

#include <stdint.h>
#include <stdbool.h>
#include <libftdi1/ftdi.h>

#ifdef __cplusplus
extern "C" {
#endif

#define OS_FTDI_GPIO_0_TO_63        0
#define OS_FTDI_GPIO_64_TO_127      1
#define OS_FTDI_GPIO_128_TO_191     2
#define OS_FTDI_GPIO_192_TO_255     3

void os_ftdi_init(void);
void os_ftdi_quit(void);

int os_ftdi_use(int pin_range, uint16_t vid, uint16_t pid, const char *description, const char *serial);
int os_ftdi_set_mpsse(int pin);
int os_ftdi_get_mode(int pin);
struct ftdi_context *os_ftdi_get_context(int pin);
int os_ftdi_has_pin(uint8_t pin);

struct ftdi_context *ftdi_direct_open(uint16_t vid, uint16_t pid, int interface, const char *description, const char *serial, int reset);

void os_ftdi_gpio_enable(uint8_t pin, bool direction);
void os_ftdi_gpio_set(uint8_t pin, uint8_t state);
uint8_t os_ftdi_gpio_read(uint8_t pin);


#ifdef __cplusplus
}
#endif

#endif /* USE_FTDI */
#endif /* _TARGET_LINUX_FTDI_H_ */
