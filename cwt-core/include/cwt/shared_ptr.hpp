/**
 * @file   shared_ptr.hpp
 * @author wladt
 * @date   Mar 26, 2013 1:47:13 PM
 *
 * @brief
 */

#ifndef __CWT_SHARED_PTR_HPP__
#define __CWT_SHARED_PTR_HPP__

#include <cwt/cwt.h>
#include <cwt/utility.hpp>
#include <cwt/atomic.hpp>

CWT_NS_BEGIN

template <typename T>
struct default_deleter {
	void operator()(T * p) const {
		delete p;
	}
};

template <typename T>
struct custom_deleter {
	void operator()(T *) const {}
};

struct ref_count
{
	typedef void (* ref_count_deleter)(ref_count *);

	atomic_int weakref;
	atomic_int strongref;
	ref_count_deleter deleter_fn;

	ref_count() : deleter_fn(NULL)
	{
		strongref.store(1);
		weakref.store(1);
	}

	~ref_count()
	{
		CWT_ASSERT(!weakref.load());
		CWT_ASSERT(strongref.load() <= 0);
	}

     void destroy() { deleter_fn(this); } // need check
};


template <class T, typename Deleter>
class ref_count_with_deleter: public ref_count
{
private:
	ref_count_with_deleter();
	CWT_DENY_COPY(ref_count_with_deleter)

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
class DLL_API shared_ptr
{
private:
    template <typename Deleter>
    void construct(T * ptr, Deleter deleter)
    {
        if (!ptr) {
            d = 0;
            return;
        }
        d = new ref_count_with_deleter<T, Deleter>(ptr, deleter);
    }

public:
    shared_ptr() : value(0), d(0)
    {
    }

    explicit shared_ptr(T * ptr) : value(ptr), d(0)
    {
    	default_deleter<T> deleter;
    	construct<default_deleter<T> >(ptr, deleter);
    }

    template <typename Deleter>
    shared_ptr(T * ptr, Deleter deleter) : value(ptr), d(0)
    {
    	construct<Deleter>(ptr, deleter);
    }

    template <typename T1>
    shared_ptr(const shared_ptr<T1> & other, T * p) : value(p), d(other.d)
    {
    }

    shared_ptr(const shared_ptr<T> & other) : value(other.value), d(other.d)
    {
    	if (d)
    		ref();
    }

    ~shared_ptr()
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
        CWT_ASSERT(value != 0);
        return *value;
    }

    T * operator -> () const
    {
        CWT_ASSERT(value != 0);
        return value;
    }

	inline operator bool() const throw()
	{
	   return (0 < use_count());
	}

    T * get() const
    {
    	return value;
    }

    void swap(shared_ptr & other)
    {
        cwt::swap(d, other.d);
        cwt::swap(this->value, other.value);
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
    	if (value != 0)
    		return d->strongref.load();
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
		return (value == ptr.value);
	}

	inline bool operator == (const T * p) const
	{
		return (value == p);
	}

	inline bool operator != (const shared_ptr & ptr) const
	{
		return (value != ptr.value);
	}

	inline bool operator != (const T * p) const
	{
		return (value != p);
	}

	inline bool operator <= (const shared_ptr& ptr) const
	{
		return (value <= ptr.value);
	}

	inline bool operator <= (const T * p) const
	{
		return (value <= p);
	}

	inline bool operator < (const shared_ptr & ptr) const
	{
		return (value < ptr.value);
	}

	inline bool operator< (const T * p) const
	{
		return (value < p);
	}

	inline bool operator >= (const shared_ptr & ptr) const
	{
		return (value >= ptr.value);
	}

	inline bool operator >= (const T * p) const
	{
		return (value >= p);
	}

	inline bool operator > (const shared_ptr & ptr) const
	{
		return (value > ptr.value);
	}

	inline bool operator > (const T * p) const
	{
		return (value > p);
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
    	deref(d);
    }

    void ref() const
    {
    	d->weakref.ref();
    	d->strongref.ref();
    }

private:
    template <typename T1> friend  class shared_ptr;

    T * value;
    ref_count * d;
};

template <typename T>
class DLL_API shareable
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

template< class T>
inline void swap(shared_ptr<T> &a, shared_ptr<T> &b )
{
	a.swap(b);
}

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

CWT_NS_END

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

#endif /* __CWT_SHARED_PTR_HPP__ */
