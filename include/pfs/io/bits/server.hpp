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

namespace pfs { namespace io { namespace bits {

struct server
{
	typedef device::native_handle_type native_handle_type;

    server () {}

    virtual ~server () {}

    virtual bool opened () const = 0;

    virtual bool close (error_code * ex) = 0;

    virtual bool set_nonblocking (bool on) = 0;

    virtual bool accept (bits::device **, bool non_blocking, error_code * ex) = 0;

    virtual native_handle_type native_handle () const = 0;
};

}}} // pfs::io::bits

#endif /* __PFS_IO_BITS_SERVER_HPP__ */
