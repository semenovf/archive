/*
 * device_pool.hpp
 *
 *  Created on: Jan 13, 2016
 *      Author: wladt
 */

#ifndef __PFS_IO_BITS_POOL_HPP__
#define __PFS_IO_BITS_POOL_HPP__

namespace pfs { namespace io { namespace bits {

struct pool {};

struct pool_iterator
{
	virtual ~pool_iterator () {}

	virtual void next () = 0;
};

}}}

#endif /* __PFS_IO_BITS_POOL_HPP__ */
