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

struct ftdi_context *ftdi_direct_open(uint16_t vid, uint16_t pid, int interface, const char *description, const char *serial, int reset);

/* enable ftdi gpio only in x86 pc platform since ftdi could be used
 * on other platforms that already have another way for gpio,
 * like for example raspberry pi has
 */
#ifdef TARGET_X86
#define os_gpio_enable(pin, direction) os_ftdi_gpio_enable(pin, direction)
#define os_gpio_set(pin, state) os_ftdi_gpio_set(pin, state)
#define os_gpio_read(pin) os_ftdi_gpio_read(pin)
#endif


#ifdef __cplusplus
}
#endif

#endif /* USE_FTDI */
#endif /* _TARGET_LINUX_FTDI_H_ */
