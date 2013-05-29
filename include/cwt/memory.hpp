/**
 * @file   memory.hpp
 * @author wladt
 * @date   Feb 27, 2013 11:00:57 AM
 *
 * @brief
 */

#ifndef __CWT_MEMORY_HPP__
#define __CWT_MEMORY_HPP__

#include <cwt/cwt.h>
#include <memory>
#include <cwt/atomic.hpp>

CWT_NS_BEGIN

// FIXME add support for native C++11 implementation of unique_ptr and shared_ptr.

template <typename T>
class unique_ptr: public std::auto_ptr<T> {
public:
	explicit unique_ptr(T *p = 0) : std::auto_ptr<T>(p) { }
	unique_ptr(const unique_ptr &a) throw() : std::auto_ptr<T>(a) { }

    template<typename T1>
    unique_ptr(const unique_ptr<T1> &a) : std::auto_ptr<T>(a) { }
};

CWT_NS_END

#endif /* __CWT_MEMORY_HPP__ */
