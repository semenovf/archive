/*
 * socket_unix.hpp
 *
 *  Created on: Oct 16, 2013
 *      Author: wladt
 */

#ifndef __CWT_SOCKET_UNIX_HPP__
#define __CWT_SOCKET_UNIX_HPP__

#include <cwt/string.hpp>
#include <sys/socket.h>
#include <netinet/in.h>

CWT_NS_BEGIN

class NativeSocket {
public:
	NativeSocket () : sockfd(-1) {}
	bool open (int socktype, int proto, int32_t oflags);
	bool opened () const { return sockfd >= 0; }
	size_t bytesAvailable() const;
	bool close();

	int sockfd;
};

class NativeInetSocket : public NativeSocket
{
public:
	NativeInetSocket() : NativeSocket() {}
	bool open (const String & hostname, uint16_t port, int socktype, int proto, int32_t oflags);
	struct sockaddr_in saddr;
};

CWT_NS_END

#endif /* __CWT_SOCKET_UNIX_HPP__ */
