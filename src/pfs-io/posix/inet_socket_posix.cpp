/*
 * tcp_socket_posix.cpp
 *
 *  Created on: Jan 11, 2016
 *      Author: wladt
 *
 * Sources:
 * ---
 * Multicast over TCP/IP HOWTO
 *      http://tldp.org/HOWTO/Multicast-HOWTO.html
 * Socket programming:
 *      http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6soxoverview.htm
 * Socket application design recommendations:
 *      http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6designrec.htm
 * A connection-oriented server example:
 *      http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconoserver.htm
 * A connectionless server example:
 *      http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconlessserver.htm
 * A connection-oriented client example:
 *      http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconoclient.htm
 * A connectionless client example:
 *      http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconlessclient.htm
 * Introduction to non-blocking I/O:
 *      http://www.kegel.com/dkftpbench/nonblocking.html
 */

#include <cerrno>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "inet_socket_posix.hpp"

namespace pfs { namespace io { namespace details {

bits::device::open_mode_flags inet_socket::open_mode () const
{
	// TODO Inherited from file (check if this apply to real socket)

	bits::device::open_mode_flags r = 0;
	char buf[1] = { 0 };

	if (::read(_fd, buf, 0) >= 0 && errno != EBADF)
		r |= io::device::read_only;

	if (::write(_fd, buf, 0) >= 0 && errno != EBADF)
		r |= io::device::write_only;

	return r;
}

size_t inet_socket::bytes_available () const
{
	PFS_ASSERT(_fd >= 0 );
	int n = 0;
	int rc = 0;
	rc = ioctl(_fd, FIONREAD, & n);

	if (rc != 0)
		PFS_THROW_SYSERR(errno);

	PFS_ASSERT(n >= 0);
	return static_cast<size_t>(n);
}

bool inet_socket::s_set_nonblocking (native_handle_type & fd, bool on)
{
    int flags = ::fcntl(fd, F_GETFL, 0);

    if (on)
    	flags |= O_NONBLOCK;
    else
    	flags &= ~O_NONBLOCK;

    return ::fcntl(fd, F_SETFL, flags) >= 0;
}

// static
bool inet_socket::s_close (native_handle_type & fd, error_code * pex)
{
    bool r = true;

    if (fd > 0) {
        if (::close(fd) < 0) {
        	if (pex)
        		*pex = errno;
        	r = false;
        }
    }

    fd = -1;
    return r;
}

// static
inet_socket::native_handle_type inet_socket::s_create (bool non_blocking, error_code * pex)
{
	int socktype = SOCK_STREAM;
	int domain   = PF_INET;
	int proto    = IPPROTO_TCP;

	if (non_blocking)
		socktype |= SOCK_NONBLOCK;

	native_handle_type fd = ::socket(AF_INET, socktype, proto);

	if (fd < 0) {
		if (pex)
			*pex = errno;
	}

	return fd;
}

// static
bool inet_socket::s_connect (tcp_socket::native_handle_type & fd
		, sockaddr_in & server_addr
		, uint32_t addr
		, uint16_t port
		, error_code * pex)
{
	memset(& server_addr, 0, sizeof(server_addr));

	server_addr.sin_family      = PF_INET;
	server_addr.sin_port        = htons(port);
	server_addr.sin_addr.s_addr = htonl(addr);

	int rc = ::connect(fd
			, reinterpret_cast<struct sockaddr *>(& server_addr)
			, sizeof(server_addr));

	if (rc == 0) {
		int yes = 1;

		/* http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconoserver.htm
		 *
		 * The setsockopt() function is used to allow the local address to
		 * be reused when the server is restarted before the required wait
		 * time expires
		 */
		rc = ::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, & yes, sizeof(int));

	} else {
		/* TODO
		    errno == EINPROGRESS:
			The socket is nonblocking and the connection cannot be completed immediately.
			It is possible to select(2) or poll(2) for completion by selecting
			the socket for writing. After select(2) indicates writability,
			use getsockopt(2) to read the SO_ERROR option at level SOL_SOCKET
			to determine whether connect() completed successfully (SO_ERROR is zero) or
			unsuccessfully (SO_ERROR is one of the usual error codes listed here,
			explaining the reason for the failure).
		 */
		if (errno == EINPROGRESS) {
/*
#if PFS_HAVE_POLL
			;
#endif
*/
		}
	}

	if (rc != 0) {
		if (pex)
			*pex = errno;
		return false;
	}

	return true;
}

// static
bool inet_socket::s_bind (native_handle_type & fd
		, sockaddr_in & bind_addr
		, uint32_t addr
		, uint16_t port
		, error_code * pex)
{
	memset(& bind_addr, 0, sizeof(bind_addr));

	bind_addr.sin_family      = PF_INET;
	bind_addr.sin_port        = htons(port);
	bind_addr.sin_addr.s_addr = htonl(addr);

	int rc = ::bind(fd
			, reinterpret_cast<struct sockaddr *>(& bind_addr)
			, sizeof(bind_addr));

	if (rc != 0) {
		if (pex)
			*pex = errno;
		return false;
	}

	return true;
}

// static
bool inet_socket::s_listen (native_handle_type & fd, int backlog, error_code * pex)
{
	int rc = ::listen(fd, backlog);

	if (rc != 0) {
		if (pex)
			*pex = errno;

		return false;
	}

	return true;
}

ssize_t tcp_socket::read (byte_t * bytes, size_t n, error_code * pex)
{
	PFS_ASSERT(_fd >= 0 );

	ssize_t r = 0;

//	do {
		r = recv(_fd, bytes, n, 0);

		if (r < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
			r = 0;
//			continue;
		}

		if (r < 0) {
			if (pex)
				*pex = errno;
		}

//		break;
//	} while (true);

	return r;
}

ssize_t tcp_socket::write (const byte_t * bytes, size_t nbytes, error_code * ex)
{
	PFS_ASSERT(_fd >= 0);

	int r = 0; // total sent

	while (r < nbytes) {
		ssize_t n = send(_fd, bytes + r, nbytes, 0);

		if (n < 0) {
			r = -1;
			break;
		}

		r += n;
		nbytes -= n;
	}

	if (r < 0) {
		if (ex)
			*ex = errno;
	}

	return r;
}

}}} // pfs::io::details

namespace pfs { namespace io {

template <>
error_code open_device<tcp_socket> (device & d, const open_params<tcp_socket> & op)
{
    if (d.opened())
        return error_code();

    error_code ex;

    bool non_blocking = op.oflags & device::non_blocking;

    details::tcp_socket::native_handle_type fd = details::tcp_socket::s_create(non_blocking, & ex);

	if (fd < 0)
		return error_code(EBADF);

	sockaddr_in server_addr;
	bool rc = details::tcp_socket::s_connect(fd, server_addr, op.addr.native(), op.port, & ex);

	if (!rc) {
		::close(fd);
		return ex;
	}

	device dd(new details::tcp_socket(fd, server_addr));
	d.swap(dd);

	return error_code(0);
}

}} // pfs::io
