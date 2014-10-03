/*
 * pointer.hpp
 *
 *  Created on: Sep 17, 2014
 *      Author: wladt
 */

#ifndef __PFS_BITS_POINTER_HPP__
#define __PFS_BITS_POINTER_HPP__

namespace pfs {

#ifdef __COMMENT__

template <typename Holder>
class pointer
{
public:
	typedef typename Holder::value_type       value_type;
	typedef typename Holder::pointer_type     pointer_type;
	typedef typename Holder::reference_type   reference_type;
	typedef typename Holder::difference_type  difference_type;

protected:
	pointer_type _p;

public:
	pointer (const pointer_type & p) : _p(p) {}
	pointer (const pointer & ptr) : _p(ptr._p) {}

	reference_type operator * () const { return *_p; }
	pointer_type operator -> () const { return _p; }

	pointer & operator ++ ()
	{
    	return this->operator += (1);
	}

	pointer operator ++ (int)
    {
		pointer r(*this);
        this->operator ++ ();
        return r;
    }

    pointer & operator -- ()
	{
    	return this->operator -= (1);
	}

    pointer operator -- (int)
    {
    	pointer r(*this);
        this->operator -- ();
        return r;
    }

    pointer operator + (difference_type n) const
    {
    	pointer ptr(*this);
    	ptr._p += n;
    	return ptr;
    }

    pointer operator - (difference_type n) const
    {
    	pointer ptr(*this);
    	ptr._p -= n;
    	return ptr;
    }

    pointer & operator += (difference_type n)
	{
    	_p += n;
		return *this;
	}

    pointer & operator -= (difference_type n)
	{
    	_p -= n;
		return *this;
	}

    // Random access iterator requirements
    reference_type operator [] (difference_type n) const
    {
    	pointer ptr(*this);
    	ptr += n;
    	return *ptr._p();
    }

    difference_type distance (const pointer & ptr) const
    {
    	difference_type d = _p - ptr._p;
    	return d < 0 ? d * -1 : d;
    }

    difference_type offset (const pointer & ptr) const
    {
    	return _p - ptr._p;
    }

//    bool operator == (const pointer & o) const { return _p == o._p; }
//    bool operator != (const pointer & o) const { return !(_p == o._p); }
//	bool operator  > (const pointer & o) const { return !(_p == o._p || _p < o._p); }
//	bool operator >= (const pointer & o) const { return !(_p < o._p); }
//	bool operator  < (const pointer & o) const { return _p <  o._p; }
//	bool operator <= (const pointer & o) const { return _p == o._p || _p < o._p; }
};
#endif

} // pfs

#endif /* __PFS_BITS_POINTER_HPP__ */
