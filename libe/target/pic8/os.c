/*
 * libe cross-platform library: operating system/target specific routines
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include <math.h>
#include <libe/os.h>
#include <libe/log.h>


int8_t os_init(void)
{
    /* all as digital outputs as default */
#ifdef TRISA
    TRISA = 0x00;
#endif
#ifdef ANSELA
    ANSELA = 0x00;
#endif
#ifdef TRISB
    TRISB = 0x00;
#endif
#ifdef ANSELB
    ANSELB = 0x00;
#endif
#ifdef TRISC
    TRISC = 0x00;
#endif
#ifdef ANSELC
    ANSELC = 0x00;
#endif
#ifdef TRISD
    TRISD = 0x00;
#endif
#ifdef ANSELD
    ANSELD = 0x00;
#endif
#ifdef TRISE
    TRISE = 0x00;
#endif
#ifdef ANSELE
    ANSELE = 0x00;
#endif
#ifdef TRISF
    TRISF = 0x00;
#endif
#ifdef ANSELF
    ANSELF = 0x00;
#endif
    return 0;
}

time_t os_timei(void)
{
    WARN_MSG("not implemented");
    return 0;
}

os_time_t os_timef(void)
{
    WARN_MSG("not implemented");
    return 0;
}

void os_sleepi(time_t t)
{
    for (time_t i = 0; i < t; i++) {
        os_delay_ms(999);
    }
}

void os_sleepf(os_time_t t)
{
    for (int i = (int)(t * 1000); i > 0; i--) {
        os_delay_us(830);
    }
}
