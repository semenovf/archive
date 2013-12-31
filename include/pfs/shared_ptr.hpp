/**
 * @file   shared_ptr.hpp
 * @author wladt
 * @date   Nov 20, 2013
 *
 * @brief  derived from: cwt-core/include/cwt/shared_ptr.hpp (Mar 26, 2013 1:47:13 PM)
 */

#ifndef __PFS_SHARED_PTR_HPP__
#define __PFS_SHARED_PTR_HPP__

#include <pfs.hpp>

#ifdef HAVE_STD_SHARED_PTR
#	include <memory>
#else
#	include <pfs/utility.hpp>
#	include <pfs/atomic.hpp>
#endif

namespace pfs {

#ifdef HAVE_STD_SHARED_PTR

template<typename T>
using shared_ptr = std::shared_ptr<T>;

#else

template <typename T>
struct default_deleter {
	void operator () (T * p) const {
		delete p;
	}
};

template <typename T>
struct custom_deleter {
	void operator () (T *) const {}
};

struct ref_count
{
	typedef void (* ref_count_deleter)(ref_count *);

	atomic_int weakref;
	atomic_int strongref;
	ref_count_deleter deleter_fn;

	ref_count () : deleter_fn(nullptr)
	{
		strongref.store(1);
		weakref.store(1);
	}

	~ref_count ()
	{
		PFS_ASSERT(!weakref.load());
		PFS_ASSERT(strongref.load() <= 0);
	}

     void destroy() { deleter_fn(this); } // need check
};


template <typename T, typename Deleter>
class ref_count_with_deleter: public ref_count
{
private:
	ref_count_with_deleter();
	PFS_DENY_COPY(ref_count_with_deleter)

public:
	ref_count_with_deleter(T * ptr, Deleter d) : ref_count(), m_ptr(ptr), m_deleter(d) { deleter_fn = deleter; }
	~ref_count_with_deleter() { this->~ref_count(); }

	static void deleter(ref_count * self)
	{
		reinterpret_cast<ref_count_with_deleter *>(self)->m_deleter(
				reinterpret_cast<ref_count_with_deleter *>(self)->m_ptr);
	}

private:
	T *     m_ptr;
	Deleter m_deleter;
};

template <class T>
class shared_ptr
{
protected:
    template <typename Deleter>
    void construct (T * ptr, Deleter deleter)
    {
        if (!ptr) {
            _d = 0;
            return;
        }
        _d = new ref_count_with_deleter<T, Deleter>(ptr, deleter);
    }

public:
    shared_ptr () : _value(0), _d(0)
    {}

    explicit shared_ptr (T * ptr) : _value(ptr), _d(0)
    {
    	if (ptr) {
    		default_deleter<T> deleter;
    		construct<default_deleter<T> >(ptr, deleter);
    	}
    }

    template <typename Deleter>
    shared_ptr (T * ptr, Deleter deleter) : _value(ptr), _d(0)
    {
    	construct<Deleter>(ptr, deleter);
    }

    template <typename T1>
    shared_ptr (const shared_ptr<T1> & other, T * p) : _value(p), _d(other.d)
    {}

    shared_ptr (const shared_ptr<T> & other) : _value(other._value), _d(other._d)
    {
    	if (_d)
    		ref();
    }

    ~shared_ptr ()
    {
    	deref();
    }

    inline shared_ptr<T> & operator = (const shared_ptr<T> & other)
    {
        shared_ptr copy(other);
        swap(copy);
        return *this;
    }

    T & operator * () const
    {
        PFS_ASSERT(_value != 0);
        return *_value;
    }

    T * operator -> () const
    {
        PFS_ASSERT(_value != 0);
        return _value;
    }

	inline operator bool() const throw()
	{
	   return (0 < use_count());
	}

    T * get() const
    {
    	return _value;
    }

    void swap(shared_ptr & other)
    {
        ::pfs::swap<ref_count *>(_d, other._d);
        ::pfs::swap(this->_value, other._value);
    }

    inline void reset()
    {
    	shared_ptr copy;
    	swap(copy);
    }

    inline void reset(T * ptr)
    {
    	shared_ptr copy(ptr);
    	swap(copy);
    }

    template <typename Deleter>
    inline void reset(T * ptr, Deleter deleter)
    {
    	shared_ptr copy(ptr, deleter);
    	swap(copy);
    }

    int use_count() const
    {
    	if (_value != 0)
    		return _d->strongref.load();
    	return 0;
    }

    /**
     * @brief Checks if *this is the only shared_ptr instance managing the current object, i.e. whether use_count() == 1.
     *
     * @return
     */
    bool unique() const
    {
    	return use_count() == 1 ? true : false;
    }

    // comparaison operators
	inline bool operator == (const shared_ptr & ptr) const
	{
		return (_value == ptr._value);
	}

	inline bool operator == (const T * p) const
	{
		return (_value == p);
	}

	inline bool operator != (const shared_ptr & ptr) const
	{
		return (_value != ptr._value);
	}

	inline bool operator != (const T * p) const
	{
		return (_value != p);
	}

	inline bool operator <= (const shared_ptr& ptr) const
	{
		return (_value <= ptr._value);
	}

	inline bool operator <= (const T * p) const
	{
		return (_value <= p);
	}

	inline bool operator < (const shared_ptr & ptr) const
	{
		return (_value < ptr._value);
	}

	inline bool operator< (const T * p) const
	{
		return (_value < p);
	}

	inline bool operator >= (const shared_ptr & ptr) const
	{
		return (_value >= ptr._value);
	}

	inline bool operator >= (const T * p) const
	{
		return (_value >= p);
	}

	inline bool operator > (const shared_ptr & ptr) const
	{
		return (_value > ptr._value);
	}

	inline bool operator > (const T * p) const
	{
		return (_value > p);
	}

private:
    static void deref(ref_count * d)
    {
        if (!d)
        	return;
        if (!d->strongref.deref()) {
            d->destroy();
        }
        if (!d->weakref.deref())
            delete d;
    }

    void deref()
    {
    	deref(_d);
    }

    void ref() const
    {
    	_d->weakref.ref();
    	_d->strongref.ref();
    }

protected:
    template <typename T1> friend  class shared_ptr;

    T * _value;
    ref_count * _d;
};

#endif // !HAVE_STD_SHARED_PTR

template <typename T>
class shareable
{
private:
	shareable();
public:
	shareable(const shareable & other) : d(other.d) {}
	shareable & operator = (const shareable & other) { d = other.d; return *this; }
	explicit shareable(T * ptr) : d(ptr) {}
	void detach()
	{
		if (d.use_count() > 1) {
			shared_ptr<T> dd(new T(*d));
			d.swap(dd);
		}
	}

    void swap(shareable & other) { d.swap(other.d); }
	bool unique() const { return d.unique(); }
	T * operator -> () const  { return d.get(); }

	shared_ptr<T> d;
};

template <typename T>
inline shared_ptr<T> make_shared() { return shared_ptr<T>(new T); }

template <class T, class Arg1>
inline shared_ptr<T> make_shared(Arg1 a1) { return shared_ptr<T>(new T(a1)); }

template <class T, class Arg1, class Arg2>
inline shared_ptr<T> make_shared(Arg1 a1, Arg2 a2) { return shared_ptr<T>(new T(a1, a2)); }

template <class T, class Arg1, class Arg2, class Arg3>
inline shared_ptr<T> make_shared(Arg1 a1, Arg2 a2, Arg3 a3) { return shared_ptr<T>(new T(a1, a2, a3)); }

template <class T, class Arg1, class Arg2, class Arg3, class Arg4>
inline shared_ptr<T> make_shared(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4) { return shared_ptr<T>(new T(a1, a2, a3, a4)); }

template <class T, class Arg1, class Arg2, class Arg3, class Arg4, class Arg5>
inline shared_ptr<T> make_shared(Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5) { return shared_ptr<T>(new T(a1, a2, a3, a4, a5)); }

/*
template< class T>
inline void swap (shared_ptr<T> & a, shared_ptr<T> & b )
{
	a.swap(b);
}
*/

template <class T, class T1>
inline shared_ptr<T> static_pointer_cast (const shared_ptr<T1> & r)
{
	return shared_ptr<T>(r, static_cast<T*>(r.get()));
}

template <class T, class T1>
inline shared_ptr<T> dynamic_pointer_cast (const shared_ptr<T1> & r)
{
	T * p = dynamic_cast<T*>(r.get());
	if (p)
		return shared_ptr<T>(r, p);
	return shared_ptr<T>();
}

template< class T, class T1>
inline shared_ptr<T> const_pointer_cast (const shared_ptr<T1> & r)
{
	return shared_ptr<T>(r, const_cast<T*>(r.get()));
}

} // namespace pfs

/**
@example Pattern of class based on shareable template

class ShareableClass
{
public:
	ShareableClass(const ShareableClass & other) : m_d(other.m_d) {}
	ShareableClass & operator = (const ShareableClass & other);

private:
	struct SharedData
	{
		// SharedData members
		...

		SharedData  () {}
		~SharedData () {
			// destroy SharedData members
			...
		}
		SharedData  (const SharedData & other) { / * clone shared data * / }
		SharedData & operator (const SharedData & other) { / * clone shared data * / return *this; }
	};
	cwt::shareable<SharedData> m_d;
};

ShareableClass & ShareableClass::operator = (const ShareableClass & other)
{
	m_d = other.m_d;
	return *this;
}
*/

#endif /* __PFS_SHARED_PTR_HPP__ */
