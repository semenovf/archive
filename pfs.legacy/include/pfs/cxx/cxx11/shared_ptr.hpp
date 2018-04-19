/**
 * @file   shared_ptr.hpp
 * @author wladt
 * @date   Nov 20, 2013
 *
 * @brief  derived from: cwt-core/include/cwt/shared_ptr.hpp (Mar 26, 2013 1:47:13 PM)
 */

#ifndef __PFS_CXX11_SHARED_PTR_HPP__
#define __PFS_CXX11_SHARED_PTR_HPP__

#include <memory>

namespace pfs {

template<typename T>
using shared_ptr = std::shared_ptr<T>;

template <class T, class T1>
inline shared_ptr<T> reinterpret_pointer_cast (const shared_ptr<T1> & r)
{
	return shared_ptr<T>(r, reinterpret_cast<T*>(r.get()));
}

} // namespace pfs

#endif /* __PFS_CXX11_SHARED_PTR_HPP__ */
