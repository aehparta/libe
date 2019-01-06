/*
 * libe cross-platform library: device communication link
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifndef _COMM_H_
#define _COMM_H_

#include <stdbool.h>
#include "spi.h"

#ifdef __cplusplus
extern "C" {
#endif

int comm_init(void);
void comm_quit(void);

int comm_recv(void *data, int size);
int comm_send(void *data, int size);

#ifdef USE_NRF
void comm_disable_nrf(void);
#else
#define comm_disable_nrf()
#endif
bool comm_using_nrf(void);

#ifdef USE_BROADCAST
void comm_disable_broadcast(void);
#else
#define comm_disable_broadcast()
#endif
bool comm_using_broadcast(void);

#ifdef __cplusplus
}
#endif

#endif /* _COMM_H_ */
