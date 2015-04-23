/*
 * inetsocket_unix.hpp
 *
 *  Created on: Oct 15, 2013
 *      Author: wladt
 */

#ifndef __CWT_IO_INETSOCKET_UNIX_HPP__
#define __CWT_IO_INETSOCKET_UNIX_HPP__

#include "../../include/cwt/io/inetsocket.hpp"
#include "socket_unix.hpp"

namespace cwt { namespace io {

class inet_socket::impl : public native_inet_socket
{
public:
	impl () : native_inet_socket() {}
};

}} // cwt::io

#endif /* __CWT_IO_INETSOCKET_UNIX_HPP__ */
