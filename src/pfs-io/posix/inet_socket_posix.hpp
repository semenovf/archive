/*
 * inet_socket_posix.hpp
 *
 *  Created on: Jan 13, 2016
 *      Author: wladt
 */

#ifndef __PFS_INET_SOCKET_POSIX_HPP__
#define __PFS_INET_SOCKET_POSIX_HPP__

#include "pfs/io/inet_socket.hpp"

namespace pfs { namespace io { namespace details {

struct inet_socket : public bits::device
{
	typedef device::native_handle_type native_handle_type;

	native_handle_type _fd;
	sockaddr_in _sockaddr;

	inet_socket () : _fd(-1) {}

	inet_socket (bits::device::native_handle_type fd
			, struct sockaddr_in & sockaddr)
		: _fd(fd)
	{
		memcpy(& _sockaddr, & sockaddr, sizeof(sockaddr));
	}

	virtual ~inet_socket ()
	{
		if (_fd > 0) {
			s_close(_fd, 0);
		}
	}

    virtual open_mode_flags open_mode () const;

    virtual size_t bytes_available () const;

    virtual ssize_t read (byte_t * bytes, size_t n, error_code * ex) = 0;

    virtual ssize_t write (const byte_t * bytes, size_t n, error_code * ex) = 0;

    virtual bool close (error_code * ex)
    {
    	return s_close(_fd, ex);
    }

    virtual bool opened () const
    {
    	return _fd >= 0;
    }

    virtual void flush ()
    {}

    virtual bool set_nonblocking (bool on)
    {
    	return s_set_nonblocking(_fd, on);
    }

    virtual native_handle_type native_handle () const
    {
    	return _fd;
    }

    virtual state_type state () const;

    static bool s_close (native_handle_type & fd, error_code * pex);

    static bool s_set_nonblocking (native_handle_type & fd, bool on);

	static native_handle_type s_create (bool non_blocking, error_code * pex);

	static bool s_connect (native_handle_type & fd
			, sockaddr_in & server_addr
			, uint32_t addr
			, uint16_t port
			, error_code * pex);

	static bool s_bind (native_handle_type & fd
			, sockaddr_in & bind_addr
			, uint32_t addr
			, uint16_t port
			, error_code * pex);

	static bool s_listen (native_handle_type & fd, int backlog, error_code * pex);
};

struct tcp_socket : public inet_socket
{
	typedef inet_socket::native_handle_type native_handle_type;

	tcp_socket ()
		: inet_socket()
	{}

	tcp_socket (native_handle_type fd, sockaddr_in & sockaddr)
		: inet_socket(fd, sockaddr)
	{}

	virtual ssize_t read (byte_t * bytes, size_t n, error_code * ex);

	virtual ssize_t write (const byte_t * bytes, size_t n, error_code * ex);
};

}}}


#endif /* __PFS_INET_SOCKET_POSIX_HPP__ */
