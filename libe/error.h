/*
 * Error handling.
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifdef USE_ERROR

#include <string.h>
#include <errno.h>

extern char *error_last;

/* if condition met, set last error to given string and return */
#define error_if(condition, ret, msg) do { if (condition) { error_last = msg; return ret; } } while(0)
#define error_if_errno(condition, ret) do { if (condition) { error_last = strerror(errno); return ret; } } while(0)
#define error_set_last(msg) do { error_last = msg; } while(0)
#define error_clear() do { error_last = NULL; } while(0)

#else 

#define error_if(condition, ret, msg) do { if (condition) { return ret; } } while(0)
#define error_if_errno(condition, ret) do { if (condition) { return ret; } } while(0)
#define error_set_last(msg)
#define error_clear()

#endif
