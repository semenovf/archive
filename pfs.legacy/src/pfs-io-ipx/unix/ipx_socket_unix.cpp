/*
 * ipx_socket_unix.cpp
 *
 *  Created on: Jun 22, 2015
 *      Author: wladt
 */

#include "ipx_p.hpp"
#include <sys/types.h>
#include <sys/socket.h>

namespace pfs { namespace io {

bool ipx_socket::open (const ipx_addr & addr, int32_t oflags)
{
    if (opened()) {
		this->addError(_u8("IPX socket: already opened"));
		return false;
    }

	ipx_socket_impl * simpl = new ipx_socket_impl;

	if (!simpl->open(addr, oflags, *this)) {
		delete simpl;
		return false;
	}

	pimpl d(simpl);
	_d.swap(d);

	return true;
}

bool ipx_socket::connect (const ipx_addr & addr)
{
	if (!opened())
		return false;

	sockaddr_ipx saddr;
	buildNativeAddr(addr, & saddr);

	//
	// man 2 connect:
	//----------------------------------------------------------------
	// If  the  socket  sockfd  is  of  type  SOCK_DGRAM then addr
	// is the address to which datagrams are sent by
	// default, and the only address from which datagrams are received
	//
	if (::connect(_d.cast<ipx_socket_impl>()->_sockfd
			, reinterpret_cast<sockaddr *>(& saddr)
			, sizeof(saddr)) < 0) {
		this->addSystemError(errno, _u8("failed to connect IPX socket"));
		return false;
	}

	return true;
}

}} // pfs::io
