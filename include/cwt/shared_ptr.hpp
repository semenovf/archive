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

CWT_NS_BEGIN

template <typename T>
struct default_deleter {
	void operator()(T* p) const {
		delete p;
	}
};

struct ref_count
{
	atomic_int weakref;
	atomic_int strongref;

	ref_count()
	{
		strongref.store(1);
		weakref.store(1);
	}

	~ref_count()
	{
		CWT_ASSERT(!weakref.load());
		CWT_ASSERT(strongref.load() <= 0);
	}

//     void destroy() { Deliter(this); }

//     inline void operator delete(void *ptr) { ::operator delete(ptr); }
//     inline void operator delete(void *, void *) { }
};


template <class T, typename Deleter>
struct ref_count_with_deleter: public ref_count
{
	typedef ref_count_with_deleter Self;
	typedef ref_count BaseClass;
	CustomDeleter<T, Deleter> extra;

	static void deleter(ref_count *self)
	{
		Self *realself = static_cast<Self *>(self);
		realself->extra.execute();

		// delete the deleter too
		realself->extra.~CustomDeleter<T, Deleter>();
	}

	static inline Self *create(T *ptr, Deleter userDeleter, DestroyerFn actualDeleter)
	{
		Self *d = static_cast<Self *>(::operator new(sizeof(Self)));

		// initialize the two sub-objects
		new (&d->extra) CustomDeleter<T, Deleter>(ptr, userDeleter);
		new (d) BaseClass(actualDeleter); // can't throw

		return d;
	}
private:
	// prevent construction
	ref_count_with_deleter();
	~ref_count_with_deleter();
	CWT_AVOID_COPY(ref_count_with_deleter);
};

template <class T>
class shared_ptr
{
private:
//    typedef T *shared_ptr:: *RestrictedBool;
//    typedef ref_count Data;
    template <typename Deleter>
    void construct(T *ptr, Deleter deleter)
    {
        if (!ptr) {
            d = 0;
            return;
        }

        typedef QtSharedPointer::ExternalRefCountWithCustomDeleter<T, Deleter> Private;
        typename Private::DestroyerFn actualDeleter = &Private::deleter;
        d = Private::create(ptr, deleter, actualDeleter);
    }

public:
//    typedef T Type;
//    typedef T element_type;
//    typedef T value_type;
//    typedef value_type *pointer;
//    typedef const value_type *const_pointer;
//    typedef value_type &reference;
//    typedef const value_type &const_reference;
//    typedef qptrdiff difference_type;

//    inline T *data() const { return value; }
//    inline bool isNull() const { return !data(); }
//    inline operator RestrictedBool() const { return isNull() ? 0 : &shared_ptr::value; }
//    inline bool operator !() const { return isNull(); }
//    inline T &operator*() const { return *data(); }
//    inline T *operator->() const { return data(); }

	// !done
    shared_ptr() : value(0), d(0) { }
    explicit shared_ptr(T *ptr) : value(ptr), d(0)
    {
    	construct(ptr, default_deleter<T>);
    }

    template <typename Deleter>
    shared_ptr(T *ptr, Deleter deleter) : value(ptr), d(0)
    {
    	construct(ptr, deleter);
    }

    // !done
    shared_ptr(const shared_ptr<T> &other) : value(other.value), d(other.d)
    {
    	if (d)
    		ref();
    }

    ~shared_ptr()
    {
    	deref();
    }


    // !done
    inline shared_ptr<T>& operator = (const shared_ptr<T> &other)
    {
        shared_ptr copy(other);
        swap(copy);
        return *this;
    }

#ifdef __COMMENT__
    // !done
    template <class X>
    inline shared_ptr(const shared_ptr<X> &other) : value(other.value), d(other.d)
    {
    	if (d)
    		ref();
    }


    template <class X>
    shared_ptr<T>& operator = (const shared_ptr<X> &other)
    {
        QSHAREDPOINTER_VERIFY_AUTO_CAST(T, X); // if you get an error in this line, the cast is invalid

        //internalCopy(other);
        //template <class X>
        //inline void internalCopy(const shared_ptr<X> &other)
        {
            ref_count *o = other.d;
            T *actual = other.value;
            if (o)
                other.ref();
            swap(d, o);
            swap(this->value, actual);
            deref(o);
        }

        return *this;
    }

    template <class X>
    shared_ptr(const QWeakPointer<X> &other) : value(0), d(0)
    {
    	*this = other;
    }

    template <class X>
    shared_ptr<T>& operator = (const QWeakPointer<X> &other)
    {
    	//internalSet(other.d, other.value);
        //internalSet(ref_count *o, T *actual)
		if (other.d) {
			// increase the strongref, but never up from zero
			// or less (-1 is used by QWeakPointer on untracked QObject)
			register int tmp = other.d->strongref.load();
			while (tmp > 0) {
				// try to increment from "tmp" to "tmp + 1"
				if (other.d->strongref.testAndSetRelaxed(tmp, tmp + 1))
					break;   // succeeded
				tmp = other.d->strongref.load();  // failed, try again
			}

			if (tmp > 0) {
				other.d->weakref.ref();
			} else {
				other.d = 0;
			}
		}

		qSwap(d, other.d);
		qSwap(this->value, other.value);
		if (!d || d->strongref.load() == 0)
			this->value = 0;

		// dereference saved data
		deref(other.d);

    	return *this;
    }
#endif

    // !done
    T& operator * () const
    {
        CWT_ASSERT(value != 0);
        return *value;
    }

    // !done
    T* operator -> () const
    {
        CWT_ASSERT(value != 0);
        return value;
    }


    // ! done
    T* get() const
    {
    	return value;
    }

    // ! done
    void swap(shared_ptr &other)
    {
        cwt::swap(d, other.d);
        cwt::swap(this->value, other.value);
    }

    // ! done
    inline void reset()
    {
    	shared_ptr copy;
    	swap(copy);
    }

    // ! done
    inline void reset(T *ptr)
    {
    	shared_ptr copy(ptr);
    	swap(copy);
    }

    // ! done
    template <typename Deleter>
    inline void reset(T *ptr, Deleter deleter)
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


#ifdef __COMMENT__
    //inline void clear() { shared_ptr copy; swap(copy); }

//    QWeakPointer<T> toWeakRef() const;

    static inline shared_ptr<T> create()
    {
        typedef QtSharedPointer::ExternalRefCountWithContiguousData<T> Private;
        typename Private::DestroyerFn destroy = &Private::deleter;
        shared_ptr<T> result(Qt::Uninitialized);
        result.d = Private::create(&result.value, destroy);

        // now initialize the data
        new (result.data()) T();
        return result;
    }
#endif
private:
    static void deref(ref_count *d)
    {
        if (!d)
        	return;
        if (!d->strongref.deref()) {
            d->destroy();
        }
        if (!d->weakref.deref())
            delete d;
    }

    // !done
    void deref()
    {
    	deref(d);
    }

    // !done
    void ref() const
    {
    	d->weakref.ref();
    	d->strongref.ref();
    }

private:
    T *value;
    ref_count *d;
};

// !done
template< class T >
inline void swap(shared_ptr<T> &a, shared_ptr<T> &b )
{
	a.swap(b);
}

// !done
template< class T, class U >
inline shared_ptr<T> static_pointer_cast (const shared_ptr<U> &r)
{
	return shared_ptr<T>(r, static_cast<U*>(r.get()));
}

// !done
template< class T, class U >
inline shared_ptr<T> dynamic_pointer_cast (const shared_ptr<U> &r)
{
	T* p = dynamic_cast<T*>(r.get());
	if (p)
		return shared_ptr<T>(r, p);
	return shared_ptr<T>();
}

// !done
template< class T, class U >
inline shared_ptr<T> const_pointer_cast (const shared_ptr<U> &r )
{
	return shared_ptr<T>(r, const_cast<T*>(r.get()));
}

CWT_NS_END

#endif /* __CWT_SHARED_PTR_HPP__ */
