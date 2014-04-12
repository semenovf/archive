/**
 * @file pimpl.hpp
 * @author wladt
 * @date Nov 29, 2013
 */

#ifndef __PFS_PIMPL_HPP__
#define __PFS_PIMPL_HPP__

#include <pfs/shared_ptr.hpp>
#include <pfs/atomic.hpp>

namespace pfs {

class pimpl
{
	struct impl_base;
	impl_base * _holder;

public:
	template<typename T>
	pimpl (T * p) : _holder(new impl_holder<T>(p)) {}

	pimpl (const pimpl & other) : _holder(other._holder)
	{
		PFS_ASSERT(_holder);
		_holder->_ref.ref();
	}

	pimpl & operator = (const pimpl & other)
	{
		if (_holder) {
			deref();
		}

		_holder = other._holder;
		_holder->_ref.ref();
		return *this;
	}

	~pimpl () { deref(); }

	void swap (pimpl & o)
	{
		impl_base * tmp;
		tmp = _holder;
		_holder = o._holder;
		o._holder = tmp;
	}

private:
	void deref ()
	{
		if (!_holder->_ref.deref())
			delete _holder;
	}

private:
	struct impl_base
	{
		pfs::atomic_int _ref;
		impl_base () : _ref(1) {}
		virtual ~impl_base () {}
		virtual impl_base * clone () const = 0;// { PFS_ASSERT(false); return nullptr; }
	};

	template<typename T>
	struct impl_holder : impl_base
	{
		T * _d;
		impl_holder (T * p) : impl_base(), _d(p) {}
		~impl_holder () { delete _d; }

		T *       operator -> ()       { return _d; }
		const T * operator -> () const { return _d; }
		T &       operator *  ()       { return *_d; }
		const T & operator *  () const { return *_d; }

		impl_base * clone () const { return new impl_holder<T>(new T(*_d)); } // used in detach()
	};

public:
	void detach ()
	{
		if (_holder->_ref.load() > 1) { // not unique
			_holder->_ref.deref();
			_holder = _holder->clone();
		}
	}

	template <typename T>
	const impl_holder<T> & cast () const
	{
		return *static_cast<impl_holder<T> *>(_holder); // not dynamic cast
	}

	template <typename T>
	impl_holder<T> & cast ()
	{
		return *static_cast<impl_holder<T> *>(_holder); // not dynamic cast
	}
};

} // pfs


// TODO DEPRECATED {

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
// } TODO DEPRECATED

#endif /* __PFS_PIMPL_HPP__ */
