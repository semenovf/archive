/*
 * tcP_server_posix.cpp
 *
 *  Created on: Jan 13, 2016
 *      Author: wladt
 */

#include <cerrno>
//#include <fcntl.h>
//#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "pfs/io/inet_server.hpp"
#include "inet_socket_posix.hpp"

namespace pfs { namespace io { namespace details {

struct tcp_server : public bits::server
{
	typedef tcp_socket::native_handle_type native_handle_type;

	native_handle_type _fd;
	struct sockaddr_in _sockaddr;

	tcp_server () : _fd(-1) {}

	tcp_server (native_handle_type fd, struct sockaddr_in & sockaddr)
	: _fd(fd)
	{
		memcpy(& _sockaddr, & sockaddr, sizeof(sockaddr));
	}

	virtual ~tcp_server ()
	{
		if (_fd > 0) {
			inet_socket::s_close(_fd, 0);
		}
	}

    virtual bool opened () const
    {
    	return _fd >= 0;
    }

    virtual bool close (error_code * ex)
    {
    	return inet_socket::s_close(_fd, ex);
    }

    virtual bool set_nonblocking (bool on)
    {
    	return inet_socket::s_set_nonblocking(_fd, on);
    }

    virtual bool accept (bits::device **, bool non_blocking, error_code * ex);
};


bool tcp_server::accept (bits::device ** peer, bool non_blocking, error_code * pex)
{
	struct sockaddr_in peer_addr;
	socklen_t peer_len = sizeof(peer_addr);

	int peer_sock = ::accept(_fd
			, reinterpret_cast<struct sockaddr *>(& peer_addr)
			, & peer_len);

	if (peer_sock < 0) {
    	if (pex)
    		*pex = errno;
    	return false;
	}

	if (non_blocking) {
		details::inet_socket::s_set_nonblocking(peer_sock, true);
	}

	*peer = new details::tcp_socket(peer_sock, peer_addr);

	return true;
}

}}}

namespace pfs { namespace io {

template <>
bool open_server<tcp_server> (server & d, const open_params<tcp_server> & op, error_code * pex)
{
    if (d.opened())
        return false;

    bool non_blocking = op.oflags & device::non_blocking;

    details::inet_socket::native_handle_type fd = details::inet_socket::s_create(non_blocking, pex);

	if (fd < 0)
		return false;

	sockaddr_in bind_addr;
	bool rc = details::inet_socket::s_bind(fd, bind_addr, op.addr.native(), op.port, pex);
	if (rc) rc = details::inet_socket::s_listen(fd, op.npendingconn, pex);

	if (!rc) {
		details::inet_socket::s_close(fd, 0);
		return false;
	}

	d._d = new details::tcp_server(fd, bind_addr);

	return true;
}

}} // pfs::io
