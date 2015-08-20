/*
 * ipx_unix.cpp
 *
 *  Created on: Jun 8, 2015
 *      Author: wladt
 */


#include "ipx_p.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

namespace pfs { namespace io {

void buildNativeAddr (const ipx_addr & addr, sockaddr_ipx * result)
{
    memset(result, 0, sizeof(sockaddr_ipx));
    result->sipx_family = PF_IPX;
    result->sipx_port = htons(addr.port());
    result->sipx_network = 0; // TODO IPX: do I need to support sipx_network?
    memcpy(result->sipx_node, addr.addrData(), ipx_addr::IpxNodeLen * sizeof(unsigned char));
    result->sipx_type = nativeFrameType(addr.frameType());
}

ipx_frame_t frameType (uint8_t nativeFrameType)
{
	switch(nativeFrameType) {
	case IPX_FRAME_NONE    : return IpxFrameNone;
	case IPX_FRAME_SNAP    : return IpxFrameSnap;
	case IPX_FRAME_8022    : return IpxFrame8022;
	case IPX_FRAME_ETHERII : return IpxFrameEtherII;
	case IPX_FRAME_8023    : return IpxFrame8023;
	case IPX_FRAME_TR_8022 : return IpxFrameTr8022;
	}

	PFS_ASSERT_UNEXPECTED();
	return IpxFrameNone;
}

uint8_t nativeFrameType (ipx_frame_t frameType)
{
	switch(frameType) {
	case IpxFrameNone   : return IPX_FRAME_NONE;
	case IpxFrameSnap   : return IPX_FRAME_SNAP;
	case IpxFrame8022   : return IPX_FRAME_8022;
	case IpxFrameEtherII: return IPX_FRAME_ETHERII;
	case IpxFrame8023   : return IPX_FRAME_8023;
	case IpxFrameTr8022 : return IPX_FRAME_TR_8022;
	}

	PFS_ASSERT_UNEXPECTED();
	return IPX_FRAME_NONE;
}

ipx_frame_t ipx_socket_impl::frameType () const
{
    return pfs::io::frameType(_sockaddr.sipx_type);
}

ssize_t ipx_socket_impl::recvfrom (sockaddr_ipx * saddr)
{
	socklen_t saddrLen = sizeof(*saddr);
	ssize_t n;

	while (true) {
		n = ::recvfrom(_sockfd, _ibuf.data(), _ibuf.capacity(), MSG_PEEK, reinterpret_cast<sockaddr *>(saddr), & saddrLen);

		if (n < 0 && errno == EINTR)
			continue;

		//
		// man 2 recvfrom:
		// ----------------------------------------------------------
		// If no messages are available at the socket,
		// the receive calls wait for a message  to  arrive,
		// unless  the socket  is  nonblocking  (see  fcntl(2)),
		// in which case the value -1 is returned and
		// the external variable errno is set to EAGAIN or EWOULDBLOCK.
		//
		if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
			break;

		if (n >= 0 && static_cast<size_t>(n) < _ibuf.capacity()) {
			ssize_t n1;

			// Actually read data
			do {
				n1 = ::recvfrom(_sockfd, _ibuf.data(), _ibuf.capacity(), 0, reinterpret_cast<sockaddr *>(saddr), & saddrLen);
			} while (n1 < 0 && errno == EINTR);

			PFS_ASSERT(n1 == n);
			PFS_ASSERT(saddrLen == sizeof(*saddr));
			_icount = n;
			break;
		}

		//
		// Other errors
		//
		if (!PFS_VERIFY_ERRNO(n > 0)) {
			break;
		}

		_ibuf.realloc(_ibuf.capacity() + ipx_socket_impl::DefaultIncrement);
	}

	return n;
}

bool ipx_socket_impl::open (const ipx_addr & addr, int32_t oflags, errorable_ext & ex)
{
    if (_sockfd >= 0) {
		ex.addError(_u8("IPX socket: already opened"));
		return false;
    }

    int socktype = SOCK_DGRAM;

	if (oflags & device::NonBlocking)
		socktype |= SOCK_NONBLOCK;

   	_sockfd = ::socket(PF_IPX, socktype, 0);

	if (_sockfd < 0) {
		ex.addSystemError(errno, _u8("failed to open IPX socket"));
		return false;
	}

	buildNativeAddr(addr, & _sockaddr);

	int rc = ::bind(_sockfd, reinterpret_cast<sockaddr *>(& _sockaddr), sizeof(sockaddr_ipx));

	if (rc < 0) {
		ex.addSystemError(errno, _u8("failed to bind IPX socket"));
		::close(_sockfd);
		_sockfd = -1;
		return false;
	}

	if (!_ibuf.capacity())
		_ibuf.realloc(ipx_socket_impl::DefaultCapacity);

	return true;
}

ssize_t ipx_socket_impl::readBytes (byte_t bytes[], size_t n, errorable_ext & /*ex*/)
{
	size_t n1 = pfs::min(n, _icount);

	_ibuf.copyTo(bytes, n1);

	//
	// Left data move to begin of array
	//
	if (_icount > n1) {
		_ibuf.move(0, n1, _icount - n1);
	}

	_icount -= n1;
	return static_cast<ssize_t>(n1);
}

ssize_t ipx_socket_impl::writeBytes (const byte_t bytes[], size_t n, errorable_ext & ex)
{
	ssize_t r = ::sendto(_sockfd, bytes, n, 0, nullptr, 0);

	if (r < 0) {
		ex.addSystemError(errno, _u8("write to IPX socket failure"));
		return -1;
	}
	return r;
}

bool ipx_socket_impl::closeDevice (errorable_ext & ex)
{
	bool r = true;

	if (_sockfd > 0) {
		PFS_VERIFY_ERRNO(0 == shutdown(_sockfd, SHUT_RDWR));

		if (::close(_sockfd) < 0) {
			ex.addSystemError(errno, _u8("IPX socket close failure"));
			r = false;
		}
	}

    _sockfd = -1;
    return r;
}

ssize_t ipx_socket_peer_impl::writeBytes (const byte_t bytes[], size_t n, errorable_ext & ex)
{
	ssize_t r = ::sendto(_sockfd, bytes, n, 0, reinterpret_cast<sockaddr *>(& _sockaddr), sizeof(sockaddr_ipx));

	if (r < 0) {
		ex.addSystemError(errno, _u8("write to IPX socket failure"));
		return -1;
	}
	return r;
}


}} // pfs::io


