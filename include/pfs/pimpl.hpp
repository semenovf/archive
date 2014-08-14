/**
 * @file pimpl.hpp
 * @author wladt
 * @date Nov 29, 2013
 */

#ifndef __PFS_PIMPL_HPP__
#define __PFS_PIMPL_HPP__

#include <pfs/shared_ptr.hpp>
#include <pfs/atomic.hpp>
#include <pfs/utility.hpp>

namespace pfs {

template <typename T>
struct pimpl_default_helper
{
	T * clone (T * p) const {
		return new T(*p);
	}
	void destroy (T * p) const {
		delete p;
	}
};

class pimpl
{
protected:
	struct impl_base;
	impl_base * _holder;

public:
	pimpl () : _holder(nullptr) {}

	template<typename T>
	pimpl (T * p) : _holder(new impl_holder<T>(p)) { }

	pimpl (const pimpl & other) : _holder(other._holder)
	{
		if (_holder) {
			_holder->_ref.ref();
		}
	}

	pimpl & operator = (const pimpl & other)
	{
		deref();
		_holder = other._holder;

		if (_holder)
			_holder->_ref.ref();

		return *this;
	}

	~pimpl () { deref(); }

	bool isNull () const { return _holder == nullptr; }

	template <typename T>
	void swap (pimpl & o)
	{
		PFS_ASSERT(_holder);
		PFS_ASSERT(o._holder);
		pfs_swap(static_cast<impl_cast_holder<T> *>(_holder)->_d
				, static_cast<impl_cast_holder<T> *>(o._holder)->_d);
	}

protected:
	void deref ()
	{
		if (_holder && !_holder->_ref.deref())
			delete _holder;
	}

protected:
	struct impl_base
	{
		pfs::atomic_int _ref;
		impl_base () : _ref(1) {}
		virtual ~impl_base () {}
		virtual impl_base * clone () const = 0;// { PFS_ASSERT(false); return nullptr; }
	};

	// Need to avoid instantiation of T while casting to abstract class
	// Matter for MSVC (critically)
	template<typename T>
	struct impl_cast_holder : impl_base
	{
		T * _d;
		impl_cast_holder (T * p) : impl_base(), _d(p) {}
	};

	template<typename T, typename Helper = pimpl_default_helper<T> >
	struct impl_holder : public impl_cast_holder<T>
	{
		Helper _helper;
		impl_holder (T * p) : impl_cast_holder<T>(p) {}
		~impl_holder () { _helper.destroy(impl_cast_holder<T>::_d); }

		impl_base * clone () const
		{
			return new impl_holder<T, Helper>(_helper.clone(impl_cast_holder<T>::_d));
		} // used in detach()
	};

public:
	void detach ()
	{
		if (_holder && _holder->_ref.load() > 1) { // not unique
			_holder->_ref.deref();
			_holder = _holder->clone();
		}
	}

	template <typename T>
	const T * cast () const
	{
		PFS_ASSERT(_holder);
		impl_cast_holder<T> * holder = static_cast<impl_cast_holder<T> *>(_holder);
		return holder->_d;
	}

	template <typename T>
	T * cast ()
	{
		PFS_ASSERT(_holder);
		impl_cast_holder<T> * holder = static_cast<impl_cast_holder<T> *>(_holder);
		return holder->_d;
	}
};

// TODO must be DEPRICATED
template <typename T>
struct default_allocator
{
	T * operator () () const { return new T; }
};

// TODO must be DEPRICATED
template <typename T, typename Alloc = default_allocator<T> >
class pimpl_lazy_init
{
	typedef Alloc allocator;
	typedef void (pimpl_lazy_init::* init_func)();

protected:
	pimpl _d;
	init_func _init;

	void initial_init ()
	{
		PFS_ASSERT(_d.isNull());
		_d = pimpl(allocator()());
		_init = & pimpl_lazy_init::init;
	}

	void init () {}

protected:
	void detach () { _d.detach(); }

public:
	pimpl_lazy_init () : _d(), _init(& pimpl_lazy_init::initial_init) {}
	pimpl_lazy_init (T * p) : _d(p), _init(& pimpl_lazy_init::init) { }
	pimpl_lazy_init (const pimpl_lazy_init & other) : _d(other._d), _init(other._init) {}

	bool isNull () const  { return _d.isNull(); }
	void swap (pimpl_lazy_init & o) { _d.swap<T>(o._d);	}

	/// @see http://www.possibility.com/Cpp/const.html
	const T * cast () const
	{
		pimpl_lazy_init * self = const_cast<pimpl_lazy_init*>(this);
		(self->*_init)();
		return _d.cast<T>();
	}
	T * cast () { (this->*_init)(); return _d.cast<T>(); }
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
    	pfs_swap(_pimpl, other._pimpl);                       \
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
    	pfs_swap(_pimpl, other._pimpl);                       \
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
