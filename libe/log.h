/*
 * libe cross-platform library: Logging
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LOG_H_
#define _LOG_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/** Log level debug. */
#define LOG_LEVEL_DEBUG     0
/** Log level info. */
#define LOG_LEVEL_INFO      1
/** Log level notice. Normal, but significant, condition. */
#define LOG_LEVEL_NOTICE    2
/** Log level warning. */
#define LOG_LEVEL_WARN      3
/** Log level error. */
#define LOG_LEVEL_ERR       4
/** Log level critical: this will reboot the controller. */
#define LOG_LEVEL_CRIT      5
/** Log level off alltogether, no logging is done at all. */
#define LOG_LEVEL_OFF       9


#ifdef TARGET_AVR
#include <avr/pgmspace.h>
#define LOG_PRINTF(format, ...)     printf_P(PSTR(format), ##__VA_ARGS__)
#else
#define LOG_PRINTF                  printf
#endif

/* PSTR() puts string into program memory in AVR */
#ifndef PSTR
#define PSTR(x)                     x
#endif

#define LOG_MSG(level, file, line, func, msg, ...)      log_msg(level, file, line, func, PSTR(msg), ##__VA_ARGS__)

/** Initialize logging. */
int log_init(void *context, uint32_t baud);

/**
 * Write message to log.
 */
void log_msg(int level, const char *file, int line, const char *func, const char *msg, ...);

/** Quit logging */
void log_quit(void);


#ifdef __cplusplus
}
#endif

#endif /* _DEBUG_H_ */