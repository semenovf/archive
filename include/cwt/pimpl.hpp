/**
 * @file   pimpl.hpp
 * @author wladt
 * @date   Apr 15, 2013 2:06:48 PM
 *
 * @brief
 */

#ifndef __CWT_PIMPL_HPP__
#define __CWT_PIMPL_HPP__

#include <cwt/cwt.h>
#include <cwt/shared_ptr.hpp>
#include <cwt/utility.hpp>

#define CWT_PIMPL_IMPL(Class)                                                  \
public:                                                                        \
	Class(const Class &other) : pimpl(other.pimpl) {}                          \
	Class& operator=(const Class &other) {                                     \
		pimpl == other.pimpl; return *this;                                    \
	}                                                                          \
private:                                                                       \
    class Impl;                                                                \
    shared_ptr<Impl> pimpl;

//CWT_NS_BEGIN
//CWT_NS_END

#endif /* __CWT_PIMPL_HPP__ */
