/*
 * tcpsocket.hpp
 *
 *  Created on: Oct 14, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_SOCKET_HPP__
#define __CWT_IO_SOCKET_HPP__

#include <cwt/cwt.hpp>
#include <cwt/pimpl.hpp>
#include <cwt/atomic.hpp>
#include <cwt/io/device.hpp>

CWT_NS_BEGIN

namespace io {

class DLL_API HostInfo
{
public:
	struct addrinfo_iterator_data;
	class addrinfo_iterator
	{
		friend class HostInfo;
		shared_ptr<addrinfo_iterator_data> m_data;

	public:
		addrinfo_iterator ();
		~addrinfo_iterator();
		addrinfo_iterator (const addrinfo_iterator & it) : m_data(it.m_data) {}
		addrinfo_iterator & operator = (const addrinfo_iterator & it)
		{
			m_data = it.m_data;
			return *this;
		}

		bool operator == (const addrinfo_iterator & it);
		bool operator != (const addrinfo_iterator & it) { return !(*this == it); }
		addrinfo_iterator & operator ++ ();
		addrinfo_iterator   operator ++ (int)
		{
			addrinfo_iterator r(*this);
            this->operator ++();
            return r;
        }

	};

/*
	class nameinfo_iterator
	{
		void * data;
	};
*/

	addrinfo_iterator beginAddrInfo(const String & hostname);
	addrinfo_iterator endAddrInfo();

};

class DLL_API HostAddress
{
public:
	static const uint32_t InvalidIpv4 = uint32_t(0);

public:
	HostAddress () : m_hostname(), m_ipv4(0), m_ipv6(nullptr) { }
	HostAddress (const String & hostname) { setAddress(hostname); }

	//HostAddressType type () const { return m_type; }
	void setPath (const String & path);
	void setAddress (const String & hostname);
	void setAddress (uint32_t ipv4);

	const String & hostname () const { return m_hostname; }
	uint32_t ipv4 () const { return m_ipv4; }

	String toString () const;

	bool resolve ();

private:
	//HostAddressType m_type;
	String    m_hostname;
	uint32_t  m_ipv4;
	uint8_t * m_ipv6; // not supported yet
};

class DLL_API Socket : public Device
{
	CWT_PIMPL_IMPL(Socket);
public:
	enum SocketType {
		  LocalDgramSocket  = 0x01000
		, LocalSocket = 0x02000 // stream socket
		, UdpSocket   = 0x03000
		, TcpSocket   = 0x04000
		, SocketMask  = TcpSocket

		, Listener    = 0x40000
	};

	static const SocketType DefaultSocketType = TcpSocket;

protected:
	virtual ssize_t readBytes(char bytes[], size_t n);
	virtual ssize_t writeBytes(const char bytes[], size_t n);
	virtual size_t  bytesAvailable() const;

public:
	Socket();
	virtual ~Socket() {}
	bool open(const String & path, int32_t oflags = ReadWrite | DefaultSocketType);
	bool open(const String & hostname, uint16_t port, int32_t oflags = ReadWrite | DefaultSocketType);
	bool open(const HostAddress & hostaddr, uint16_t port, int32_t oflags = ReadWrite | DefaultSocketType);
	virtual bool close();
	virtual void flush();
	virtual bool opened() const;
};

} // namespace io

CWT_NS_END

#endif /* __CWT_IO_SOCKET_HPP__ */
