/*
 * libe cross-platform library: Logging
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifdef USE_LOG

#include <stdarg.h>
#include <libe/libe.h>

void putch(char ch)
{
#ifdef TX1IF
	/* wait till the transmitter register becomes empty */
	while (!TX1IF);
	/* clear transmitter flag and send character */
	TX1IF = 0;
	TX1REG = ch;
#else
	/* wait till the transmitter register becomes empty */
	while (!TXIF);
	/* clear transmitter flag and send character */
	TXIF = 0;
	TXREG = ch;
#endif
}

int log_init(void *context, uint32_t baud)
{
	/* default baud */
	if (baud < 1) {
		baud = 38400;
	}

	/* if device supports mapping of pins, TX to RC6 as default */
#ifdef RC6PPS
	RC6PPS = 0x14;
#endif

	/* enable uart transmit with baud rate multiplier and calculate baud rate setting */
#ifdef BAUD1CON
	BAUD1CON = 0x08;
#endif
#ifdef TX1STA
	TX1STA = 0x24;
	RC1STA = 0x80;
	SP1BRG = _XTAL_FREQ / (long)(4 * (baud + 1));
#else
	TXSTA = 0x24;
	RCSTA = 0x80;
	SPBRG = _XTAL_FREQ / (long)(4 * (baud + 1));
#endif

	return 0;
}

void log_quit(void)
{
#ifdef TX1STA
	TX1STA = 0;
	SP1BRG = 0;
#else
	TXSTA = 0;
	SPBRG = 0;
#endif
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

#endif
