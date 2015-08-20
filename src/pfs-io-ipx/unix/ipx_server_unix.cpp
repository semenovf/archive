/*
 * ipx_server_unix.cpp
 *
 *  Created on: Jun 23, 2015
 *      Author: wladt
 */

#include "ipx_p.hpp"

namespace pfs { namespace io {

bool ipx_server::opened () const
{
    if (!_d.isNull() && _d.cast<ipx_socket_impl>()->_sockfd >= 0)
		return true;
    return false;
}

bool ipx_server::open (const ipx_addr & addr, int32_t oflags)
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

bool ipx_server::accept (ipx_socket * sock)
{
	if (!opened())
		return false;

	ipx_socket_impl * d = _d.cast<ipx_socket_impl>();

	sockaddr_ipx sockaddr;
	ssize_t n = d->recvfrom(& sockaddr);

	if (n < 0)
		return false;

	if (sock) {
		pimpl peer(new ipx_socket_peer_impl(d->_sockfd, sockaddr, d->_ibuf, static_cast<size_t>(n)));
		sock->_d.swap(peer);
	}

	return true;
}

}} // pfs::io


