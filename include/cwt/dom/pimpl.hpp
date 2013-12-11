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

#endif /* __CWT_DOM_PIMPL_HPP__ */
