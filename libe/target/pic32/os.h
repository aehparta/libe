/*
 * 32-bit pic specific os routines as macros.
 */

#ifndef _TARGET_PIC32_OS_H_
#define _TARGET_PIC32_OS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <xc.h>

#define _BV(_PIN) (1 << (_PIN))

int os_init(void);
#define os_quit()
#define os_wdt_reset()    \
	do {                  \
		WDTCONSET = 0x01; \
	} while (0)

void os_delay_ms(uint32_t ms);
void os_delay_us(uint32_t us);

#ifdef __cplusplus
}
#endif

#endif /* _TARGET_PIC32_OS_H_ */
