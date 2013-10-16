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

CWT_NS_BEGIN

namespace io {

class InetSocket::Impl : public NativeInetSocket
{
public:
	Impl() : NativeInetSocket() {}
};

} // namespace io

CWT_NS_END

#endif /* __CWT_IO_INETSOCKET_UNIX_HPP__ */
