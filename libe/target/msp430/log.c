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


// static int printchar(char c, FILE *stream)
// {
// 	return 0;
// }

int log_init(void *context, uint32_t baud)
{
	/* default baud */
	if (baud < 1) {
		baud = 38400;
	}

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
	vfprintf(stdout, msg, args);
	va_end(args);
	LOG_PRINTF("\r\n");
}
