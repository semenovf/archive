/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   inet_server_posix.hpp
 * Author: wladt
 *
 * Created on November 15, 2016, 6:14 PM
 */

#ifndef __PFS_INET_SERVER_POSIX_HPP__
#define __PFS_INET_SERVER_POSIX_HPP__

#include "pfs/io/inet_server.hpp"
#include "posix_utils.hpp"

namespace pfs {
namespace io {
namespace details {

class inet_server : public bits::server
{
public:
    typedef bits::server::native_handle_type native_handle_type;

protected:
	native_handle_type _fd;
	sockaddr_in  _sockaddr;
    
public:
    inet_server ()
        : bits::server()
        , _fd(-1)
    {}
    
  	virtual ~inet_server ()
	{
		close();
	}

    virtual error_code close ()
    {
        error_code ex = pfs::io::close_socket(_fd) != 0 
                ? error_code(errno) 
                : error_code();
        _fd = -1;
        return ex;
    }

    virtual bool opened () const
    {
    	return _fd >= 0;
    }
    
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
    
public:
    error_code bind (uint32_t addr, uint16_t port);    
};

class tcp_server : public inet_server
{
public:
	typedef inet_server::native_handle_type native_handle_type;

public:
    error_code open (bool non_blocking)
    {
        _fd = pfs::io::create_tcp_socket(non_blocking);
        return _fd < 0 ? error_code(errno) : error_code();
    }

	error_code listen (int backlog)
	{
		return (::listen(_fd, backlog) != 0) 
                ? error_code(errno) 
                : error_code();
	}

public:
	tcp_server ()
		: inet_server()
	{}

    virtual server_type type () const
    {
    	return server_tcp;
    }
    
    virtual string url () const
    {
        return pfs::io::inet_socket_url("tcp", _sockaddr);
    }
  
    virtual error_code accept (bits::device ** peer, bool non_blocking);
};

class udp_server : public inet_server
{
public:
	typedef inet_server::native_handle_type native_handle_type;

public:
    error_code open (bool non_blocking)
    {
        _fd = pfs::io::create_udp_socket(non_blocking);
        return _fd < 0 ? error_code(errno) : error_code();
    }

    error_code listen (int backlog)
	{
        PFS_UNUSED(backlog);
		return error_code();
	}

public:
	udp_server ()
		: inet_server()
	{}

    virtual server_type type () const
    {
    	return server_udp;
    }
    
    virtual string url () const
    {
        return pfs::io::inet_socket_url("udp", _sockaddr);
    }

    error_code accept (bits::device ** peer, bool non_blocking);
};


}}}

#endif /* __PFS_INET_SERVER_POSIX_HPP__ */

