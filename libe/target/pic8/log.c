/*
 * libe cross-platform library: Logging
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include <stdarg.h>
#include <libe/libe.h>

/* baud rate */
#ifndef PIC8_LOG_BAUD
#define PIC8_LOG_BAUD LOG_BAUD
#endif

/* RC6 as default TX pin */
#if !defined(PIC8_LOG_TX_PPS) && defined(RC6PPS)
#define PIC8_LOG_TX_PPS RC6PPS
/* works for some */
#ifndef PIC8_LOG_TX_PPS_VALUE
#define PIC8_LOG_TX_PPS_VALUE 0x14
#endif
#endif

/* registers */

#ifndef PIC8_LOG_TXIF
#define PIC8_LOG_TXIF TXIF
#endif

#ifndef PIC8_LOG_TXREG
#define PIC8_LOG_TXREG TXREG
#endif

#ifndef PIC8_LOG_TXSTA
#define PIC8_LOG_TXSTA TXSTA
#endif

#ifndef PIC8_LOG_RCSTA
#define PIC8_LOG_RCSTA RCSTA
#endif

#ifndef PIC8_LOG_SPBRG
#define PIC8_LOG_SPBRG SPBRG
#endif

#ifndef PIC8_LOG_BAUDCON
#ifdef BAUDCON1
#define PIC8_LOG_BAUDCON BAUDCON1
#endif
#endif


void putch(char ch)
{
	/* wait till the transmitter register becomes empty */
	while (!PIC8_LOG_TXIF);
	/* clear transmitter flag and send character */
	PIC8_LOG_TXIF = 0;
	PIC8_LOG_TXREG = ch;
}

int log_init(void)
{
	/* if device supports mapping of pins */
#ifdef PIC8_LOG_TX_PPS
	PIC8_LOG_TX_PPS = PIC8_LOG_TX_PPS_VALUE;
#endif

	/* enable uart transmit only and calculate baud rate setting */
#ifdef PIC8_LOG_BAUDCON
	PIC8_LOG_BAUDCON = 0x00;
#endif
	PIC8_LOG_TXSTA = 0x24;
	PIC8_LOG_RCSTA = 0x80;
	PIC8_LOG_SPBRG = _XTAL_FREQ / (16UL * (PIC8_LOG_BAUD + 1UL));

	return 0;
}

void log_quit(void)
{
	PIC8_LOG_TXSTA = 0;
	PIC8_LOG_RCSTA = 0;
	PIC8_LOG_SPBRG = 0;
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

	// va_list args;
	// va_start(args, msg);
	// vprintf(NULL, msg, args);
	// va_end(args);
	LOG_PRINTF(msg);
	LOG_PRINTF("\r\n");
}
