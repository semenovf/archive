/*
 * mutex.hpp
 *
 *  Created on: Oct 23, 2016
 *      Author: wladt
 */

#ifndef __PFS_CXX_CXX11_MUTEX_HPP__
#define __PFS_CXX_CXX11_MUTEX_HPP__

#include <mutex>

namespace pfs {

//typedef native_handle_type decltype(std::mutex::native_handle);

using mutex = std::mutex;

template <typename Mutex>
using lock_guard = std::lock_guard<Mutex>;

template <typename Mutex>
using unique_lock = std::unique_lock<Mutex>;

#define DEFER_LOCK  std::defer_lock
#define TRY_TO_LOCK std::try_to_lock
#define ADOPT_LOCK  std::adopt_lock

} // pfs

#endif /* __PFS_CXX_CXX11_MUTEX_HPP__ */
