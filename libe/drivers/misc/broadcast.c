/*
 * libe cross-platform library: broadcast communication
 *
 * Authors:
 *  Antti Partanen <aehparta@iki.fi>
 */

#ifdef USE_BROADCAST

#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <libe/debug.h>
#include "broadcast.h"
#ifdef TARGET_ESP32
#include <libe/target/esp32/wifi.h>
#endif


static int bsock = -1;
static uint16_t bport = 0;


int broadcast_init(uint16_t port)
{
	int opt;
	struct sockaddr_in addr;

#ifdef TARGET_ESP32
	wifi_init();
#endif

	bport = port;
	if (bport == 0) {
		bport = BROADCAST_DEFAULT_PORT;
	}

	/* create socket */
	bsock = socket(AF_INET, SOCK_DGRAM, 0);
	ERROR_IF_R(bsock < 0, -1, "could not open socket");
	/* enable broadcast */
	opt = 1;
	ERROR_IF_R(setsockopt(bsock, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt)), -1, "could not set socket to broadcast mode");
	/* enable reuse */
	opt = 1;
	ERROR_IF_R(setsockopt(bsock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)), -1, "could not set socket to reuse mode\n");
	/* enable non-blocking socket */
	opt = fcntl(bsock, F_GETFL, 0);
	ERROR_IF_R(fcntl(bsock, F_SETFL, opt | O_NONBLOCK), -1, "could not set socket to non-blocking mode\n");

	/* bind for listening messages */
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(bport);
	ERROR_IF_R(bind(bsock, (struct sockaddr*)&addr, sizeof(addr)), -1, "could not bind for listening broadcast messages");

	return 0;
}


void broadcast_quit(void)
{
	if (bsock > 0) {
		close(bsock);
		bsock = -1;
	}
}


int broadcast_recv(void *data, int size)
{
	struct sockaddr_in addr;
	socklen_t alen = sizeof(addr);
	int n = recvfrom(bsock, data, size, 0, (struct sockaddr *)&addr, &alen);
	ERROR_IF_R(n < 0 && errno != EAGAIN, -1, "receive failed, reason: %s", strerror(errno));
	if (n > 0) {
		// DEBUG_MSG("broadcast packet from: %s", inet_ntoa(addr.sin_addr));
		return n;
	}
	return 0;
}


int broadcast_send(void *data, int size)
{
	struct sockaddr_in addr;

	/* setup send address as broadcast */
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	addr.sin_port = htons(bport);

	/* send data */
	ERROR_IF_R(sendto(bsock, data, size, 0, (struct sockaddr *)&addr, sizeof(addr)) < size, -1, "unable to send data");

	return size;
}


#endif
