/**
 * @file pimpl.hpp
 * @author wladt
 * @date Nov 29, 2013
 */

#ifndef __PFS_PIMPL_HPP__
#define __PFS_PIMPL_HPP__

#include <pfs/shared_ptr.hpp>

#define PFS_PIMPL_INLINE(Class,Impl)                           \
private:                                                       \
	pfs::shared_ptr<Impl> _pimpl;                              \
                                                               \
	void detach()                                              \
	{                                                          \
		if (!_pimpl.unique()) {                                \
			pfs::shared_ptr<Impl> d(new Impl(*_pimpl));        \
			_pimpl.swap(d);                                    \
		}                                                      \
	}                                                          \
                                                               \
	Class (const Impl & other) : _pimpl(new Impl(other)) {}    \
                                                               \
public:                                                        \
	Class (const Class & other) : _pimpl(other._pimpl) { }     \
	Class & operator = (const Class & other)                   \
	{                                                          \
		_pimpl = other._pimpl;                                 \
		return *this;                                          \
	}                                                          \
                                                               \
    void swap (Class & other)                                  \
    {                                                          \
    	pfs::swap(_pimpl, other._pimpl);                       \
    }

#define PFS_PIMPL_DECL(Class,Impl)                             \
private:                                                       \
	class Impl;                                                \
	pfs::shared_ptr<Impl> _pimpl;                              \
                                                               \
	void detach();                                             \
	Class (const Impl & other);                                \
                                                               \
public:                                                        \
	Class (const Class & other) : _pimpl(other._pimpl) { }     \
	Class & operator = (const Class & other)                   \
	{                                                          \
		_pimpl = other._pimpl;                                 \
		return *this;                                          \
	}                                                          \
                                                               \
    void swap (Class & other)                                  \
    {                                                          \
    	pfs::swap(_pimpl, other._pimpl);                       \
    }

// w/o copy constructor
//
#define PFS_PIMPL_DECL_NOCC(Class,Impl)                        \
private:                                                       \
	class Impl;                                                \
	pfs::shared_ptr<Impl> _pimpl;                              \
                                                               \
	void detach();                                             \
	Class (const Impl & other);                                \
                                                               \
public:                                                        \
	Class & operator = (const Class & other)                   \
	{                                                          \
		_pimpl = other._pimpl;                                 \
		return *this;                                          \
	}                                                          \
                                                               \
    void swap (Class & other)                                  \
    {                                                          \
    	pfs::swap(_pimpl, other._pimpl);                       \
    }


#define PFS_PIMPL_DEF(Class,Impl)                              \
void Class::detach()                                           \
{                                                              \
	if (!_pimpl.unique()) {                                    \
		pfs::shared_ptr<Class::Impl> d(new Class::Impl(*_pimpl)); \
		_pimpl.swap(d);                                        \
	}                                                          \
}                                                              \
                                                               \
Class::Class (const Class::Impl & other)                       \
	: _pimpl(new Class::Impl(other)) {}


#endif /* __PFS_PIMPL_HPP__ */
