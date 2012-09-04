/*
 * socket.c
 *
 *  Created on: 29.08.2012
 *      Author: user
 */



#include <cwt/net/socket.h>
#include "../socket_p.h"

BOOL __socket_allowSockets(void)
{
	if( !__socket_is_sockets_allowed ) {
		__socket_is_sockets_allowed = TRUE;
	}
	return __socket_is_sockets_allowed;
}


/*----------------------------------------------------------------------
 * Source: http://www.kegel.com/dkftpbench/nonblocking.html
 *
 * Portable function to set a socket into nonblocking mode.
 * Calling this on a socket causes all future read() and write() calls on
 * that socket to do only as much as they can immediately, and return
 * without waiting.
 * If no data can be read or written, they return -1 and set errno
 * to EAGAIN (or EWOULDBLOCK).
 * Thanks to Bjorn Reese for this code.
----------------------------------------------------------------------*/
BOOL __socket_setNonBlockingNative (SOCKET sockfd, BOOL is_nonblocking)
{
/*	return ioctl(sockfd, FIONBIO, (int*)&is_nonblocking) < 0 ? FALSE : TRUE;*/
/*
}


BOOL setNonblocking(int fd)
{
*/
    int flags = 0;

    /* If they have O_NONBLOCK, use the Posix way to do it */
#if defined(O_NONBLOCK)
    /* Fixme: O_NONBLOCK is defined but broken on SunOS 4.1.x and AIX 3.2.5. */
    if (-1 == (flags = fcntl(sockfd, F_GETFL, 0))) {
        flags = 0;
    }

	if( is_nonblocking )
		flags |= O_NONBLOCK;
	else
		flags &= ~((int)O_NONBLOCK);

    return fcntl(sockfd, F_SETFL, flags) == 0 ? TRUE : FALSE;
#else
    /* Otherwise, use the old way of doing it */
    flags = is_nonblocking ? 1 : 0;
    return ioctl(sockfd, FIONBIO, &flags) == 0 ? TRUE : FALSE;
#endif
}
