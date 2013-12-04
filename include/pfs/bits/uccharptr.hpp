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
class ucchar_ref;
class ucchar_const_ref;
class ucchar_ptr;
class ucchar_const_ptr;

template <typename _Str>
class ucchar_ref_basic;

template <typename _Str, typename _Ref>
class ucchar_ptr_basic;

template <typename _utf8string>
class ucchar_ref_basic
{
	friend class ucchar_ref;
	friend class ucchar_const_ref;
	friend class ucchar_ptr;
	friend class ucchar_const_ptr;

	template <typename _Str>
	friend class ucchar_ref_basic;

	template <typename _Str, typename _Ref>
	friend class ucchar_ptr_basic;

public:
	typedef ptrdiff_t difference_type;
	typedef ucchar    value_type;

protected:
	difference_type _pos;
	_utf8string *   _owner;

public:
	ucchar_ref_basic ()  : _pos(0), _owner(nullptr) {}
	ucchar_ref_basic (difference_type offset, _utf8string & owner);

	// Allow ucchar_ref to ucchar_const_ref conversion
	template<typename _Str2>
    ucchar_ref_basic(const ucchar_ref_basic<_Str2> & other)
    	: _pos(other._pos), _owner(other._owner) {}

	operator ucchar () const;
	value_type value () const { return this->operator ucchar (); }

	const char * ptr () const { return _owner->data(); }

    bool operator == (const ucchar_ref_basic<_utf8string> & o) const { return ucchar(*this) == ucchar(o); }
    bool operator == (const ucchar & o) const     { return ucchar(*this) == o; }
    bool operator != (const ucchar_ref_basic<_utf8string> & o) const { return ucchar(*this) != ucchar(o); }
    bool operator != (const ucchar & o) const     { return ucchar(*this) != o; }
    bool operator >  (const ucchar_ref_basic<_utf8string> & o) const { return ucchar(*this) > ucchar(o); }
    bool operator >  (const ucchar & o) const     { return ucchar(*this) > o; }
    bool operator >= (const ucchar_ref_basic<_utf8string> & o) const { return ucchar(*this) >= ucchar(o); }
    bool operator >= (const ucchar & o) const     { return ucchar(*this) >= o; }
    bool operator <  (const ucchar_ref_basic<_utf8string> & o) const { return ucchar(*this) < ucchar(o); }
    bool operator <  (const ucchar & o) const     { return ucchar(*this) < o; }
    bool operator <= (const ucchar_ref_basic<_utf8string> & o) const { return ucchar(*this) <= ucchar(o); }
    bool operator <= (const ucchar & o) const     { return ucchar(*this) <= o; }
};

class ucchar_const_ref : public ucchar_ref_basic<const utf8string>
{
public:
	ucchar_const_ref () : ucchar_ref_basic() {};
	ucchar_const_ref (difference_type offset, const utf8string & owner);
	ucchar_const_ref (const ucchar_ref_basic & ref) : ucchar_ref_basic(ref) {}
};

class ucchar_ref : public ucchar_ref_basic<utf8string>
{
public:
	ucchar_ref () : ucchar_ref_basic() {};
	ucchar_ref (difference_type offset, utf8string & owner);
	ucchar_ref (const ucchar_ref_basic & ref) : ucchar_ref_basic(ref) {}
	//ucchar_ref (const ucchar_ref & other) : ucchar_ref_basic(other) {}
	ucchar_ref & operator = (ucchar c);
	//ucchar_ref & operator = (const ucchar_ref & other);
};


template <typename _utf8string, typename _Ref>
class ucchar_ptr_basic
{
	template <typename _Str2, typename _Ref2>
	friend class ucchar_ptr_basic;

	template <typename _Ptr>
	friend class utf8string_iterator;

public:
	typedef typename _Ref::difference_type  difference_type;
	typedef typename _Ref::value_type  value_type;
	typedef _Ref reference;
	typedef _utf8string owner_type;

protected:
	_Ref _ref;

public:
	ucchar_ptr_basic () : _ref() {}
	ucchar_ptr_basic (difference_type offset, _utf8string & owner) : _ref(offset, owner) {}
	ucchar_ptr_basic (_utf8string & s) : _ref(0, s) {}

    template<typename _Str2>
    ucchar_ptr_basic(const ucchar_ref_basic<_Str2> & ref)
    	: _ref(ref) { }

    template<typename _Str2, typename _Ref2>
    ucchar_ptr_basic(const ucchar_ptr_basic<_Str2, _Ref2> & p)
    	: _ref(p._ref) { }

	bool isOutOfBounds () const;

	_Ref operator * () const { return _ref; }
    const _Ref * operator -> () const { return & _ref; }
    //_Ref *       operator -> () { return & _ref; }
	//_Ref         operator * () { return _ref; }

    ucchar_ptr_basic & operator ++ ()
	{
    	return this->operator += (1);
	}

    ucchar_ptr_basic operator ++ (int)
    {
    	ucchar_ptr_basic r(*this);
        this->operator ++ ();
        return r;
    }

    ucchar_ptr_basic & operator -- ()
	{
    	return this->operator -= (1);
	}

    ucchar_ptr_basic operator -- (int)
    {
    	ucchar_ptr_basic r(*this);
        this->operator -- ();
        return r;
    }

    ucchar_ptr_basic operator + (difference_type n) const
    {
    	ucchar_ptr_basic p(*this);
    	p += n;
    	return p;
    }

    ucchar_ptr_basic operator - (difference_type n) const
    {
    	ucchar_ptr_basic p(*this);
    	p -= n;
    	return p;
    }

    ucchar_ptr_basic & operator += (difference_type n);
    ucchar_ptr_basic & operator -= (difference_type n);

    // Random access iterator requirements
    _Ref operator [] (difference_type n) const
    {
    	ucchar_ptr_basic p(*this);
    	p += n;
    	return p._ref;
    }

    difference_type distance (const ucchar_ptr_basic & p) const
    {
       	difference_type d = _ref.ptr() - p._ref.ptr();
        return d < 0 ? d * -1 : d;
    }

    bool operator == (const ucchar_ptr_basic & o) const { return _ref.ptr() + _ref._pos == o._ref.ptr() + o._ref._pos; }
    bool operator != (const ucchar_ptr_basic & o) const { return _ref.ptr() + _ref._pos != o._ref.ptr() + o._ref._pos; }
	bool operator  > (const ucchar_ptr_basic & o) const { return _ref.ptr() + _ref._pos >  o._ref.ptr() + o._ref._pos; }
	bool operator >= (const ucchar_ptr_basic & o) const { return _ref.ptr() + _ref._pos >= o._ref.ptr() + o._ref._pos; }
	bool operator  < (const ucchar_ptr_basic & o) const { return _ref.ptr() + _ref._pos <  o._ref.ptr() + o._ref._pos; }
	bool operator <= (const ucchar_ptr_basic & o) const { return _ref.ptr() + _ref._pos <= o._ref.ptr() + o._ref._pos; }
};


class ucchar_const_ptr : public ucchar_ptr_basic<const utf8string, ucchar_const_ref>
{
	friend class ucchar_ptr;
public:
	typedef ucchar_ptr_basic pointer;
public:
	ucchar_const_ptr () : ucchar_ptr_basic() {}
	ucchar_const_ptr (difference_type offset, const utf8string & owner) : ucchar_ptr_basic(offset, owner) {}
	ucchar_const_ptr (const utf8string & s) : ucchar_ptr_basic(s) {}

	ucchar_const_ptr (const ucchar_ptr_basic & p) : ucchar_ptr_basic(p) {}
	ucchar_const_ptr (const ucchar_ptr & p);
	ucchar_const_ptr (const ucchar_const_ptr & p) : ucchar_ptr_basic(p) {}
};


class ucchar_ptr : public ucchar_ptr_basic<utf8string, ucchar_ref>
{
	friend class ucchar_const_ptr;
public:
	typedef ucchar_ptr_basic pointer;
public:
	ucchar_ptr () : ucchar_ptr_basic() {}
	ucchar_ptr (difference_type offset, utf8string & owner) : ucchar_ptr_basic(offset, owner) {}
	ucchar_ptr (utf8string & s) : ucchar_ptr_basic(s) {}

	ucchar_ptr (const ucchar_ptr_basic & p) : ucchar_ptr_basic(p) {}
	ucchar_ptr (const ucchar_ptr & p) : ucchar_ptr_basic(p) {}

	ucchar_ref   operator * () { return _ref; }
	ucchar_ref * operator -> () { return & _ref; }
};

inline ucchar_const_ptr::ucchar_const_ptr (const ucchar_ptr & p)
	: ucchar_ptr_basic()
{
	_ref._pos = p._ref._pos;
	_ref._owner = p._ref._owner;
}


template <typename _Str1, typename _Ref1, typename _Str2, typename _Ref2>
inline typename ucchar_ptr_basic<_Str1, _Ref1>::difference_type
operator - (const ucchar_ptr_basic<_Str1, _Ref1> & it1, const ucchar_ptr_basic<_Str2, _Ref2> & it2)
{
	return it1.difference(it2);
}

} // pfs

#endif /* __PFS_BITS_UCCHARPTR_HPP__ */
