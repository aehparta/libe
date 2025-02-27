/*
 * libe cross-platform library: Logging
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include <stdarg.h>
#include <libe/libe.h>


#if defined(PIC8_LOG_USART) && PIC8_LOG_USART == 0

/* leave empty for older defaults */

#elif PIC8_LOG_USART == 1

#define TXSTA       TX1STA
#define RCSTA       RC1STA
#define SPBRG       SP1BRG
#define TXREG       TX1REG
#define TXIF        TX1IF

#elif PIC8_LOG_USART == 2

#define TXSTA       TX2STA
#define RCSTA       RC2STA
#define SPBRG       SP2BRG
#define TXREG       TX2REG
#define TXIF        TX2IF

#else
#error "no log USART module selected, define PIC8_LOG_USART to 1 or 2, or 0 for older devices"
#endif


void putch(char ch)
{
    /* wait till the transmitter register becomes empty */
    while (!TXIF);
    /* clear transmitter flag and send character */
    TXIF = 0;
    TXREG = ch;
}

int log_init(void)
{
#if defined(GPIO_HAS_PPS) && defined(LOG_TX_PIN)
    gpio_pps(LOG_TX_PIN, GPIO_PPS_TX);
#endif

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
