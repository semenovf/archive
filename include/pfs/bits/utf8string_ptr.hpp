/*
 * utf8string.hpp
 *
 *  Created on: Jul 21, 2013
 *      Author: wladt
 */

#ifndef __PFS_BITS_UTF8STRING_PTR_HPP__
#define __PFS_BITS_UTF8STRING_PTR_HPP__

#include <pfs.hpp>
#include <pfs/ucchar.hpp>
#include <string>

namespace pfs {

class utf8string;

class DLL_API utf8string_ptr
{
public:
	typedef ucchar     value_type;
	typedef ptrdiff_t difference_type;
	typedef const ucchar * pointer;
	typedef const ucchar /*& */reference;

private:
	std::string::const_pointer _p;     // pointer to begin of code units

	value_type value () const;
	void jump_forward (int n);
	void jump_backward (int n);

public:
	utf8string_ptr () : _p(nullptr) {}
	utf8string_ptr (const std::string::pointer p) : _p(p) {}
	utf8string_ptr (std::string::const_pointer p) : _p(p) {}
	//utf8string_ptr (std::string::const_iterator p) : _p(& *p) {}

	std::string::const_pointer ptr () const { return _p; }
	value_type operator * () const { return value(); }
    value_type operator -> () const { return value(); }

    utf8string_ptr & operator ++ ()
	{
    	jump_forward(1);
    	return *this;
	}

    utf8string_ptr operator ++ (int)
    {
    	utf8string_ptr r(*this);
        this->operator ++();
        return r;
    }

    utf8string_ptr & operator -- ()
	{
   		jump_backward(1);
   		return *this;
	}

    utf8string_ptr operator -- (int)
    {
    	utf8string_ptr r(*this);
        this->operator --();
        return r;
    }

    utf8string_ptr operator + (difference_type n) const
    {
    	utf8string_ptr it(*this);
    	it += n;
    	return it;
    }

    utf8string_ptr operator - (difference_type n ) const
    {
    	utf8string_ptr it(*this);
    	it -= n;
    	return it;
    }

    utf8string_ptr & operator += (difference_type n)
    {
    	jump_forward(n);
      	return *this;
    }

    utf8string_ptr & operator -= (difference_type n)
    {
    	jump_backward(n);
      	return *this;
    }

    // Random access iterator requirements
    value_type operator [] (difference_type n) const
    {
    	utf8string_ptr it(*this);
    	it += n;
    	return it.value();
    }

    bool operator == (const utf8string_ptr & o) const { return _p == o._p; }
    bool operator != (const utf8string_ptr & o) const { return !(_p == o._p); }
	bool operator  > (const utf8string_ptr & o) const { return _p >  o._p; }
	bool operator >= (const utf8string_ptr & o) const { return _p >= o._p; }
	bool operator  < (const utf8string_ptr & o) const { return _p <  o._p; }
	bool operator <= (const utf8string_ptr & o) const { return _p <= o._p; }

	friend difference_type operator - (const utf8string_ptr & p1, const utf8string_ptr & p2);
};

} // pfs

#endif /* __PFS_BITS_UTF8STRING_POINTER_HPP__ */
