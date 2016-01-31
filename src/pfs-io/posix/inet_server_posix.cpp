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
#include "pfs/io/inet_server.hpp"
#include "inet_socket_posix.hpp"

namespace pfs { namespace io { namespace details {

class tcp_server : public bits::server
{
public:
	typedef bits::server::native_handle_type native_handle_type;

protected:
	native_handle_type _fd;
	sockaddr_in  _sockaddr;

public:
	error_code open (bool non_blocking);

	error_code bind (uint32_t addr, uint16_t port);

	error_code listen (int backlog)
	{
		return (::listen(_fd, backlog) != 0) ? error_code(errno) : error_code();
	}

public:

	tcp_server ()
		: bits::server()
		, _fd(-1)
	{}

	virtual ~tcp_server ()
	{
		close();
	}

    virtual bool opened () const
    {
    	return _fd >= 0;
    }

    virtual error_code close ();

    virtual bool set_nonblocking (bool on);

    error_code accept (bits::device ** peer, bool non_blocking);

    virtual native_handle_type native_handle () const
    {
    	return _fd;
    }
};

error_code tcp_server::open (bool non_blocking)
{
	int socktype = SOCK_STREAM;

	if (non_blocking)
		socktype |= SOCK_NONBLOCK;

	_fd = ::socket(PF_INET, socktype, IPPROTO_TCP);

	return _fd < 0 ? error_code(errno) : error_code();
}


error_code tcp_server::bind (uint32_t addr, uint16_t port)
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

error_code tcp_server::close ()
{
	error_code ex;

    if (_fd > 0) {
        if (::close(_fd) < 0) {
        	ex = error_code(errno);
        }
    }

    _fd = -1;
    return ex;
}

bool tcp_server::set_nonblocking (bool on)
{
    int flags = ::fcntl(_fd, F_GETFL, 0);

    if (on)
    	flags |= O_NONBLOCK;
    else
    	flags &= ~O_NONBLOCK;

    return ::fcntl(_fd, F_SETFL, flags) >= 0;
}

error_code tcp_server::accept (bits::device ** peer, bool non_blocking)
{
	struct sockaddr_in peer_addr;
	socklen_t peer_len = sizeof(peer_addr);

	int peer_sock = ::accept(_fd
			, reinterpret_cast<struct sockaddr *>(& peer_addr)
			, & peer_len);

	PFS_ASSERT(sizeof(sockaddr_in) == peer_len);

	if (peer_sock < 0) {
    	return error_code(errno);
	}

	details::tcp_socket * peer_socket = new details::tcp_socket(peer_sock, peer_addr);

	PFS_ASSERT(peer_socket->set_nonblocking(non_blocking));

	*peer = dynamic_cast<bits::device *>(peer_socket);

	return error_code();
}

}}}

namespace pfs { namespace io {

template <>
server open_server<tcp_server> (const open_params<tcp_server> & op, error_code & ex)
{
    bool non_blocking = op.oflags & bits::non_blocking;

    details::tcp_server * d = new details::tcp_server;

    ex = d->open(non_blocking);
    if (!ex) ex = d->bind(op.addr.native(), op.port);
    if (!ex) ex = d->listen(op.npendingconn);

    if (ex) {
    	delete d;
    	return server();
    }

    return server(d);
}

}} // pfs::io
