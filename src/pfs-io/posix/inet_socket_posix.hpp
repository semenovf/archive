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

struct inet_socket_base
{
	typedef bits::device::native_handle_type native_handle_type;
	typedef bits::device::state_type state_type;

	native_handle_type _fd;
	state_type         _state;
	sockaddr_in        _sockaddr;

	inet_socket_base ()
		: _fd(-1)
		, _state(bits::unconnected_state)
	{}

    static error_code s_close (native_handle_type & fd);

    static bool s_set_nonblocking (native_handle_type & fd, bool on);

	static std::pair<error_code, native_handle_type> s_create (bool non_blocking);

	static error_code s_connect (inet_socket_base & sock
			, uint32_t addr
			, uint16_t port);

	static error_code s_bind (inet_socket_base & sock
			, uint32_t addr
			, uint16_t port);

	static error_code s_listen (inet_socket_base & sock, int backlog);
};

struct inet_socket : public inet_socket_base, public bits::device
{
	typedef inet_socket_base::native_handle_type native_handle_type;
	typedef inet_socket_base::state_type state_type;

	inet_socket () : inet_socket_base() {}

	virtual ~inet_socket ()
	{
		if (_fd > 0) {
			s_close(_fd);
		}
	}

    virtual open_mode_flags open_mode () const;

    virtual size_t bytes_available () const;

    virtual ssize_t read (byte_t * bytes, size_t n, error_code * ex) = 0;

    virtual ssize_t write (const byte_t * bytes, size_t n, error_code * ex) = 0;

    virtual error_code close ()
    {
    	return s_close(_fd);
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

    virtual state_type state () const
    {
    	return _state;
    }
};

struct tcp_socket : public inet_socket
{
	typedef inet_socket_base::native_handle_type native_handle_type;
	typedef inet_socket_base::state_type state_type;

	tcp_socket () : inet_socket()
	{}

	tcp_socket (native_handle_type fd, state_type state)
		: inet_socket()
	{
		_fd = fd;
		_state = state;
	}


	tcp_socket (native_handle_type fd, const sockaddr_in & sockaddr, state_type state)
		: inet_socket()
	{
		_fd = fd;
		_state = state;
		::memcpy(& _sockaddr, & sockaddr, sizeof(_sockaddr));
	}

	virtual ssize_t read (byte_t * bytes, size_t n, error_code * ex);

	virtual ssize_t write (const byte_t * bytes, size_t n, error_code * ex);
};

}}}


#endif /* __PFS_INET_SOCKET_POSIX_HPP__ */
