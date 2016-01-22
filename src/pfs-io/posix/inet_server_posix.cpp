/*
 * tcP_server_posix.cpp
 *
 *  Created on: Jan 13, 2016
 *      Author: wladt
 */

#include <cerrno>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "pfs/io/inet_server.hpp"
#include "inet_socket_posix.hpp"

namespace pfs { namespace io { namespace details {

struct tcp_server : public inet_socket_base, public bits::server
{
	typedef inet_socket_base::native_handle_type native_handle_type;
	typedef inet_socket_base::state_type state_type;

	tcp_server () : inet_socket_base() {}

	tcp_server (native_handle_type fd, state_type state)
		: inet_socket_base()
	{
		_fd = fd;
		_state = state;
	}

	virtual ~tcp_server ()
	{
		if (_fd > 0) {
			inet_socket_base::s_close(_fd);
		}
	}

    virtual bool opened () const
    {
    	return _fd >= 0;
    }

    virtual error_code close ()
    {
    	return inet_socket_base::s_close(_fd);
    }

    virtual bool set_nonblocking (bool on)
    {
    	return inet_socket_base::s_set_nonblocking(_fd, on);
    }

    virtual bool accept (bits::device **, bool non_blocking, error_code * ex);

    virtual native_handle_type native_handle () const
    {
    	return _fd;
    }
};

bool tcp_server::accept (bits::device ** peer, bool non_blocking, error_code * pex)
{
	struct sockaddr_in peer_addr;
	socklen_t peer_len = sizeof(peer_addr);

	int peer_sock = ::accept(_fd
			, reinterpret_cast<struct sockaddr *>(& peer_addr)
			, & peer_len);

	PFS_ASSERT(sizeof(sockaddr_in) == peer_len);

	if (peer_sock < 0) {
    	if (pex)
    		*pex = errno;
    	return false;
	}

	if (non_blocking) {
		details::inet_socket_base::s_set_nonblocking(peer_sock, true);
	}

	*peer = new details::tcp_socket(peer_sock, peer_addr, bits::connected_state);

	return true;
}

}}}

namespace pfs { namespace io {

template <>
error_code open_server<tcp_server> (server & dev, const open_params<tcp_server> & op)
{
    if (dev.opened())
        return error_code(EBADF);

    bool non_blocking = op.oflags & bits::non_blocking;

    std::pair<error_code, details::inet_socket_base::native_handle_type> rc = details::inet_socket_base::s_create(non_blocking);

	if (rc.first)
		return rc.first;

	details::tcp_server * sock = new details::tcp_server(rc.second, bits::unconnected_state);
	PFS_ASSERT_NULLPTR(sock);

	error_code ex = details::inet_socket_base::s_bind(*sock, op.addr.native(), op.port);

	if (!ex)
		ex = details::inet_socket_base::s_listen(*sock, op.npendingconn);

	if (ex) {
		sock->close();
		return ex;
	}

    shared_ptr<bits::server> d(sock);
    dev._d.swap(d);

	return error_code();
}

}} // pfs::io
