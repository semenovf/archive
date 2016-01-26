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

class inet_socket : public bits::device
{
public:
	typedef bits::device::native_handle_type native_handle_type;

protected:
	native_handle_type _fd;
	sockaddr_in  _sockaddr;

private:
	inet_socket (const inet_socket & other);
	inet_socket & operator = (const inet_socket & other);

protected:
	error_code open (bool non_blocking);

public:
	inet_socket ()
		: bits::device()
		, _fd(-1)
	{}

	virtual ~inet_socket ()
	{
		close();
	}

    virtual open_mode_flags open_mode () const;

    virtual size_t bytes_available () const;

    virtual ssize_t read (byte_t * bytes, size_t n, error_code * ex) = 0;

    virtual ssize_t write (const byte_t * bytes, size_t n, error_code * ex) = 0;

    virtual error_code close ();

    virtual bool opened () const
    {
    	return _fd >= 0;
    }

    virtual void flush ()
    {}

    virtual bool set_nonblocking (bool on);

    virtual native_handle_type native_handle () const
    {
    	return _fd;
    }
};

class tcp_socket : public inet_socket
{
public:
	typedef inet_socket::native_handle_type native_handle_type;

	//	bool     _non_blocking;

public:
	error_code open (bool non_blocking);
	error_code connect (uint32_t addr, uint16_t port);

public:
	tcp_socket ()
		: inet_socket()
	{}

	// Used for initialization of peer socket
	//
	tcp_socket (native_handle_type fd, const sockaddr_in & sockaddr)
		: inet_socket()
	{
		_fd = fd;
		::memcpy(& _sockaddr, & sockaddr, sizeof(_sockaddr));
	}

	virtual error_code reopen ();

	virtual ssize_t read (byte_t * bytes, size_t n, error_code * ex);

	virtual ssize_t write (const byte_t * bytes, size_t n, error_code * ex);
};

}}}


#endif /* __PFS_INET_SOCKET_POSIX_HPP__ */
