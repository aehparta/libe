/*
 * libe cross-platform library: Logging
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include <libe/os.h>
#include <libe/log.h>
#include <libe/debug.h>


static pthread_mutex_t log_mutex;


int log_init(void *context, uint32_t baud)
{
	pthread_mutex_init(&log_mutex, NULL);
	return 0;
}

void log_quit(void)
{
	pthread_mutex_destroy(&log_mutex);
}

void log_msg(int level, const char *file, int line, const char *func, const char *msg, ...)
{
	pthread_mutex_lock(&log_mutex);

	if (level == LOG_LEVEL_DEBUG) {
		LOG_PRINTF(LDC_PURPLE "D:");
	} else if (level == LOG_LEVEL_INFO) {
		LOG_PRINTF(LDC_BLUE "I:");
	} else if (level == LOG_LEVEL_NOTICE) {
		LOG_PRINTF(LDC_CYAN "N:");
	} else if (level == LOG_LEVEL_WARN) {
		LOG_PRINTF(LDC_YELLOW "W:");
	} else if (level == LOG_LEVEL_ERR) {
		LOG_PRINTF(LDC_RED "E:");
	} else if (level == LOG_LEVEL_CRIT) {
		LOG_PRINTF(LDC_REDB "C:");
	}
	LOG_PRINTF("%s:%s:%d: " LDC_DEFAULT, file, func, line);

	va_list args;
	va_start(args, msg);
	char *vabuf = NULL;
	if (vasprintf(&vabuf, msg, args) > 0) {
		LOG_PRINTF("%s", vabuf);
		free(vabuf);
	}
	va_end(args);
	LOG_PRINTF("\r\n");

	pthread_mutex_unlock(&log_mutex);
}
