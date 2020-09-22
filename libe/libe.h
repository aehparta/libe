/*
 * libe cross-platform library: Logging
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifndef _LIBE_LIBE_H_
#define _LIBE_LIBE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "os.h"
#include "opt.h"
#include "log.h"
#include "wifi.h"
#include "nvm.h"
#include "error.h"
/* gpio */
#include "gpio.h"
/* i2c drivers */
#include "i2c.h"
/* spi drivers */
#include "spi.h"
/* internal adc drivers */
#include "adci.h"
/* display */
#include "display.h"
/* simple button helper macros for gpio attached buttons */
#include "button.h"
/* encryption */
#include "crypt/aes.h"
#include "crypt/rc5.h"
#include "crypt/xtea.h" /* NOTE: this implementation has not been properly tested */
#include "crypt/xxtea.h" /* NOTE: this implementation has not been properly tested */
#include "crypt/crc.h"
/* pwm */
#include "pwm.h"

/* other extras */
#ifdef USE_BROADCAST
#include "drivers/misc/broadcast.h"
#endif


#ifdef __cplusplus
}
#endif

#endif /* _LIBE_LIBE_H_ */
