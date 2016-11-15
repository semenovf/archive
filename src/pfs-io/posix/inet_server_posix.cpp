/*
 * tcP_server_posix.cpp
 *
 *  Created on: Jan 13, 2016
 *      Author: wladt
 */

#include <cerrno>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "inet_server_posix.hpp"
#include "inet_socket_posix.hpp"

namespace pfs {
namespace io {
namespace details {

error_code inet_server::bind (uint32_t addr, uint16_t port)
{
	memset(& _sockaddr, 0, sizeof(_sockaddr));

	_sockaddr.sin_family      = PF_INET;
	_sockaddr.sin_port        = htons(port);
	_sockaddr.sin_addr.s_addr = htonl(addr);

	int yes = 1;

	/* http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconoserver.htm
	 *
	 * The setsockopt() function is used to allow the local address to
	 * be reused when the server is restarted before the required wait
	 * time expires
	 */
	int rc = ::setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, & yes, sizeof(int));

	if (rc != 0) return error_code(errno);

	rc = ::bind(_fd
			, reinterpret_cast<struct sockaddr *>(& _sockaddr)
			, sizeof(_sockaddr));

	if (rc != 0)
		return error_code(errno);

	return error_code();
}

error_code tcp_server::accept (bits::device ** peer, bool non_blocking)
{
	struct sockaddr_in peer_addr;
	socklen_t peer_addr_len = sizeof(peer_addr);

	int peer_sock = ::accept(_fd
			, reinterpret_cast<struct sockaddr *>(& peer_addr)
			, & peer_addr_len);

	PFS_ASSERT(sizeof(sockaddr_in) == peer_addr_len);

	if (peer_sock < 0) {
    	return error_code(errno);
	}

	details::tcp_socket_peer * peer_socket = new details::tcp_socket_peer(peer_sock, peer_addr);

	PFS_ASSERT(peer_socket->set_nonblocking(non_blocking));

	*peer = dynamic_cast<bits::device *>(peer_socket);

	return error_code();
}

error_code udp_server::accept (bits::device ** peer, bool non_blocking)
{
	struct sockaddr_in peer_addr;
	socklen_t peer_addr_len = sizeof(peer_addr);
    
    char buf[1];
    ssize_t n = recvfrom (_fd, buf, 0, MSG_PEEK
            , reinterpret_cast<sockaddr *>(& peer_addr), & peer_addr_len);

	if (n < 0) {
    	return error_code(errno);
	}

	details::udp_socket_peer * peer_socket = new details::udp_socket_peer(_fd, peer_addr);

	PFS_ASSERT(peer_socket->set_nonblocking(non_blocking));

	*peer = dynamic_cast<bits::device *>(peer_socket);

	return error_code();
}

}}}

namespace pfs { namespace io {

template <>
server open_server<tcp_server> (const open_params<tcp_server> & op, error_code & ex)
{
    bool non_blocking_flag = op.oflags & pfs::io::non_blocking;
    
    details::tcp_server * d = new details::tcp_server;

    ex = d->open(non_blocking_flag);
    if (!ex) ex = d->bind(op.addr.native(), op.port);
    if (!ex) ex = d->listen(op.npendingconn);

    if (ex) {
    	delete d;
    	return server();
    }

    return server(d);
}

template <>
server open_server<udp_server> (const open_params<udp_server> & op, error_code & ex)
{
    bool non_blocking_flag = op.oflags & pfs::io::non_blocking;

    details::udp_server * d = new details::udp_server;

    ex = d->open(non_blocking_flag);

    if (!ex) ex = d->bind(op.addr.native(), op.port);

    if (ex) {
    	delete d;
    	return server();
    }

    return server(d);
}


}} // pfs::io
