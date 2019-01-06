/*
 * libe cross-platform library
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "log.h"

#ifdef __cplusplus
extern "C" {
#endif


/* log macros are disabled in AVR when not in debug mode */
#if defined(DEBUG) || !(defined(TARGET_AVR) || defined(TARGET_PIC8) || defined(TARGET_PIC16) || defined(TARGET_PIC32)  || defined(TARGET_MSP430))

/** HEX dumping. */
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
/* production level macro in AVR */
#define HEX_DUMP(data, size)
#define ASCII_DUMP(data, size, pad)
#endif

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


/** If condition is true, simply return */
#define IF_R(condition, ret) do { if (condition) { return ret; } } while(0)

/* allocate memory for struct */
#define SALLOC(p_item, ERROR_ret) \
do { \
    p_item = malloc(sizeof(*p_item)); \
    ERROR_IF_R(p_item == NULL, ERROR_ret, "malloc() failed"); \
    memset(p_item, 0, sizeof(*p_item)); \
} while (0)

/* colors in terminal */
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

#define LDC_SET(color)          LOG_PRINTF(color)


#ifdef __cplusplus
}
#endif

#endif /* _DEBUG_H_ */
