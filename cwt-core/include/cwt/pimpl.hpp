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
	CWT_DENY_COPY(Class)                                                       \
protected:                                                                     \
    class Impl;                                                                \
    shared_ptr<Impl> pimpl; // TODO may be unique_ptr<> is more suitable in this case


#define CWT_PIMPL_COPYABLE(Class)                                              \
public:                                                                        \
	Class(const Class & other) : pimpl(other.pimpl) {}                         \
	Class & operator = (const Class & other) {                                 \
		pimpl = other.pimpl; return *this;                                     \
	}                                                                          \
protected:                                                                     \
	void detach()                                                              \
    {                                                                          \
		if (!pimpl.unique()) {                                                 \
			shared_ptr<Impl> __d(new Impl(*pimpl));                            \
			pimpl.swap(__d);                                                   \
		}                                                                      \
	}                                                                          \
protected:                                                                     \
    shared_ptr<Impl> pimpl;


#define CWT_PIMPL_DECL_COPYABLE(Class)                                         \
public:                                                                        \
	Class(const Class &other) : pimpl(other.pimpl) {}                          \
	Class & operator = (const Class & other) {                                 \
		pimpl = other.pimpl; return *this;                                     \
	}                                                                          \
protected:                                                                     \
	void detach();                                                             \
	class Impl;                                                                \
    shared_ptr<Impl> pimpl;

#define CWT_PIMPL_IMPL_COPYABLE(Class)                                         \
	void Class::detach()                                                       \
    {                                                                          \
		if (!pimpl.unique()) {                                                 \
			shared_ptr<Class::Impl> __d(new Class::Impl(*pimpl));              \
			pimpl.swap(__d);                                                   \
		}                                                                      \
	}

#endif /* __CWT_PIMPL_HPP__ */
