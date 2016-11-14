/*
 * inet_socket_posix.hpp
 *
 *  Created on: Jan 13, 2016
 *      Author: wladt
 */

#ifndef __PFS_INET_SOCKET_POSIX_HPP__
#define __PFS_INET_SOCKET_POSIX_HPP__

#include <cstring>
#include <arpa/inet.h>
#include "pfs/io/inet_socket.hpp"

namespace pfs { namespace io { namespace details {

class inet_socket : public bits::device
{
public:
	typedef bits::device::native_handle_type native_handle_type;

protected:
	native_handle_type _fd;
	sockaddr_in  _sockaddr;
	device_type  _devtype;

private:
	inet_socket (const inet_socket & other);
	inet_socket & operator = (const inet_socket & other);

public:
	virtual error_code open (bool non_blocking) = 0;

public:
	inet_socket ()
		: bits::device()
		, _fd(-1)
		, _devtype(device_unknown)
	{}

	virtual ~inet_socket ()
	{
		close();
	}

    error_code connect (uint32_t addr, uint16_t port);
    
    virtual error_code reopen ();
    
    virtual open_mode_flags open_mode () const;

    virtual size_t bytes_available () const;

    virtual ssize_t read (byte_t * bytes, size_t n, error_code * ex);

    virtual ssize_t write (const byte_t * bytes, size_t n, error_code * ex);

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

    device_type type () const
    {
    	return _devtype;
    }

    error_code set_socket_options (uint32_t sso);
    
    virtual string url () const = 0;
    
    string url (char const * proto) const;
};

class tcp_socket : public inet_socket
{
public:
	typedef inet_socket::native_handle_type native_handle_type;

public:
	virtual error_code open (bool non_blocking);

public:
	tcp_socket ()
		: inet_socket()
	{
		_devtype = device_tcp_socket;
	}

	// Used for initialization of peer socket
	//
	tcp_socket (native_handle_type fd, const sockaddr_in & sockaddr)
		: inet_socket()
	{
		_fd = fd;
		::memcpy(& _sockaddr, & sockaddr, sizeof(_sockaddr));
		_devtype = device_tcp_peer;
	}

    virtual string url () const
    {
        return inet_socket::url("tcp");
    }
};

class udp_socket : public inet_socket
{
public:
	typedef inet_socket::native_handle_type native_handle_type;

public:
	virtual error_code open (bool non_blocking);

public:
	udp_socket ()
		: inet_socket()
	{
		_devtype = device_udp_socket;
	}

    virtual string url () const
    {
        return inet_socket::url("udp");
    }

};

}}}


#endif /* __PFS_INET_SOCKET_POSIX_HPP__ */
