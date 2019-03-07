/*
 * libe cross-platform library: Logging
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_LOG_H_
#define _LIBE_LOG_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#ifdef TARGET_LINUX
#include <string.h>
#endif

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

/* PSTR() puts string into program memory in AVR, define replacement for other platforms */
#ifndef PSTR
#define PSTR(x)                     x
#endif

#define LOG_MSG(level, file, line, func, msg, ...)      log_msg(level, file, line, func, PSTR(msg), ##__VA_ARGS__)


#ifdef USE_LOG /* logging functions */
/** Initialize logging. */
int log_init(void *context, uint32_t baud);
/**
 * Write message to log.
 */
void log_msg(int level, const char *file, int line, const char *func, const char *msg, ...);
/** Quit logging */
void log_quit(void);
#else /* if logging is disabled, define empty ones */
#define log_init(context, baud)
#define log_msg(level, file, line, func, msg, ...)
#define log_quit()
#endif /* USE_LOG */


/* colors in terminal */
#if defined(USE_LOG_COLORS) && defined(USE_LOG)
#define LDC_DEFAULT "\033[0m"
#define LDC_DGRAYB "\033[1;30m"
#define LDC_REDB "\033[1;31m"
#define LDC_GREENB "\033[1;32m"
#define LDC_YELLOWB "\033[1;33m"
#define LDC_BLUEB "\033[1;34m"
#define LDC_PURPLEB "\033[1;35m"
#define LDC_CYANB "\033[1;36m"
#define LDC_WHITEB "\033[1;37m"
#define LDC_DGRAY "\033[30m"
#define LDC_RED "\033[31m"
#define LDC_GREEN "\033[32m"
#define LDC_YELLOW "\033[33m"
#define LDC_BLUE "\033[34m"
#define LDC_PURPLE "\033[35m"
#define LDC_CYAN "\033[36m"
#define LDC_WHITE "\033[37m"
#define LDC_BDGRAY "\033[40m"
#define LDC_BRED "\033[41m"
#define LDC_BGREEN "\033[42m"
#define LDC_BYELLOW "\033[43m"
#define LDC_BBLUE "\033[44m"
#define LDC_BPURPLE "\033[45m"
#define LDC_BCYAN "\033[46m"
#define LDC_BWHITE "\033[47m"
#define LOG_COLOR(color) LOG_PRINTF(color)
#else
#define LDC_DEFAULT
#define LDC_DGRAYB
#define LDC_REDB
#define LDC_GREENB
#define LDC_YELLOWB
#define LDC_BLUEB
#define LDC_PURPLEB
#define LDC_CYANB
#define LDC_WHITEB
#define LDC_DGRAY
#define LDC_RED
#define LDC_GREEN
#define LDC_YELLOW
#define LDC_BLUE
#define LDC_PURPLE
#define LDC_CYAN
#define LDC_WHITE
#define LDC_BDGRAY
#define LDC_BRED
#define LDC_BGREEN
#define LDC_BYELLOW
#define LDC_BBLUE
#define LDC_BPURPLE
#define LDC_BCYAN
#define LDC_BWHITE
#define LOG_COLOR(color)
#endif /* defined(USE_LOG_COLORS) && defined(USE_LOG) */


/** If condition is true, simply return */
#define IF_R(condition, ret) do { if (condition) { return ret; } } while(0)

/* allocate memory for struct */
#define SALLOC(p_item, err_ret) \
do { \
    p_item = malloc(sizeof(*p_item)); \
    IF_R(p_item == NULL, err_ret); \
    memset(p_item, 0, sizeof(*p_item)); \
} while (0)

/* just a helper macro */
#define DEBUG_LOG_MSG(level, ...) LOG_MSG(level, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

/** Debug level log message. */
#define DEBUG_MSG(...) DEBUG_LOG_MSG(LOG_LEVEL_DEBUG, __VA_ARGS__);
/** Info level log message. */
#define INFO_MSG(...) DEBUG_LOG_MSG(LOG_LEVEL_INFO, __VA_ARGS__);
/** Notice level log message. */
#define NOTICE_MSG(...) DEBUG_LOG_MSG(LOG_LEVEL_NOTICE, __VA_ARGS__);
/** Warning level log message. */
#define WARN_MSG(...) DEBUG_LOG_MSG(LOG_LEVEL_WARN, __VA_ARGS__);
/** Error level log message. */
#define ERROR_MSG(...) DEBUG_LOG_MSG(LOG_LEVEL_ERR, __VA_ARGS__);
/** Critical level log message. */
#define CRIT_MSG(...) DEBUG_LOG_MSG(LOG_LEVEL_CRIT, __VA_ARGS__);

/** Debug level log message with condition. */
#define DEBUG_IF(condition, ...) do { if (condition) { DEBUG_LOG_MSG(LOG_LEVEL_DEBUG, __VA_ARGS__); } } while(0)
/** Info level log message with condition. */
#define INFO_IF(condition, ...) do { if (condition) { DEBUG_LOG_MSG(LOG_LEVEL_INFO, __VA_ARGS__); } } while(0)
/** Notice level log message with condition. */
#define NOTICE_IF(condition, ...) do { if (condition) { DEBUG_LOG_MSG(LOG_LEVEL_NOTICE, __VA_ARGS__); } } while(0)
/** Warning level log message with condition. */
#define WARN_IF(condition, ...) do { if (condition) { DEBUG_LOG_MSG(LOG_LEVEL_WARN, __VA_ARGS__); } } while(0)
/** Error level log message with condition. */
#define ERROR_IF(condition, ...) do { if (condition) { DEBUG_LOG_MSG(LOG_LEVEL_ERR, __VA_ARGS__); } } while(0)
/** Critical level log message with condition. */
#define CRIT_IF(condition, ...) do { if (condition) { DEBUG_LOG_MSG(LOG_LEVEL_CRIT, __VA_ARGS__); } } while(0)
/** If condition is NULL, log critical level message. Use to check memory allocation failures. */
#define NULL_IF(condition) do { if (condition == NULL) { DEBUG_LOG_MSG(LOG_LEVEL_CRIT, "condition is NULL, memory allocation failed?"); } } while(0)

/** Debug level log message with condition and return. */
#define DEBUG_IF_R(condition, ret, ...) do { if (condition) { DEBUG_LOG_MSG(LOG_LEVEL_DEBUG, __VA_ARGS__); return ret; } } while(0)
/** Info level log message with condition and return. */
#define INFO_IF_R(condition, ret, ...) do { if (condition) { DEBUG_LOG_MSG(LOG_LEVEL_INFO, __VA_ARGS__); return ret; } } while(0)
/** Notice level log message with condition and return. */
#define NOTICE_IF_R(condition, ret, ...) do { if (condition) { DEBUG_LOG_MSG(LOG_LEVEL_NOTICE, __VA_ARGS__); return ret; } } while(0)
/** Warning level log message with condition and return. */
#define WARN_IF_R(condition, ret, ...) do { if (condition) { DEBUG_LOG_MSG(LOG_LEVEL_WARN, __VA_ARGS__); return ret; } } while(0)
/** Error level log message with condition and return. */
#define ERROR_IF_R(condition, ret, ...) do { if (condition) { DEBUG_LOG_MSG(LOG_LEVEL_ERR, __VA_ARGS__); return ret; } } while(0)
/** Critical level log message with condition and return. */
#define CRIT_IF_R(condition, ret, ...) do { if (condition) { DEBUG_LOG_MSG(LOG_LEVEL_CRIT, __VA_ARGS__); return ret; } } while(0)
/** If condition is NULL, log critical level message. Use to check memory allocation failures. */
#define NULL_IF_R(condition, ret) do { if (condition == NULL) { DEBUG_LOG_MSG(LOG_LEVEL_CRIT, "condition is NULL, memory allocation failed?"); return ret; } } while(0)


/* dumping memory macros that are defined only in specific situations */
#if defined(DEBUG) || defined(TARGET_LINUX) || defined(TARGET_ESP32)
/** hex dumping. */
#define HEX_DUMP(data, size) \
    do { \
        int __i; \
        for (__i = 0; __i < size; __i++) { \
            if (__i > 0) { \
                LOG_PRINTF(" "); \
            } \
            LOG_PRINTF("%02x", ((uint8_t *)data)[__i]); \
        } \
        LOG_PRINTF("\r\n"); \
    } while (0);
/** ascii dumping. */
#define ASCII_DUMP(data, size, pad) \
    do { \
        int __i, __j; \
        for (__i = 0; __i < size; __i++) { \
            if (__i > 0) { \
                for (__j = 0; __j < pad; __j++) { LOG_PRINTF(" "); } \
            } \
            if (isprint(((uint8_t *)data)[__i])) { LOG_PRINTF("%c", ((uint8_t *)data)[__i]); } \
            else { LOG_PRINTF("?"); } \
        } \
        LOG_PRINTF("\r\n"); \
    } while (0);
#else
#define HEX_DUMP(data, size)
#define ASCII_DUMP(data, size, pad)
#endif /* defined(DEBUG) || defined(TARGET_LINUX) || defined(TARGET_ESP32) */


#ifdef __cplusplus
}
#endif

#endif /* _LIBE_LOG_H_ */
