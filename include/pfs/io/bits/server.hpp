/*
 * server.hpp
 *
 *  Created on: Jan 13, 2016
 *      Author: wladt
 */

#ifndef __PFS_IO_BITS_SERVER_HPP__
#define __PFS_IO_BITS_SERVER_HPP__

#include <pfs/error_code.hpp>
#include <pfs/io/bits/device.hpp>


namespace pfs { namespace io {

enum server_type
{
	  server_unknown = 0
	, server_tcp
    , server_udp
};

}}

namespace pfs { namespace io { namespace bits {

struct server : public basic_device
{
	typedef device::native_handle_type native_handle_type;

    server ()
		: basic_device()
	{}

    virtual ~server () {}

    virtual bool opened () const = 0;

    virtual error_code close () = 0;

    virtual bool set_nonblocking (bool on) = 0;

    virtual bool is_nonblocking () const = 0;
    
    virtual error_code accept (bits::device **, bool non_blocking) = 0;

    virtual native_handle_type native_handle () const = 0;

    virtual server_type type () const = 0;
};

}}} // pfs::io::bits

#endif /* __PFS_IO_BITS_SERVER_HPP__ */
