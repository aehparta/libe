/*
 * libe cross-platform library: broadcast communication
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifndef _BROADCAST_H_
#define _BROADCAST_H_

#ifdef __cplusplus
extern "C" {
#endif

#define BROADCAST_DEFAULT_PORT      21579 /* same as packet magic! */

int broadcast_init(uint16_t port);
void broadcast_quit(void);

int broadcast_recv(void *data, int size);
int broadcast_send(void *data, int size);

#ifdef __cplusplus
}
#endif

#endif /* _BROADCAST_H_ */
