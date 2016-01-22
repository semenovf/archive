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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "inet_socket_posix.hpp"

namespace pfs { namespace io { namespace details {

bits::device::open_mode_flags inet_socket::open_mode () const
{
	// TODO Inherited from file (check if this apply to real socket)

	bits::device::open_mode_flags r = 0;
	char buf[1] = { 0 };

	if (::read(_fd, buf, 0) >= 0 && errno != EBADF)
		r |= bits::read_only;

	if (::write(_fd, buf, 0) >= 0 && errno != EBADF)
		r |= bits::write_only;

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

bool inet_socket_base::s_set_nonblocking (native_handle_type & fd, bool on)
{
    int flags = ::fcntl(fd, F_GETFL, 0);

    if (on)
    	flags |= O_NONBLOCK;
    else
    	flags &= ~O_NONBLOCK;

    return ::fcntl(fd, F_SETFL, flags) >= 0;
}

// static
error_code inet_socket_base::s_close (native_handle_type & fd)
{
    error_code ex;

    if (fd > 0) {
        if (::close(fd) < 0) {
        	ex = error_code(errno);
        }
    }

    fd = -1;
    return ex;
}

// static
std::pair<error_code, inet_socket_base::native_handle_type> inet_socket_base::s_create (bool non_blocking)
{
	typedef std::pair<error_code, inet_socket_base::native_handle_type> result_type;
	int socktype = SOCK_STREAM;
	int domain   = PF_INET;
	int proto    = IPPROTO_TCP;

	if (non_blocking)
		socktype |= SOCK_NONBLOCK;

	inet_socket_base::native_handle_type	fd = ::socket(AF_INET, socktype, proto);

	if (fd < 0) return result_type(error_code(errno), fd);

	return result_type(error_code(), fd);
}

// static
error_code inet_socket_base::s_connect (inet_socket_base & sock
		, uint32_t addr
		, uint16_t port)
{
	memset(& sock._sockaddr, 0, sizeof(sock._sockaddr));

	sock._sockaddr.sin_family      = PF_INET;
	sock._sockaddr.sin_port        = htons(port);
	sock._sockaddr.sin_addr.s_addr = htonl(addr);

	int rc = ::connect(sock._fd
			, reinterpret_cast<struct sockaddr *>(& sock._sockaddr)
			, sizeof(sock._sockaddr));

	error_code ex;

    if (rc < 0) {
        switch (errno) {
        case EISCONN:
        	sock._state = bits::connected_state;
            break;

        case ECONNREFUSED:
        case EINVAL:
            ex = error_code(errno);
            sock._state = bits::unconnected_state;
            break;

        case ETIMEDOUT:
        	ex = error_code(errno);
            break;

        case EHOSTUNREACH:
        	ex = error_code(errno);
        	sock._state = bits::unconnected_state;
            break;

        case ENETUNREACH:
        	ex = error_code(errno);
        	sock._state = bits::unconnected_state;
            break;

        case EADDRINUSE:
        	ex = error_code(errno);
            break;

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
        case EINPROGRESS:
        case EALREADY:
        	ex = error_code(errno);
        	sock._state = bits::connecting_state;
            break;

        case EAGAIN:
        	ex = error_code(errno);
            break;

        case EACCES:
        case EPERM:
            ex = error_code(errno);
            sock._state = bits::unconnected_state;
            break;

        case EAFNOSUPPORT:
        case EBADF:
        case EFAULT:
        case ENOTSOCK:
        	sock._state = bits::unconnected_state;
        	break;

        default:
            break;
        }

        if (sock._state != bits::connected_state) {
            return ex;
        }
    }

    sock._state = bits::connected_state;
    return error_code();
}

// static
error_code inet_socket_base::s_bind (inet_socket_base & sock
		, uint32_t addr
		, uint16_t port)
{
	memset(& sock._sockaddr, 0, sizeof(sock._sockaddr));

	sock._sockaddr.sin_family      = PF_INET;
	sock._sockaddr.sin_port        = htons(port);
	sock._sockaddr.sin_addr.s_addr = htonl(addr);

	int yes = 1;

	/* http://publib.boulder.ibm.com/infocenter/iseries/v5r3/topic/rzab6/rzab6xconoserver.htm
	 *
	 * The setsockopt() function is used to allow the local address to
	 * be reused when the server is restarted before the required wait
	 * time expires
	 */
	int rc = ::setsockopt(sock._fd, SOL_SOCKET, SO_REUSEADDR, & yes, sizeof(int));

	if (rc != 0) return error_code(errno);

	rc = ::bind(sock._fd
			, reinterpret_cast<struct sockaddr *>(& sock._sockaddr)
			, sizeof(sock._sockaddr));

	if (rc != 0) return error_code(errno);

	sock._state = bits::bound_state;

	return error_code();
}

// static
error_code inet_socket_base::s_listen (inet_socket_base & sock, int backlog)
{
	int rc = ::listen(sock._fd, backlog);
	if (rc != 0) return error_code(errno);
	return error_code();
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
error_code open_device<tcp_socket> (device & dev, const open_params<tcp_socket> & op)
{
    if (dev.opened())
        return error_code();

    bool non_blocking = op.oflags & bits::non_blocking;

    std::pair<error_code, details::tcp_socket::native_handle_type> rc = details::tcp_socket::s_create(non_blocking);

	if (rc.first)
		return rc.first;

	details::tcp_socket * sock = new details::tcp_socket(rc.second, bits::unconnected_state);

	PFS_ASSERT_NULLPTR(sock);

	error_code ex = details::tcp_socket::s_connect(*sock, op.addr.native(), op.port);

    shared_ptr<bits::device> d(sock);
    dev._d.swap(d);

	return ex;
}

}} // pfs::io
