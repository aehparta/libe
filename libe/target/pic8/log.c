/*
 * libe cross-platform library: Logging
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include <stdarg.h>
#include <libe/libe.h>


void putch(char ch)
{
    /* wait till the transmitter register becomes empty */
    while (!TXIF)
        ;
    /* clear transmitter flag and send character */
    TXIF = 0;
    TXREG = ch;
}

int log_init(void)
{
    TXSTA = 0x24;
    RCSTA = 0x80;
    SPBRG = F_CPU / (16UL * (LOG_BAUD + 1UL));

    return 0;
}

void log_quit(void)
{
    TXSTA = 0;
    RCSTA = 0;
    SPBRG = 0;
}

void log_msg(int level, const char *file, int line, const char *func, const char *msg, ...)
{
    if (level == LOG_LEVEL_DEBUG) {
        LOG_PRINTF(LDC_PURPLE "D:" LDC_DEFAULT);
    } else if (level == LOG_LEVEL_INFO) {
        LOG_PRINTF(LDC_BLUE "I:" LDC_DEFAULT);
    } else if (level == LOG_LEVEL_NOTICE) {
        LOG_PRINTF(LDC_CYAN "N:" LDC_DEFAULT);
    } else if (level == LOG_LEVEL_WARN) {
        LOG_PRINTF(LDC_YELLOW "W:" LDC_DEFAULT);
    } else if (level == LOG_LEVEL_ERR) {
        LOG_PRINTF(LDC_RED "E:" LDC_DEFAULT);
    } else if (level == LOG_LEVEL_CRIT) {
        LOG_PRINTF(LDC_REDB "C:" LDC_DEFAULT);
    }

    LOG_PRINTF("%s", msg);
    LOG_PRINTF("\r\n");
}
