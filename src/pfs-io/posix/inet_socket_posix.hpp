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

	native_handle_type _fd;
	sockaddr_in  _sockaddr;

	inet_socket_base ()
		: _fd(-1)
	{}

	inet_socket_base (const inet_socket_base & other)
		: _fd(other._fd)
	{
		memcpy(& _sockaddr, & other._sockaddr, sizeof(_sockaddr));
	}

    static error_code s_close (native_handle_type & fd);

    static bool s_set_nonblocking (native_handle_type & fd, bool on);

	static native_handle_type s_create (bool non_blocking, error_code * pex);

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

	inet_socket () : inet_socket_base() {}

	inet_socket (const inet_socket & other)
		: inet_socket_base(other)
	{}

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
};

struct tcp_socket : public inet_socket
{
	uint32_t _addr;
	uint16_t _port;
	bool     _non_blocking;

	typedef inet_socket_base::native_handle_type native_handle_type;

	tcp_socket ()
		: inet_socket()
		, _addr(net::inet4_addr::invalid_addr_value)
		, _port(-1)
		, _non_blocking(false)
	{}

	tcp_socket (const tcp_socket & other)
		: inet_socket(other)
		, _addr(other._addr)
		, _port(other._port)
		, _non_blocking(other._non_blocking)
	{}


	// Used for initialization of peer socket
	//
	tcp_socket (native_handle_type fd, const sockaddr_in & sockaddr)
		: inet_socket()
		, _addr(net::inet4_addr::invalid_addr_value)
		, _port(-1)
		, _non_blocking(false)
	{
		_fd = fd;
		::memcpy(& _sockaddr, & sockaddr, sizeof(_sockaddr));
	}

	error_code open (uint32_t addr, uint16_t port, bool non_blocking);

	virtual error_code reopen ()
	{
		close();
		return open(_addr, _port, _non_blocking);
	}

	virtual ssize_t read (byte_t * bytes, size_t n, error_code * ex);

	virtual ssize_t write (const byte_t * bytes, size_t n, error_code * ex);
};

}}}


#endif /* __PFS_INET_SOCKET_POSIX_HPP__ */
