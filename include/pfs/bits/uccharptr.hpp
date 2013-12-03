/**
 * @file uccharptr.hpp
 * @author wladt
 * @date Dec 3, 2013
 */

#ifndef __PFS_BITS_UCCHARPTR_HPP__
#define __PFS_BITS_UCCHARPTR_HPP__

#include <pfs/ucchar.hpp>

namespace pfs {

class utf8string;
class ucchar_ptr;

class ucchar_ref
{
	friend class ucchar_ptr;
public:
	typedef ptrdiff_t  difference_type;
	typedef ucchar     value_type;

private:
	difference_type _pos;
	utf8string & _owner;

public:
	ucchar_ref (difference_type offset, utf8string & owner);
	ucchar_ref & operator = (ucchar c);
	ucchar_ref & operator = (const ucchar_ref & other);
	operator ucchar () const;

	const char * ptr () const;

    bool operator == (const ucchar_ref & o) const { return ucchar(*this) == ucchar(o); }
    bool operator == (const ucchar & o) const     { return ucchar(*this) == o; }
    bool operator != (const ucchar_ref & o) const { return ucchar(*this) != ucchar(o); }
    bool operator != (const ucchar & o) const     { return ucchar(*this) != o; }
    bool operator >  (const ucchar_ref & o) const { return ucchar(*this) > ucchar(o); }
    bool operator >  (const ucchar & o) const     { return ucchar(*this) > o; }
    bool operator >= (const ucchar_ref & o) const { return ucchar(*this) >= ucchar(o); }
    bool operator >= (const ucchar & o) const     { return ucchar(*this) >= o; }
    bool operator <  (const ucchar_ref & o) const { return ucchar(*this) < ucchar(o); }
    bool operator <  (const ucchar & o) const     { return ucchar(*this) < o; }
    bool operator <= (const ucchar_ref & o) const { return ucchar(*this) <= ucchar(o); }
    bool operator <= (const ucchar & o) const     { return ucchar(*this) <= o; }
};

class ucchar_ptr
{
	ucchar_ref _ref;

public:
	typedef ptrdiff_t difference_type;

public:
	ucchar_ptr (utf8string & s) : _ref(0, s) {}
	bool isOutOfBounds () const;

	const ucchar_ref   operator * () const { return _ref; }
	ucchar_ref         operator * () { return _ref; }
    const ucchar_ref * operator -> () const { return & _ref; }
    ucchar_ref *       operator -> () { return & _ref; }

    ucchar_ptr & operator ++ ()
	{
    	return this->operator += (1);
	}

    ucchar_ptr operator ++ (int)
    {
    	ucchar_ptr r(*this);
        this->operator ++ ();
        return r;
    }

    ucchar_ptr & operator -- ()
	{
    	return this->operator -= (1);
	}

    ucchar_ptr operator -- (int)
    {
    	ucchar_ptr r(*this);
        this->operator -- ();
        return r;
    }

    ucchar_ptr operator + (difference_type n) const
    {
    	ucchar_ptr p(*this);
    	p += n;
    	return p;
    }

    ucchar_ptr operator - (difference_type n) const
    {
    	ucchar_ptr p(*this);
    	p -= n;
    	return p;
    }

    ucchar_ptr & operator += (difference_type n);
    ucchar_ptr & operator -= (difference_type n);

    // Random access iterator requirements
    ucchar_ref operator [] (difference_type n) const
    {
    	ucchar_ptr p(*this);
    	p += n;
    	return p._ref;
    }

    bool operator == (const ucchar_ptr & o) const { return _ref.ptr() == o._ref.ptr(); }
    bool operator != (const ucchar_ptr & o) const { return _ref.ptr() != o._ref.ptr(); }
	bool operator  > (const ucchar_ptr & o) const { return _ref.ptr() >  o._ref.ptr(); }
	bool operator >= (const ucchar_ptr & o) const { return _ref.ptr() >= o._ref.ptr(); }
	bool operator  < (const ucchar_ptr & o) const { return _ref.ptr() <  o._ref.ptr(); }
	bool operator <= (const ucchar_ptr & o) const { return _ref.ptr() <= o._ref.ptr(); }
};

} // pfs

#endif /* __PFS_BITS_UCCHARPTR_HPP__ */
