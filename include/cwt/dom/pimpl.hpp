/**
 * @file pimpl.hpp
 * @author wladt
 * @date Dec 11, 2013
 */

#ifndef __CWT_DOM_PIMPL_HPP__
#define __CWT_DOM_PIMPL_HPP__

#include <pfs/shared_ptr.hpp>

#define CWT_DOM_PIMPL_INLINE(Class)                            \
public:                                                        \
	class impl;                                                \
	typedef pfs::shared_ptr<impl> pimpl_type;                  \
	                                                           \
protected:                                                     \
	pimpl_type _pimpl;                                         \
                                                               \
protected:                                                     \
	Class (const pimpl_type & other) : _pimpl(other) {}        \
                                                               \
public:                                                        \
	Class ()  : _pimpl() {}                                    \
	Class (const Class & other) : _pimpl(other._pimpl) {}      \
	Class & operator = (const Class & other)                   \
	{                                                          \
		_pimpl = other._pimpl;                                 \
		return *this;                                          \
	}                                                          \
                                                               \
	bool operator == (const Class & other) const               \
			{ return _pimpl.operator == (other._pimpl); }      \
	bool operator != (const Class & other) const               \
			{ return _pimpl.operator != (other._pimpl); }


#define CWT_DOM_PIMPL_DERIVED(Class,Base)                      \
public:                                                        \
	class impl;                                                \
                                                               \
protected:                                                     \
	Class (const pfs::shared_ptr<impl> & other)                \
		: Base(dynamic_pointer_cast<Base::impl>(other)) {}     \
                                                               \
public:                                                        \
    Class () : Base () {}                                      \
    Class (const Class & a) : Base(a) {}                       \
    Class & operator = (const Class & a)                       \
    {                                                          \
    	Base::operator = (a);                                  \
    	return *this;                                          \
    }


#endif /* __CWT_DOM_PIMPL_HPP__ */
