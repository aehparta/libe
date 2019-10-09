/*
 * libe cross-platform library: Logging
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifdef USE_LOG

#include <libe/libe.h>

/* baud rate */
#ifndef AVR_LOG_BAUD
#define AVR_LOG_BAUD LOG_BAUD
#endif

#if defined(AVR_LOG_UDR)
/* already defined outside of this */
#elif defined(UDR0)
#define AVR_LOG_UCSRA  UCSR0A
#define AVR_LOG_UCSRB  UCSR0B
#define AVR_LOG_UCSRC  UCSR0C
#define AVR_LOG_UDRE   UDRE0
#define AVR_LOG_UDR    UDR0
#define AVR_LOG_UBRR   UBRR0
#define AVR_LOG_TXEN   TXEN0
#define AVR_LOG_RXEN   RXEN0
#define AVR_LOG_UCSZ   UCSZ00
#elif defined(UDR)
#define AVR_LOG_UCSRA  UCSRA
#define AVR_LOG_UCSRB  UCSRB
#define AVR_LOG_UCSRC  UCSRC
#define AVR_LOG_UDRE   UDRE
#define AVR_LOG_UDR    UDR
#define AVR_LOG_UBRR   UBRRL
#define AVR_LOG_TXEN   TXEN
#define AVR_LOG_RXEN   RXEN
#define AVR_LOG_UCSZ   UCSZ0
#else
#error "no support for serial port in this mcu, maybe disable logging with USE=NOT_LOG"
#endif


static int log_putchar(char c, FILE *stream)
{
	/* wait for empty transmit buffer and then send data */
	while (!(AVR_LOG_UCSRA & (1 << AVR_LOG_UDRE)));
	AVR_LOG_UDR = c;
	return 0;
}

int log_init(void)
{
	/* calculate ubrr register value from baud */
	AVR_LOG_UBRR = (F_CPU / (16UL * AVR_LOG_BAUD) - 1UL) & 0x0fff;

	/* disable double */
	AVR_LOG_UCSRA = 0;

	/* enable receiver and transmitter */
	AVR_LOG_UCSRB = (1 << AVR_LOG_RXEN) | (1 << AVR_LOG_TXEN);

	/* set frame format: 8N1 */
#ifdef URSEL
	AVR_LOG_UCSRC = (1 << URSEL) | (0x3 << AVR_LOG_UCSZ);
#else
	AVR_LOG_UCSRC = (0x3 << AVR_LOG_UCSZ);
#endif

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

#endif
