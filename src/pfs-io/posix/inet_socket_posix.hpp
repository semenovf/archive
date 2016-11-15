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
#include <unistd.h>
#include "pfs/io/inet_socket.hpp"
#include "posix_utils.hpp"

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

public:
	virtual error_code open (bool non_blocking) = 0;

public:
	inet_socket ()
		: bits::device()
		, _fd(-1)
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

    virtual error_code close ()
    {
        error_code ex = close_socket(_fd) != 0 
                ? error_code(errno) 
                : error_code();
        _fd = -1;
        return ex;
    }

    virtual bool opened () const
    {
    	return _fd >= 0;
    }

    virtual void flush ()
    {}

    virtual bool set_nonblocking (bool on)
    {
        return pfs::io::set_nonblocking(_fd, on);
    }

    virtual bool is_nonblocking () const
    {
        return pfs::io::is_nonblocking(_fd);
    }

    virtual native_handle_type native_handle () const
    {
    	return _fd;
    }

    virtual device_type type () const = 0;
    
    virtual string url () const = 0;
    
    error_code set_socket_options (uint32_t sso);
};

class tcp_socket : public inet_socket
{
public:
	typedef inet_socket::native_handle_type native_handle_type;

public:
	virtual error_code open (bool non_blocking)
    {
        _fd = create_tcp_socket(non_blocking);
        return _fd < 0 ? error_code(errno) : error_code();
    }

public:
	tcp_socket ()
		: inet_socket()
	{}

    virtual device_type type () const
    {
        return device_tcp_socket;
    }
        
    virtual string url () const
    {
        return inet_socket_url("tcp", _sockaddr);
    }
};

class tcp_socket_peer : public tcp_socket
{
public:
	typedef tcp_socket::native_handle_type native_handle_type;

public:
	tcp_socket_peer (native_handle_type fd, const sockaddr_in & sockaddr)
		: tcp_socket()
	{
		_fd = fd;
		::memcpy(& _sockaddr, & sockaddr, sizeof(_sockaddr));
	}
        
    virtual device_type type () const
    {
        return device_tcp_peer;
    }
};

class udp_socket : public inet_socket
{
public:
	typedef inet_socket::native_handle_type native_handle_type;

public:
	virtual error_code open (bool non_blocking)
    {
        _fd = create_udp_socket(non_blocking);
        return _fd < 0 ? error_code(errno) : error_code();
    }

public:
	udp_socket ()
		: inet_socket()
	{}

    virtual device_type type () const
    {
        return device_udp_socket;
    }

    virtual string url () const
    {
        return inet_socket_url("udp", _sockaddr);
    }
};

class udp_socket_peer : public udp_socket
{
public:
	typedef udp_socket::native_handle_type native_handle_type;

public:
	udp_socket_peer (native_handle_type fd, const sockaddr_in & sockaddr)
		: udp_socket()
	{
		_fd = dup(fd);
		::memcpy(& _sockaddr, & sockaddr, sizeof(_sockaddr));
	}
        
    virtual device_type type () const
    {
        return device_udp_peer;
    }
};


}}}


#endif /* __PFS_INET_SOCKET_POSIX_HPP__ */
