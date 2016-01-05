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
	pimpl () : _holder(0) {}

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
	    if (_holder != other._holder) {
	        deref();
	        _holder = other._holder;

	        if (_holder)
	            _holder->_ref.ref();
	    }
		return *this;
	}

	~pimpl ()
	{
	    deref();
	}

	bool isNull () const { return _holder == 0; }
	bool isUnique () const
	{
		return isNull() || _holder->_ref.load() == 1;
	}

	void swap (pimpl & o)
	{
	    if (!this->isNull())
	        this->detach();

	    if (!o.isNull())
	        o.detach();

	    std::swap(_holder, o._holder);
	}

    void reset ()
    {
        pimpl copy;
        swap(copy);
    }

protected:
	void deref ()
	{
		if (_holder && !_holder->_ref.deref()) {
			delete _holder;
			_holder = 0;
		}
	}

protected:
	struct impl_base
	{
		pfs::atomic_int _ref;
		impl_base () : _ref(1) {}
		virtual ~impl_base () {}
		virtual impl_base * clone () const = 0;
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

		//
		// used in detach()
		//
		impl_base * clone () const
		{
			return new impl_holder<T, Helper>(_helper.clone(impl_cast_holder<T>::_d));
		}
	};

public:
	void detach ()
	{
#ifdef _NDEBUG
		if (_holder && _holder->_ref.load() > 1) { // not unique
#else
	    if (_holder) {
	        int rf = _holder->_ref.load();
	        PFS_ASSERT(rf > 0);
	        if (rf > 1) {
#endif
	            _holder->_ref.deref();
	            _holder = _holder->clone();
#ifndef _NDEBUG
	        }
#endif
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

template <typename T>
struct default_allocator
{
	T * operator () () const { return new T; }
};

template <typename T, typename Alloc = default_allocator<T> >
class nullable
{
	typedef Alloc allocator;
#ifdef __PFS_OLD_IMPL__
	typedef void (nullable::* init_func)();
#endif

protected:
	pimpl _d;
#ifdef __PFS_OLD_IMPL__
	init_func _init;
#else
	//bool _initialized;
#endif

#ifdef __PFS_OLD_IMPL__
	void initial_init ()
	{
		PFS_ASSERT(_d.isNull());
		_d = pimpl(allocator()());
		_init = & nullable::init;
	}

	void init () {}
#else
	void init ()
	{
		PFS_ASSERT(_d.isNull());
		_d = pimpl(allocator()());
	}
#endif

protected:
	void detach () { _d.detach(); }

public:
#ifdef __PFS_OLD_IMPL__
	nullable () : _d(), _init(& nullable::initial_init) {}
	nullable (T * p) : _d(p), _init(& nullable::init) { }
	nullable (const nullable & other) : _d(other._d), _init(other._init) {}
#else
	nullable () : _d()/*, _initialized(false)*/ {}
	nullable (T * p) : _d(p)/*, _initialized(p ? true : false)*/ { }
	nullable (const nullable & other) : _d(other._d)/*, _initialized(other._initialized)*/ {}
#endif

	bool isNull () const  { return _d.isNull(); }
	bool isUnique () const  { return _d.isUnique(); }

	void swap (nullable & o)
	{
		_d.swap(o._d);
	}

	/// @see http://www.possibility.com/Cpp/const.html
#ifdef __PFS_OLD_IMPL__
	const T * cast () const
	{
		nullable * self = const_cast<nullable *>(this);
		(self->*_init)();
		return _d.cast<T>();
	}
	T * cast () { (this->*_init)(); return _d.cast<T>(); }
#else
	const T * cast () const
	{
		nullable * self = const_cast<nullable *>(this);
		if (_d.isNull())
			self->init();
		return _d.cast<T>();
	}

	T * cast ()
	{
		if (_d.isNull())
			init();
		return _d.cast<T>();
	}
#endif
};

} // pfs

#endif /* __PFS_PIMPL_HPP__ */
