/*
 * tcpserver_unix.cpp
 *
 *  Created on: Oct 16, 2013
 *      Author: wladt
 */

#include "../../include/cwt/net/tcpserver.hpp"
#include "inetsocket_unix.hpp"
#include "socket_unix.hpp"
#include <cwt/logger.hpp>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cerrno>

CWT_NS_BEGIN

namespace net {

class TcpServer::Impl : public NativeInetSocket
{
public:
	Impl() : NativeInetSocket() {}
};


TcpServer::TcpServer () : pimpl(new TcpServer::Impl)
{}

bool TcpServer::opened () const
{
	return pimpl->opened();
}

bool TcpServer::open (const String hostname, uint16_t port, int32_t oflags)
{
	if (pimpl->open(hostname, port, SOCK_STREAM, 0, oflags)) {

		do {
			int rc = 0;

			/* http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconoserver.htm
			 *
			 * The setsockopt() function is used to allow the local address to
			 * be reused when the server is restarted before the required wait
			 * time expires
			 */
			int reuse = 1;
			CWT_VERIFY_ERRNO((rc = ::setsockopt(pimpl->sockfd, SOL_SOCKET, SO_REUSEADDR, (char *) & reuse, sizeof(reuse)) == 0)
				, errno);

			if (!rc)
				break;

			/* Bind the socket */
			CWT_VERIFY_ERRNO((rc = ::bind(pimpl->sockfd, reinterpret_cast<struct sockaddr *>(& pimpl->saddr), sizeof(pimpl->saddr))) == 0, errno);
			if( rc != 0 )
				break;

			/* Listen the socket */
			CWT_VERIFY_ERRNO((rc = ::listen(pimpl->sockfd, 10)) == 0, errno);
			if (rc < 0) {
				break;
			}

			return true;
		} while (false);
	}

	pimpl->close();
	return false;
}

io::TcpSocket * TcpServer::accept ()
{
	io::TcpSocket * peer = nullptr;
	int sockfd = -1;
	struct sockaddr_in saddr;
	socklen_t socklen = sizeof(saddr);

	CWT_VERIFY_ERRNO((sockfd = ::accept(pimpl->sockfd
			, reinterpret_cast<struct sockaddr *>(& saddr)
			, & socklen)) >= 0
		, errno);

	if (sockfd >= 0) {
		peer = new io::TcpSocket;
		peer->pimpl->sockfd = sockfd;
		memcpy(& peer->pimpl->saddr, & saddr, sizeof(saddr));
	}

	return peer;
}

} // namespace net

CWT_NS_END
