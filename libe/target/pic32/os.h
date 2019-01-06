/*
 * 32-bit pic specific os routines as macros.
 */

#include <xc.h>

#define NAN                 -1e13
#define isfinite(x)         (x > -0.9e13)
#define M_E                 2.7182818284590452354
