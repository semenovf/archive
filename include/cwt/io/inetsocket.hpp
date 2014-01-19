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

namespace cwt { namespace net {
class TcpServer;
}

namespace cwt { namespace io {

class DLL_API InetSocket : public Device
{
	CWT_PIMPL_IMPL(InetSocket);

protected:
	virtual size_t  bytesAvailable() const;
	virtual bool    deviceIsOpened () const;
	virtual void    flushDevice    () {}

public:
	InetSocket();
	uint32_t ipv4 () const;
	String ipv4name () const;
	String hostname () const;
	uint16_t port () const;
};

class TcpSocket : public InetSocket
{
	friend class cwt::net::TcpServer;

protected:
	virtual ssize_t readBytes(char bytes[], size_t n);
	virtual ssize_t writeBytes(const char bytes[], size_t n);
	virtual bool    closeDevice ();

public:
	TcpSocket() : InetSocket() {}
	virtual ~TcpSocket() { close(); }
	bool open (const String & hostname, uint16_t port, int32_t oflags = ReadWrite);
	bool connect (const String & hostname, uint16_t port, int32_t oflags = ReadWrite)
	{
		return open(hostname, port, oflags);
	}
};

}} // cwt::io

#endif /* __CWT_IO_SOCKET_HPP__ */
