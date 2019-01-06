/*
 * libe cross-platform library: Logging
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include <libe/os.h>
#include <libe/log.h>
#include <libe/debug.h>


static int log_putchar(char c, FILE *stream)
{
	/* wait for empty transmit buffer and then send data */
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
	return 0;
}

int log_init(void *context, uint32_t baud)
{
	/* default baud */
	if (baud < 1) {
		baud = 38400;
	}

	/* calculate ubrr register value from baud */
	UBRR0 = (F_CPU / (16 * baud) - 1) & 0x0fff;

	/* disable double */
	UCSR0A = 0;

	/* enable receiver and transmitter */
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	/* set frame format: 8N1 */
	UCSR0C = (0x3 << UCSZ00);

	static FILE log_stdout = FDEV_SETUP_STREAM(log_putchar, NULL, _FDEV_SETUP_WRITE);
	stdout = &log_stdout;
	
	return 0;
}

void log_quit(void)
{
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
	vfprintf_P(stdout, msg, args);
	va_end(args);
	LOG_PRINTF("\r\n");
}
