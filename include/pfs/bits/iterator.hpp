/**
 * @file iterator.hpp
 * @author wladt
 * @date Aug 20, 2014
 */

#ifndef __PFS_BITS_ITERATOR_HPP__
#define __PFS_BITS_ITERATOR_HPP__

#include <iterator>
#include <pfs.hpp> // for PFS_ASSERT
#include <pfs/bits/constness.hpp>
#include <pfs/bits/reference.hpp>
#include <pfs/utility.hpp>
#include <pfs/type_traits.hpp>

namespace pfs {

template <typename Holder>
class iterator_base
{
public:
	typedef Holder  holder_class;
	typedef typename constness<Holder>::pointer pointer;
	typedef typename constness<Holder>::reference reference;
	typedef typename holder_class::value_type value_type;
	typedef typename holder_class::size_type size_type;
	typedef typename holder_class::difference_type difference_type;

protected:
	holder_class * _holder;
	pointer        _ptr;

protected:
	iterator_base (holder_class * holder, pointer ptr)
		: _holder(holder), _ptr(ptr) {}

public:
	iterator_base (const iterator_base & o)
		: _holder(o._holder), _ptr(o._ptr) {}

	~iterator_base () {}

	iterator_base & operator = (const iterator_base & o)
	{
		_holder = o._holder;
		_ptr = o._ptr;
		return *this;
	}

	void swap (iterator_base & i1, iterator_base & i2)
	{
		pfs_swap(i1._holder, i2._holder);
		pfs_swap(i1._ptr, i2._ptr);
	}

	iterator_base & operator ++ () // prefix increment
	{
    	++_ptr;
    	return *this;
	}

    reference operator * () const
    {
    	return reference(*_holder, _ptr);
    }

    pointer base () const
    {
    	return _ptr;
    }

    holder_class * holder () const
    {
    	return _holder;
    }
};


template <typename Holder>
class output_iterator : public iterator_base<Holder>
{
public:
	typedef std::output_iterator_tag iterator_category;
	typedef typename iterator_base<Holder>::holder_class holder_class;
	typedef typename iterator_base<Holder>::pointer pointer;
	typedef typename iterator_base<Holder>::value_type value_type;
	typedef typename iterator_base<Holder>::reference reference;
	typedef typename iterator_base<Holder>::size_type size_type;
	typedef typename iterator_base<Holder>::difference_type difference_type;

public:
	output_iterator (holder_class * holder, pointer ptr)
		: iterator_base<Holder>(holder, ptr) {}

	explicit
	output_iterator (const iterator_base<Holder> & it)
		: iterator_base<Holder>(it) {}

/*
    reference operator * () const
    {
    	return reference(*this->_holder, this->_ptr);
    }
*/
	output_iterator & operator ++ () // prefix increment
	{
    	++this->_ptr;
    	return *this;
	}


	output_iterator operator ++ (int)
	{
		return output_iterator(this->_holder, this->_ptr++);
	}
};

template <typename Holder>
class input_iterator : public iterator_base<Holder>
{
public:
	typedef std::input_iterator_tag iterator_category;
	typedef typename iterator_base<Holder>::holder_class holder_class;
	typedef typename iterator_base<Holder>::pointer pointer;
	typedef typename iterator_base<Holder>::value_type value_type;
	typedef typename iterator_base<Holder>::reference reference;
	typedef typename iterator_base<Holder>::size_type size_type;
	typedef typename iterator_base<Holder>::difference_type difference_type;

public:
	input_iterator (holder_class * holder, pointer ptr)
		: iterator_base<Holder>(holder, ptr) {}

	explicit
	input_iterator (const iterator_base<Holder> & it)
		: iterator_base<Holder>(it) {}

    value_type operator * () const
    {
    	return reference(*this->_holder, this->_ptr);
    }

    pointer operator -> () const
    {
    	return this->_ptr;
    }

    input_iterator & operator ++ () // prefix increment
	{
    	++this->_ptr;
    	return *this;
	}

    input_iterator operator ++ (int) // postfix increment
	{
		return input_iterator(this->_holder, this->_ptr++);
	}
};

template <typename Holder1, typename Holder2>
inline bool operator == (const input_iterator<Holder1> & i1, const input_iterator<Holder2> & i2)
{
	return i1.base() == i2.base();
}

template <typename Holder1, typename Holder2>
inline bool operator != (const input_iterator<Holder1> & i1, const input_iterator<Holder2> & i2)
{
	return i1.base() != i2.base();
}

template <typename Holder>
class forward_iterator : public input_iterator<Holder>
{
public:
	typedef std::forward_iterator_tag iterator_category;
	typedef typename iterator_base<Holder>::holder_class holder_class;
	typedef typename iterator_base<Holder>::pointer pointer;
	typedef typename iterator_base<Holder>::value_type value_type;
	typedef typename iterator_base<Holder>::reference reference;
	typedef typename iterator_base<Holder>::size_type size_type;
	typedef typename iterator_base<Holder>::difference_type difference_type;

public:
	forward_iterator ()
		: input_iterator<Holder>(nullptr, pointer()) {}

	forward_iterator (holder_class * holder, pointer ptr)
		: input_iterator<Holder>(holder, ptr) {}

	explicit
	forward_iterator (const iterator_base<Holder> & it)
		: input_iterator<Holder>(it) {}

    reference operator * () const
    {
    	return reference(*this->_holder, this->_ptr);
    }

	forward_iterator & operator ++ ()
	{
    	++this->_ptr;
    	return *this;
	}

	forward_iterator operator ++ (int)
	{
		return forward_iterator(this->_holder, this->_ptr++);
	}
};

template <typename Holder>
class bidirectional_iterator : public forward_iterator<Holder>
{
public:
	typedef std::bidirectional_iterator_tag iterator_category;
	typedef typename iterator_base<Holder>::holder_class holder_class;
	typedef typename iterator_base<Holder>::pointer pointer;
	typedef typename iterator_base<Holder>::value_type value_type;
	typedef typename iterator_base<Holder>::reference reference;
	typedef typename iterator_base<Holder>::size_type size_type;
	typedef typename iterator_base<Holder>::difference_type difference_type;

public:
	bidirectional_iterator ()
		: forward_iterator<Holder>() {}

	bidirectional_iterator (holder_class * holder, pointer ptr)
		: forward_iterator<Holder>(holder, ptr) {}

	explicit
	bidirectional_iterator (const iterator_base<Holder> & it)
		: forward_iterator<Holder>(it) {}

	bidirectional_iterator & operator -- ()
	{
    	--this->_ptr;
    	return *this;
	}

	bidirectional_iterator operator -- (int)
	{
		return bidirectional_iterator(*this->_holder, this->_ptr--);
	}
};

template <typename Holder>
class random_access_iterator : public bidirectional_iterator<Holder>
{
public:
	typedef std::random_access_iterator_tag iterator_category;
	typedef typename iterator_base<Holder>::holder_class holder_class;
	typedef typename iterator_base<Holder>::pointer pointer;
	typedef typename iterator_base<Holder>::value_type value_type;
	typedef typename iterator_base<Holder>::reference reference;
	typedef typename iterator_base<Holder>::size_type size_type;
	typedef typename iterator_base<Holder>::difference_type difference_type;

public:
	random_access_iterator ()
		: bidirectional_iterator<Holder>() {}

	random_access_iterator (holder_class * holder, pointer ptr)
		: bidirectional_iterator<Holder>(holder, ptr) {}

	random_access_iterator (const random_access_iterator & it)
		: bidirectional_iterator<Holder>(it) {}

	explicit
	random_access_iterator (const iterator_base<Holder> & it)
		: bidirectional_iterator<Holder>(it) {}

	random_access_iterator & operator += (size_type n)
	{
		this->_ptr += n;
    	return *this;
	}

	random_access_iterator & operator -= (size_type n)
	{
    	this->_ptr -= n;
    	return *this;
	}

	reference operator [] (size_type index)
	{
		return reference(*this->_holder, this->base() + index);
	}
};


template <typename Holder1, typename Holder2>
inline bool operator < (const iterator_base<Holder1> & i1, const iterator_base<Holder2> & i2)
{
	return i1.base() < i2.base();
}

template <typename Holder1, typename Holder2>
inline bool operator > (const iterator_base<Holder1> & i1, const iterator_base<Holder2> & i2)
{
	return i1.base() > i2.base();
}

template <typename Holder1, typename Holder2>
inline bool operator <= (const iterator_base<Holder1> & i1, const iterator_base<Holder2> & i2)
{
	return i1.base() <= i2.base();
}

template <typename Holder1, typename Holder2>
inline bool operator >= (const iterator_base<Holder1> & i1, const iterator_base<Holder2> & i2)
{
	return i1.base() >= i2.base();
}

template <typename Holder>
inline random_access_iterator<Holder> operator + (const random_access_iterator<Holder> & i, typename random_access_iterator<Holder>::size_type n)
{
	return random_access_iterator<Holder>(i.holder(), i.base() + n);
}

template <typename Holder>
inline random_access_iterator<Holder> operator + (typename random_access_iterator<Holder>::size_type n, const random_access_iterator<Holder> & i)
{
	return random_access_iterator<Holder>(i.holder(), i.base() + n);
}

template <typename Holder>
inline random_access_iterator<Holder> operator - (const random_access_iterator<Holder> & i, typename random_access_iterator<Holder>::size_type n)
{
	return random_access_iterator<Holder>(i.holder(), i.base() - n);
}

// TODO is arguments must be a constant references? Check
template <typename Holder1, typename Holder2>
inline typename random_access_iterator<Holder1>::difference_type operator - (random_access_iterator<Holder1> i1, random_access_iterator<Holder2> i2)
{
	return i1.base() - i2.base();
}

} // pfs

#endif /* __PFS_BITS_ITERATOR_HPP__ */
