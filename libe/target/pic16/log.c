/*
 * libe cross-platform library: Logging
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#include <stdarg.h>
#include <libe/os.h>
#include <libe/log.h>
#include <libe/log.h>


int log_init(void)
{
	return 0;
}

void log_quit(void)
{
}

void log_msg(int level, const char *file, int line, const char *func, const char *msg, ...)
{
}
