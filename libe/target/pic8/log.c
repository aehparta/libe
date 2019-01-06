/*
 * libe cross-platform library: Logging
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include <stdarg.h>
#include <libe/os.h>
#include <libe/log.h>
#include <libe/debug.h>


void putch(char ch)
{
	/* wait till the transmitter register becomes empty */
	while (!TXIF);
	/* clear transmitter flag and send character */
	TXIF = 0;
	TXREG = ch;
}

int getchar(void)
{
	/* wait till the data is received */
	while (!RCIF);
	/* clear receiver flag and read character */
	RCIF = 0;
	return RCREG;
}

int log_init(void *context, uint32_t baud)
{
	/* default baud */
	if (baud < 1) {
		baud = 38400;
	}
	/* enable uart with baud rate multiplier and calculate baud rate setting */
	TXSTA = 0x24;
	RCSTA = 0x90;
	SPBRG = (_XTAL_FREQ / (long)(16UL * baud)) - 1;
	return 0;
}

void log_quit(void)
{
	RCSTA = 0;
	TXSTA = 0;
	SPBRG = 0;
	os_gpio_input(22);
	os_gpio_input(23);
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

	va_list args;
	va_start(args, msg);
	vprintf(msg, args);
	va_end(args);
	LOG_PRINTF("\r\n");
}