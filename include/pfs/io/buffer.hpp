/*
 * buffer.hpp
 *
 *  Created on: Jul 16, 2013
 *      Author: wladt
 */

#ifndef __PFS_IO_BUFFER_HPP__
#define __PFS_IO_BUFFER_HPP__

#include <pfs/io/device.hpp>
#include <pfs/byte_string.hpp>

namespace pfs { namespace io {

class DLL_API buffer : public device
{
public:
	buffer () : device() {}
	virtual ~buffer() {}

	const byte_t * constData () const;
	size_t size () const;
	byte_string data () const;
};

}} // pfs::io

#endif /* __PFS_IO_BUFFER_HPP__ */
