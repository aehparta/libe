/*
 * Error handling.
 */

#include <string.h>
#include <errno.h>

extern char *error_last;

/* if condition met, set last error to given string and return */
#define error_if(condition, ret, msg) do { if (condition) { error_last = msg; return ret; } } while(0)
#define error_if_errno(condition, ret) do { if (condition) { error_last = strerror(errno); return ret; } } while(0)

