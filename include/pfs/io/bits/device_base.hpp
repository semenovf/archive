/*
 * device_base.hpp
 *
 *  Created on: Jan 20, 2016
 *      Author: wladt
 */

#ifndef __PFS_IO_BITS_DEVICE_BASE_HPP__
#define __PFS_IO_BITS_DEVICE_BASE_HPP__

#include <pfs/error_code.hpp>

namespace pfs { namespace io { namespace bits {

// TODO Support other platform specific native file handle types.
//
typedef int native_handle_type;

struct device_base
{
	typedef pfs::io::bits::native_handle_type native_handle_type;
	typedef uint32_t open_mode_flags;

	// Contains additional (user defined information)
	//
	struct identifier
	{
	public:
		identifier () {}
		virtual ~identifier () {}
	};

	shared_ptr<identifier> _id;

	device_base () {}

    virtual ~device_base () {}

    void set_id (identifier * pid)
    {
    	shared_ptr<identifier> id(pid);
    	_id.swap(id);
    }

    const identifier * id () const
    {
    	return _id.get();
    }

};

}}} // pfs::io::bits

#endif /* __PFS_IO_BITS_DEVICE_BASE_HPP__ */
