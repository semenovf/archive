/**
 * @file pimpl.cpp
 * @author wladt
 * @date Apr 16, 2014
 */

#ifndef __PFS_PIMPL_CPP__
#define __PFS_PIMPL_CPP__

#include "pfs/pimpl.hpp"

namespace pfs {


//void pimpl::swap (pimpl & o)
//{
//	impl_base * tmp;
//	pfs::atomic_int i(0);
//	pfs::atomic_int j(0);
//
//	tmp = _holder;
//	_holder = o._holder;
//	o._holder = tmp;
//
//	if (_holder)
//		i.store(_holder->_ref.load());
//	if (o._holder)
//		j.store(o._holder->_ref.load());
//
//	if (_holder)
//		_holder->_ref.store(j.load());
//
//	if (o._holder)
//		o._holder->_ref.store(i.load());
//}

} // pfs

#endif /* __PFS_PIMPL_CPP__ */
