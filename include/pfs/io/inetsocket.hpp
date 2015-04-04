/*
 * tcpsocket.hpp
 *
 *  Created on: Oct 14, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_SOCKET_HPP__
#define __CWT_IO_SOCKET_HPP__

#include <pfs/pimpl.hpp>
#include <cwt/io/device.hpp>
#include <cwt/net/hostinfo.hpp>

namespace cwt { namespace io {

class tcp_server;

class DLL_API inet_socket : public device
{
	PFS_PIMPL_DECL(inet_socket, protected, impl, protected);

protected:
	virtual size_t  bytesAvailable () const;
	virtual bool    deviceIsOpened () const;
	virtual void    flushDevice    () {}

public:
	inet_socket ();
	uint32_t ipv4 () const;
	pfs::string ipv4name () const;
	pfs::string hostname () const;
	uint16_t port () const;
};

class tcp_socket : public inet_socket
{
	friend class cwt::io::tcp_server;

protected:
	virtual ssize_t readBytes (char bytes[], size_t n);
	virtual ssize_t writeBytes (const char bytes[], size_t n);
	virtual bool    closeDevice ();

public:
	tcp_socket() : inet_socket() {}
	virtual ~tcp_socket() {}
	bool open (const pfs::string & hostname, uint16_t port, int32_t oflags = ReadWrite);
	bool connect (const pfs::string & hostname, uint16_t port, int32_t oflags = ReadWrite)
	{
		return open(hostname, port, oflags);
	}
};

}} // cwt::io

#endif /* __CWT_IO_SOCKET_HPP__ */
