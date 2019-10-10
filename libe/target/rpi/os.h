/*
 * rpi specific os routines.
 */

#ifndef _TARGET_RPI_OS_H_
#define _TARGET_RPI_OS_H_

#ifdef __cplusplus
extern "C" {
#endif

int os_init(void);
void os_quit(void);
#define os_wdt_reset()

#define os_delay_ms(x)      os_sleepf((os_time_t)x / 1000.0)
#define os_delay_us(x)      os_sleepf((os_time_t)x / 1000.0 / 1000.0)

#ifdef __cplusplus
}
#endif

#endif /* _TARGET_RPI_OS_H_ */
