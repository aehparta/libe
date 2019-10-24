/*
 * libe cross-platform library: Logging
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifdef USE_LOG

#include <stdio.h>
#include <stdarg.h>
#include <libe/libe.h>

#ifdef __STDC_NO_THREADS__
#include <pthread.h>
static pthread_mutex_t log_mutex;
#define MTX_INIT(mtx) pthread_mutex_init(mtx, NULL)
#define MTX_DESTROY(mtx) pthread_mutex_destroy(mtx)
#define MTX_LOCK(mtx) pthread_mutex_lock(mtx)
#define MTX_UNLOCK(mtx) pthread_mutex_unlock(mtx)
#else
#include <threads.h>
static mtx_t log_mutex;
#define MTX_INIT(mtx) mtx_init(mtx, mtx_plain)
#define MTX_DESTROY(mtx) mtx_destroy(mtx)
#define MTX_LOCK(mtx) mtx_lock(mtx)
#define MTX_UNLOCK(mtx) mtx_unlock(mtx)
#endif


int log_init(void)
{
	MTX_INIT(&log_mutex);
	return 0;
}

void log_quit(void)
{
	MTX_DESTROY(&log_mutex);
}

void log_msg(int level, const char *file, int line, const char *func, const char *msg, ...)
{
	MTX_LOCK(&log_mutex);

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

	MTX_UNLOCK(&log_mutex);
}

#endif
