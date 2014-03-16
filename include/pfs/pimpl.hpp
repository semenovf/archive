/**
 * @file pimpl.hpp
 * @author wladt
 * @date Nov 29, 2013
 */

#ifndef __PFS_PIMPL_HPP__
#define __PFS_PIMPL_HPP__

#include <pfs/shared_ptr.hpp>

namespace pfs {

//#define PFS_PIMPL_INLINE(Class,ImplClassScope,Impl)
template <typename Impl>
class pimpl
{
	pfs::shared_ptr<Impl> _pimpl;

public:
	template <typename Impl1>
	void detach ()
	{
		if (_pimpl.get() != nullptr) {
			if (!_pimpl.unique()) {
				pfs::shared_ptr<Impl> d(new Impl1(*_pimpl));
				_pimpl.swap(d);
			}
		}
	}

	pfs::shared_ptr<Impl> & operator -> ()
	{
		if (_pimpl.get() == nullptr)
			_pimpl = pfs::make_shared<Impl>();
		return _pimpl;
	}

	const pfs::shared_ptr<Impl> & operator -> () const
	{
		return _pimpl;
	}

	template <typename Impl1>
	Impl1 & cast ()
	{
		return *pfs::static_pointer_cast<Impl1,Impl>(_pimpl);
	}

	template <typename Impl1>
	const Impl1 & cast () const
	{
		return *pfs::static_pointer_cast<Impl1,Impl>(_pimpl);
	}

	template <typename Impl1>
	Impl1 & assign ()
	{
		if (_pimpl.get() == nullptr)
			_pimpl = pfs::dynamic_pointer_cast<Impl, Impl1>(pfs::make_shared<Impl1>());
		return static_cast<Impl1 &>(*_pimpl);
	}

public:
	pimpl () : _pimpl() {}
	pimpl (Impl * p) : _pimpl(p) {}
	pimpl (const pimpl & other) : _pimpl(other._pimpl) { }

	pimpl & operator = (const pimpl & other)
	{
		_pimpl = other._pimpl;
		return *this;
	}

	bool isNull () const
	{
		return _pimpl.get() == nullptr;
	}

	void swap (pimpl & other)
    {
    	_pimpl.swap(other._pimpl);
    }
};

} // pfs

#define PFS_PIMPL_DECL(Class,ImplClassScope,Impl,PimplScope)   \
ImplClassScope:                                                \
	class Impl;                                                \
PimplScope:	                                                   \
	pfs::shared_ptr<Impl> _pimpl;                              \
protected:                                                     \
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
	bool isNull () const { return _pimpl.get() == nullptr; }   \
	                                                           \
    void swap (Class & other)                                  \
    {                                                          \
    	pfs::swap(_pimpl, other._pimpl);                       \
    }

// w/o copy constructor
//
#define PFS_PIMPL_DECL_NOCC(Class,Impl)                        \
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
	bool isNull () const { return _pimpl.get() == nullptr; }   \
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











#define PFS_PIMPL_INLINE(Class,ImplClassScope,Impl)            \
ImplClassScope:                                                \
	pfs::shared_ptr<Impl> _pimpl;                              \
protected:                                                     \
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
	bool isNull () const { return _pimpl.get() == nullptr; }   \
	                                                           \
    void swap (Class & other)                                  \
    {                                                          \
    	_pimpl.swap(other._pimpl);                             \
    }

#define PFS_PIMPL_DECL(Class,ImplClassScope,Impl,PimplScope)   \
ImplClassScope:                                                \
	class Impl;                                                \
PimplScope:	                                                   \
	pfs::shared_ptr<Impl> _pimpl;                              \
protected:                                                     \
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
	bool isNull () const { return _pimpl.get() == nullptr; }   \
	                                                           \
    void swap (Class & other)                                  \
    {                                                          \
    	pfs::swap(_pimpl, other._pimpl);                       \
    }

// w/o copy constructor
//
#define PFS_PIMPL_DECL_NOCC(Class,Impl)                        \
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
	bool isNull () const { return _pimpl.get() == nullptr; }   \
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


/*
#define PFS_PIMPL_DEF1(Class,Impl)                             \
void Class::detach()                                           \
{                                                              \
	if (!_pimpl.unique()) {                                    \
		pfs::shared_ptr<Impl> d(new Impl(*_pimpl));            \
		_pimpl.swap(d);                                        \
	}                                                          \
}                                                              \
                                                               \
Class::Class (const Impl & other)                              \
	: _pimpl(new Impl(other)) {}
*/

#endif /* __PFS_PIMPL_HPP__ */
