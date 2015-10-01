/*
 * ipx_p.hpp
 *
 *  Created on: Jun 22, 2015
 *      Author: wladt
 */

#ifndef __PFS_UNIX_IPX_P_HPP__
#define __PFS_UNIX_IPX_P_HPP__

#include "pfs/io/ipx.hpp"
#include <pfs/array.hpp>
#include <netipx/ipx.h>
#include <unistd.h>

namespace pfs { namespace io {

void buildNativeAddr (const ipx_addr & addr, sockaddr_ipx * result);
uint8_t nativeFrameType (ipx_frame_enum frameType);
ipx_frame_enum frameType (uint8_t nativeFrameType);

struct ipx_socket_impl : public device_impl
{
	static const size_t DefaultCapacity = 512;
	static const size_t DefaultIncrement = 256;

	typedef array<byte_t> buffer_type;

	int           _sockfd;
	sockaddr_ipx  _sockaddr;
	buffer_type   _ibuf;   // input buffer
	size_t        _icount; // number of bytes in the input buffer

	ipx_socket_impl () : _sockfd(-1), _icount(0) {}
	ipx_socket_impl (int fd, const sockaddr_ipx & saddr, const buffer_type buf, size_t count)
		: _sockfd(fd), _icount(count)
	{
		memcpy(& _sockaddr, & saddr, sizeof(saddr));
		_ibuf.resize(count);
		_ibuf.copyFrom(buf, count);
	}

    virtual ssize_t readBytes      (byte_t bytes[], size_t n, errorable_ext & ex);
    virtual ssize_t writeBytes     (const byte_t bytes[], size_t n, errorable_ext & ex);
    virtual size_t  bytesAvailable () const { return _icount; }
    virtual bool    closeDevice    (errorable_ext & ex);
    virtual bool    deviceIsOpened () const { return _sockfd >= 0; }
    virtual void    flushDevice    () {}

    bool open (const ipx_addr & addr, int32_t oflags, errorable_ext & ex);
    ipx_frame_enum frameType () const;

    ssize_t recvfrom (sockaddr_ipx * saddr);
};

struct ipx_socket_peer_impl : public ipx_socket_impl
{
	ipx_socket_peer_impl (int sockfd
			, const sockaddr_ipx & sockaddr
			, const buffer_type buf
			, size_t count)
		: ipx_socket_impl(sockfd, sockaddr, buf, count)
	{}

	ssize_t writeBytes (const byte_t bytes[], size_t n, errorable_ext & ex);
    virtual bool closeDevice (errorable_ext & ) { return true; }
};

}} // pfs::io

#endif /* __PFS_UNIX_IPX_P_HPP__ */
