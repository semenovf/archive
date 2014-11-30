/**
 * @file scoped_ptr.hpp
 * @author wladt
 * @date Apr 28, 2014
 */

#ifndef __PFS_SCOPED_PTR_HPP__
#define __PFS_SCOPED_PTR_HPP__

#include <pfs.hpp>
#include <pfs/noncopyable.hpp>

namespace pfs {

// Simple implementation
template <typename T>
class scoped_ptr : noncopyable
{
protected:
	T * _value;

public:
	typedef T value_type;

public:
	scoped_ptr() : _value(nullptr) { }
	explicit scoped_ptr (value_type * p) : _value(p) {}
	scoped_ptr (const scoped_ptr & other) : _value(other._value) {}
	~scoped_ptr () { delete _value; }

    inline scoped_ptr & operator = (const scoped_ptr<T> & other)
    {
    	scoped_ptr copy(other);
        swap(copy);
        return *this;
    }

    void swap (scoped_ptr & other)
    {
        pfs_swap(this->_value, other._value);
    }

    inline void reset ()
    {
    	scoped_ptr copy;
    	swap(copy);
    }

    value_type & operator * () const
    {
        PFS_ASSERT(_value != 0);
        return *_value;
    }

    value_type * operator -> () const
    {
        PFS_ASSERT(_value != 0);
        return _value;
    }

    value_type * get() const
    {
    	return _value;
    }

    // comparaison operators
	inline bool operator == (const scoped_ptr & ptr) const
	{
		return (_value == ptr._value);
	}

	inline bool operator == (const T * p) const
	{
		return (_value == p);
	}

	inline bool operator != (const scoped_ptr & ptr) const
	{
		return (_value != ptr._value);
	}

	inline bool operator != (const T * p) const
	{
		return (_value != p);
	}

	inline bool operator <= (const scoped_ptr & ptr) const
	{
		return (_value <= ptr._value);
	}

	inline bool operator <= (const T * p) const
	{
		return (_value <= p);
	}

	inline bool operator < (const scoped_ptr & ptr) const
	{
		return (_value < ptr._value);
	}

	inline bool operator< (const T * p) const
	{
		return (_value < p);
	}

	inline bool operator >= (const scoped_ptr & ptr) const
	{
		return (_value >= ptr._value);
	}

	inline bool operator >= (const T * p) const
	{
		return (_value >= p);
	}

	inline bool operator > (const scoped_ptr & ptr) const
	{
		return (_value > ptr._value);
	}

	inline bool operator > (const T * p) const
	{
		return (_value > p);
	}

};

// Simple implementation
template <typename T>
class scoped_array_ptr : public scoped_ptr<T>
{
public:
	scoped_array_ptr () : scoped_ptr<T>() { }
	explicit scoped_array_ptr (T * p) : scoped_ptr<T>(p) { }
	~scoped_array_ptr () { delete [] scoped_ptr<T>::_value; }

	T & operator [] (int i) { return scoped_ptr<T>::_value[i]; }
	const T & operator [] (int i) const { return scoped_ptr<T>::_value[i]; }
};


} // pfs

#endif /* __PFS_SCOPED_PTR_HPP__ */
