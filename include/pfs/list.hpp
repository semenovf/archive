/*
 * list.hpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */

#ifndef __PFS_LIST_HPP__
#define __PFS_LIST_HPP__

#include <list>
#include <pfs/shared_ptr.hpp>
#include <pfs/pimpl.hpp>

namespace pfs {

template <typename T>
class list
{
	typedef std::list<T> impl;
	PFS_PIMPL_INLINE(list, impl)

public:
	typedef typename impl::iterator iterator;
	typedef typename impl::const_iterator const_iterator;

public:
	list () : _pimpl(new impl()) {}

	size_t size () const { return _pimpl->size(); }
};

} // pfs

#endif /* __PFS_LIST_HPP__ */
