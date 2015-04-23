/*
 * socket_unix.hpp
 *
 *  Created on: Oct 16, 2013
 *      Author: wladt
 */

#ifndef __CWT_SOCKET_UNIX_HPP__
#define __CWT_SOCKET_UNIX_HPP__

#include <pfs/string.hpp>
#include <sys/socket.h>
#include <netinet/in.h>

namespace cwt { namespace io {

class native_socket
{
public:
	native_socket () : sockfd(-1) {}
	bool open (int socktype, int proto, int32_t oflags);
	bool opened () const { return sockfd >= 0; }
	size_t bytesAvailable() const;
	bool close();

	int sockfd;
};

class native_inet_socket : public native_socket
{
public:
	native_inet_socket() : native_socket() {}
	bool open (const pfs::string & hostname, uint16_t port, int socktype, int proto, int32_t oflags);
	struct sockaddr_in saddr;
};

}} // cwt::net

#endif /* __CWT_SOCKET_UNIX_HPP__ */
