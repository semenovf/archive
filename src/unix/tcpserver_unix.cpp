/*
 * tcpserver_unix.cpp
 *
 *  Created on: Oct 16, 2013
 *      Author: wladt
 */

#include "../../include/cwt/io/tcpserver.hpp"
#include "inetsocket_unix.hpp"
#include "socket_unix.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cerrno>

namespace cwt { namespace io {

class tcp_server::impl : public native_inet_socket
{
public:
	impl() : native_inet_socket() {}
};


tcp_server::tcp_server () : _pimpl(new tcp_server::impl)
{}

bool tcp_server::opened () const
{
	return _pimpl->opened();
}

bool tcp_server::open (const pfs::string hostname, uint16_t port, int32_t oflags)
{
	if (_pimpl->open(hostname, port, SOCK_STREAM, 0, oflags)) {

		do {
			int rc = 0;

			/* http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconoserver.htm
			 *
			 * The setsockopt() function is used to allow the local address to
			 * be reused when the server is restarted before the required wait
			 * time expires
			 */
			int reuse = 1;
			PFS_VERIFY_ERRNO((rc = ::setsockopt(_pimpl->sockfd
					, SOL_SOCKET
					, SO_REUSEADDR
					, (char *) & reuse
					, sizeof(reuse)) == 0)
				, errno);

			if (!rc)
				break;

			/* Bind the socket */
			PFS_VERIFY_ERRNO((rc = ::bind(_pimpl->sockfd
					, reinterpret_cast<struct sockaddr *>(& _pimpl->saddr)
					, sizeof(_pimpl->saddr))) == 0, errno);
			if( rc != 0 )
				break;

			/* Listen the socket */
			PFS_VERIFY_ERRNO((rc = ::listen(_pimpl->sockfd, 10)) == 0, errno);
			if (rc < 0) {
				break;
			}

			return true;
		} while (false);
	}

	_pimpl->close();
	return false;
}

tcp_socket * tcp_server::accept ()
{
	tcp_socket * peer = nullptr;
	int sockfd = -1;
	struct sockaddr_in saddr;
	socklen_t socklen = sizeof(saddr);

	PFS_VERIFY_ERRNO((sockfd = ::accept(_pimpl->sockfd
			, reinterpret_cast<struct sockaddr *>(& saddr)
			, & socklen)) >= 0
		, errno);

	if (sockfd >= 0) {
		peer = new tcp_socket;
		peer->_pimpl->sockfd = sockfd;
		memcpy(& peer->_pimpl->saddr, & saddr, sizeof(saddr));
	}

	return peer;
}

}} // cwt::io
