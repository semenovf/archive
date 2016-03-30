/*
 * unique_ptr.hpp
 *
 *  Created on: Mar 30, 2016
 *      Author: wladt
 */

#ifndef __PFS_UNIQUE_PTR_HPP__
#define __PFS_UNIQUE_PTR_HPP__

#include <pfs.hpp>

#if __cplusplus >= 201103L
#	define HAVE_STD_UNIQUE_PTR 1
#endif

#if HAVE_STD_UNIQUE_PTR
#	include <memory>
#else
#	include <pfs/utility.hpp>
#endif

namespace pfs {

#if HAVE_STD_UNIQUE_PTR

template <typename T, typename Deleter = std::default_delete<T> >
using unique_ptr = std::unique_ptr<T, Deleter>;

#else

/// Primary template of default_delete, used by unique_ptr
template <typename T>
struct default_delete
{
    default_delete () noexcept {}

    void operator() (T * p) const
    {
		delete p;
    }
};

// Specialization for arrays, default_delete.
//
template <typename T>
struct default_delete<T[]>
{
public:
    default_delete() noexcept {}

    void operator () (T * p) const
    {
    	delete [] p;
    }
};

// Unique_ptr for single objects.
//
template <typename T, typename Deleter = default_delete<T> >
class unique_ptr
{
    T *     _value;
    Deleter _deleter;

public:
    typedef T *     pointer;
    typedef T       element_type;
    typedef Deleter deleter_type;

private:
    // Disable copy from lvalue.
    unique_ptr (const unique_ptr &);
    unique_ptr & operator = (const unique_ptr &);

public:
    // Default constructor, creates a unique_ptr that owns nothing.
    unique_ptr () noexcept
    	: _value(0)
    {
    	// TODO Need to check null function pointer deleter
    }

    /**
     * @brief Takes ownership of a pointer.
     * @details The deleter will be value-initialized.
     *
     * @param p  A pointer to an object of @c element_type
     */
    explicit unique_ptr (pointer p) noexcept
    	: _value(p), _deleter(deleter_type())
    {
    	// TODO Need to check null function pointer deleter
    }

    /**
     * @brief Takes ownership of a pointer.
     *
     * @details The deleter will be initialized with @p d
     * @param p  A pointer to an array of @c element_type
     * @param d  A reference to a deleter.
     */
    unique_ptr (pointer p, const deleter_type & d) noexcept
    		: _value(p), _deleter(d)
    {}

    // Destructor, invokes the deleter if the stored pointer is not null.
    ~unique_ptr () noexcept
    {
    	if (_value != 0)
    		_deleter(_value);
    	_value = 0;
    }

    // Observers.

    T & operator * () const
    {
        PFS_ASSERT(_value != 0);
        return *get();
    }


    pointer operator -> () const
    {
        PFS_ASSERT(_value != 0);
        return get();
    }

    pointer get () const noexcept
    {
    	return _value;
    }

    deleter_type & get_deleter() noexcept
    {
    	return _deleter;
    }

    const deleter_type & get_deleter() const noexcept
    {
    	return _deleter;
    }

    operator bool () const noexcept
    {
    	return get() == 0 ? false : true;
    }

    // Release ownership of any stored pointer.
    pointer release () noexcept
    {
    	pointer p = get();
    	_value = 0;
    	return p;
    }

    /**
     * @brief Replace the stored pointer.
     * @details The deleter will be invoked if a pointer is already owned.
     *
     * @param p  The new pointer to store.
     */
    void reset (pointer p = 0) noexcept
    {
    	pfs::swap(_value, p);

    	if (p != 0)
    		get_deleter()(p);
    	}

    /// Exchange the pointer and deleter with another object.
    void swap (unique_ptr & u) noexcept
    {
    	pfs::swap(_value, u._value);
    	pfs::swap(_deleter, u._deleter);
    }
};


/// 20.7.1.3 unique_ptr for array objects with a runtime length
template <typename T, typename Deleter>
class unique_ptr<T[], Deleter>
{
    T *     _value;
    Deleter _deleter;

public:
    typedef T *     pointer;
    typedef T       element_type;
    typedef Deleter deleter_type;

private:
    // Disable copy from lvalue.
    unique_ptr (const unique_ptr &);
    unique_ptr & operator = (const unique_ptr &);

public:
    // Default constructor, creates a unique_ptr that owns nothing.
    unique_ptr () noexcept
    	: _value(0)
    {
    	// TODO Need to check null function pointer deleter
    }

    /**
     * @brief Takes ownership of a pointer.
     * @details The deleter will be value-initialized.
     *
     * @param p  A pointer to an object of @c element_type
     */
    explicit unique_ptr (pointer p) noexcept
    	: _value(p), _deleter(deleter_type())
    {
    	// TODO Need to check null function pointer deleter
    }

    /**
     * @brief Takes ownership of a pointer.
     *
     * @details The deleter will be initialized with @p d
     * @param p  A pointer to an array of @c element_type
     * @param d  A reference to a deleter.
     */
    unique_ptr (pointer p, const deleter_type & d) noexcept
    		: _value(p), _deleter(d)
    {}

    /// Destructor, invokes the deleter if the stored pointer is not null.
    ~unique_ptr()
    {
		if (_value != 0)
			_deleter(_value);
		_value = 0;
    }

    // Observers.

    /// Access an element of owned array.
    T & operator [] (size_t i) const
    {
    	PFS_ASSERT(_value != 0);
    	return get()[i];
    }

    /// Return the stored pointer.
    pointer get () const noexcept
    {
    	return _value;
    }

    /// Return a reference to the stored deleter.
    deleter_type & get_deleter() noexcept
    {
    	return _deleter;
    }

    /// Return a reference to the stored deleter.
    const deleter_type & get_deleter() const noexcept
    {
    	return _deleter;
    }

    /// Return @c true if the stored pointer is not null.
    operator bool () const noexcept
    {
    	return get() == 0 ? false : true;
    }

    // Modifiers.

    /// Release ownership of any stored pointer.
    pointer release() noexcept
    {
    	pointer p = get();
    	_value = 0;
    	return p;
    }

    /**
     * @brief Replace the stored pointer.
     *
     * @details The deleter will be invoked if a pointer is already owned.
     *
     * @param p  The new pointer to store.
     */
    void reset (pointer p = 0) noexcept
    {
    	pfs::swap(_value, p);

    	if (p != 0)
    		get_deleter()(p);
    }

    /// Exchange the pointer and deleter with another object.
    void swap (unique_ptr & u) noexcept
    {
    	pfs::swap(_value, u._value);
    	pfs::swap(_deleter, u._deleter);
    }
};

#endif // !HAVE_STD_UNIQUE_PTR


template <typename T, typename Deleter>
inline void swap (unique_ptr<T, Deleter> & x, unique_ptr<T, Deleter> & y) noexcept
{
	x.swap(y);
}

template<typename T, typename Deleter, typename U, typename Ep>
inline bool operator == (const unique_ptr<T, Deleter> & x, const unique_ptr<U, Ep> & y)
{
	return x.get() == y.get();
}

template<typename T, typename Deleter, typename U, typename Ep>
inline bool operator != (const unique_ptr<T, Deleter> & x, const unique_ptr<U, Ep> & y)
{
	return x.get() != y.get();
}

template<typename T, typename Deleter, typename U, typename Ep>
inline bool operator < (const unique_ptr<T, Deleter> & x, const unique_ptr<U, Ep> & y)
{
	return x.get() < y.get();
}

template<typename T, typename Deleter, typename U, typename Ep>
inline bool operator <= (const unique_ptr<T, Deleter> & x, const unique_ptr<U, Ep> & y)
{
	return !(y < x);
}

template<typename T, typename Deleter, typename U, typename Ep>
inline bool operator > (const unique_ptr<T, Deleter> & x, const unique_ptr<U, Ep> & y)
{
	return (y < x);
}

template<typename T, typename Deleter, typename U, typename Ep>
inline bool operator >= (const unique_ptr<T, Deleter> & x, const unique_ptr<U, Ep> & y)
{
	return !(x < y);
}

} // namespace pfs


#endif /* __PFS_UNIQUE_PTR_HPP__ */
