/*
 * buffer.hpp
 *
 *  Created on: Jul 16, 2013
 *      Author: wladt
 */

#ifndef __PFS_IO_BUFFER_HPP__
#define __PFS_IO_BUFFER_HPP__

#include <pfs/io/device.hpp>

namespace pfs { namespace io {

class DLL_API buffer : public device
{
public:
	buffer () : device() {}
	virtual ~buffer() {}

	bool open (byte_t a[], size_t n, uint32_t oflags = ReadWrite);
	bool open (size_t sz, uint32_t oflags = ReadWrite);
    size_t size () const;
};

}} // pfs::io

#endif /* __PFS_IO_BUFFER_HPP__ */
