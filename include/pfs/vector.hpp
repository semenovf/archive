/*
 * vector.hpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */

#ifndef __PFS_VECTOR_HPP__
#define __PFS_VECTOR_HPP__

#include <vector>
#include <algorithm>
#include <pfs/pimpl.hpp>

namespace pfs {

template <typename T, typename Alloc>
class vector;

template <typename T, typename Alloc>
class item_ref
{
	friend class vector<T, Alloc>;

	typedef vector<T, Alloc> vector_type;
	typedef T value_type;

	vector<T, Alloc> & _a;
    size_t _i;

    item_ref (vector_type & a, size_t index)
        : _a(a), _i(index) {}

public:
    item_ref (const item_ref & o) : _a(o._a), _i(o._i) {}

    value_type * operator -> ()
    {
    	_a._d.detach();
    	return & _a.refAt(_i);
    }

    const value_type * operator -> () const
    {
    	return & _a.refAt(_i);
    }

    operator value_type () const
    {
    	return _i < _a.size()
    			? _a.valueAt(_i)
    			: value_type();
    }

    item_ref & operator = (const value_type & v)
    {
    	PFS_ASSERT(_i < _a.size());
    	_a._d.detach();
    	_a.refAt(_i) = v;
        return *this;
    }

    item_ref & operator = (const item_ref & v)
    {
    	PFS_ASSERT(_i < _a.size());
    	_a._d.detach();
        _a.refAt(_i) = v._a.valueAt(v._i);
        return *this;
    }

    bool operator == (const value_type & v) const { return _a.valueAt(_i) == v; }
    bool operator != (const value_type & v) const { return _a.valueAt(_i) != v; }
    bool operator >  (const value_type & v) const { return _a.valueAt(_i) >  v; }
    bool operator >= (const value_type & v) const { return _a.valueAt(_i) >= v; }
    bool operator <  (const value_type & v) const { return _a.valueAt(_i) <  v; }
    bool operator <= (const value_type & v) const { return _a.valueAt(_i) <= v; }
};


template <typename T, typename Alloc = std::allocator<T> >
class vector
{
	friend class item_ref<T,Alloc>;

protected:
	typedef std::vector<T, Alloc> impl;
	pimpl _d;

public:
	typedef T item_type;
	typedef T value_type;
	typedef typename impl::iterator iterator;
	typedef typename impl::const_iterator const_iterator;
	typedef typename impl::reference reference;
	typedef typename impl::const_reference const_reference;

	typedef item_ref<T,Alloc> item_ref_type;

protected:
	const impl & cast () const
	{
		return *pfs::pimpl_lazy_init<impl>(_d);
	}

	impl & cast ()
	{
		return *pfs::pimpl_lazy_init<impl>(_d);
	}

	const T & valueAt (size_t i) { PFS_ASSERT(i < size()); return cast().operator [] (i); }
	T & refAt (size_t i) { PFS_ASSERT(i < size()); return cast().operator [] (i); }

public:
	vector () : _d() {}
	vector (size_t n, const T & v = T()) : _d(new impl(n, v)) {}
	vector (const T * values, size_t n) : _d(new impl) { append(values, n); }
	virtual ~vector () {}

public:
	bool isNull () const  { return _d.isNull(); }
	bool isEmpty () const { return _d.isNull() || cast().empty(); }

    iterator       begin ()       { _d.detach(); return cast().begin(); }
    iterator       end   ()       { _d.detach(); return cast().end(); }
    const_iterator begin () const { return cast().begin(); }
    const_iterator end   () const { return cast().end(); }
    const_iterator cbegin() const { return begin(); }
    const_iterator cend  () const { return end(); }

	const T &     at (size_t i) const { PFS_ASSERT(i < size()); return cast().operator [] (i); }
	const T &     operator [] (size_t i) const { return at(i); }

	item_ref_type at (size_t i) { PFS_ASSERT(i < size()); return item_ref_type(*this, i); }
	item_ref_type operator [] (size_t i) { return at(i); }

	reference       front ()       { return cast().front(); }
	const_reference front () const { return cast().front(); }
	reference       first ()       { return cast().front(); }
	const_reference first () const { return cast().front(); }
	reference       back  ()       { return cast().back(); }
	const_reference back  () const { return cast().back(); }
	reference       last  ()       { return cast().back(); }
	const_reference last  () const { return cast().back(); }

	T *       data ()            { _d.detach(); return cast().data(); }
	const T * data () const      { return cast().data(); }
	const T * constData () const { return cast().data(); }

	size_t size    () const { return isNull() ? 0 : cast().size(); }
	size_t length  () const { return size(); }
	size_t capacity() const { return isNull() ? 0 : cast().capacity(); }

	int compare (const vector<T, Alloc> & v) const { return compare(0, size(), v, 0, v.size()); }
	int compare (size_t pos, size_t len, const vector<T, Alloc> & v) const {  return compare(pos, len, v, 0, v.size());  }
	int compare (size_t pos, size_t len, const vector<T, Alloc> & v, size_t subpos, size_t sublen) const;
	int compare (size_t pos, size_t len, const T * v, size_t n) const;
	int compare (const T * v, size_t n) const { return compare(0, size(), v, n); }

	const_iterator find (const_iterator pos, const vector<T, Alloc> & v) const;
	const_iterator find (const_iterator pos, size_t len, const vector<T, Alloc> & v) const;
	const_iterator find (size_t pos, const value_type * v, size_t n) const;
	const_iterator find (size_t pos, size_t len, const value_type * v, size_t n) const;
	const_iterator find (size_t pos, const T & val) const;

	bool contains (const vector<T, Alloc> & v) const { return find(begin(), v) != cend(); }
	bool contains (const T * v, size_t n) const { return find(0, v, n) != cend(); }

    void swap (vector & o) { _d.swap<impl>(o._d); }
    void reserve (size_t n) { _d.detach(); cast().reserve(n); }

    virtual void resize (size_t n, T v = T()) { _d.detach(); cast().resize(n, v); }
	virtual void clear () { _d.detach(); cast().clear(); }
	virtual void insert (iterator position, const T * ptr, size_t n)
	{
		_d.detach();
		cast().insert(position, ptr, ptr + n);
	}
	virtual void insert (iterator position, size_t n, const T & val)
	{
		_d.detach();
		cast().insert(position, n, val);
	}
    virtual iterator erase (iterator first, iterator last) { _d.detach(); return cast().erase(first, last); }

	void insert (iterator position, const T & val) { insert(position, 1, val); }
	void insert (iterator position, const vector<T,Alloc> & val) { insert(position, val.data(), val.size()); }
	void append  (const T & value)               { insert(end(), 1, value); }
	void append  (size_t n, const T & value)     { insert(end(), n, value); }
	void append  (const T * ptr, size_t n)       { insert(end(), ptr, n); }
	void append  (const vector<T,Alloc> & other) { append(other.constData(), other.size()); }
	void prepend (const T & value)               { insert(begin(), 1, value); }
	void prepend (size_t n, const T & value)     { insert(begin(), n, value); }
	void prepend (const T * values, size_t n)    { insert(begin(), values, n); }
	void prepend (const vector<T,Alloc> & other) { insert(begin(), other.constData(), other.size()); }

    iterator erase (iterator pos) { return erase(begin() + pos, begin() + pos + 1); }
    void     erase (size_t index) { erase(begin() + index); }
    void     erase (size_t index, size_t n) { erase(begin() + index, begin() + index + n); }

    iterator remove (iterator pos) { return erase(pos); }
    iterator remove (iterator first, iterator last) { return erase(first, last); }
    void     remove (size_t index) { erase(index); }
    void     remove (size_t index, size_t n) { erase(index, n); }


//    friend bool operator == (const vector<T, Alloc> & lhs, const vector<T, Alloc> & rhs)
//    {
//    	return *(lhs._d.cast<vector<T, Alloc>::impl>())
//    			==
//    			*(rhs._d.cast<vector<T, Alloc>::impl>());
//    }
//    friend bool operator != (const vector<T, Alloc> & lhs, const vector<T, Alloc> & rhs)
//    		{ return *lhs._d.cast<impl>() != *rhs._d.cast<impl>(); }
//    friend bool operator <  (const vector<T, Alloc> & lhs, const vector<T, Alloc> & rhs)
//    	{ return *lhs._d.cast<impl>() <  *rhs._d.cast<impl>(); }
//    friend bool operator <= (const vector<T, Alloc> & lhs, const vector<T, Alloc> & rhs)
//    		{ return *lhs._d.cast<impl>() <= *rhs._d.cast<impl>(); }
//    friend bool operator >  (const vector<T, Alloc> & lhs, const vector<T, Alloc> & rhs)
//    	{ return *lhs._d.cast<impl>() >  *rhs._d.cast<impl>(); }
//    friend bool operator >= (const vector<T, Alloc> & lhs, const vector<T, Alloc> & rhs)
//    		{ return *lhs._d.cast<impl>() >= *rhs._d.cast<impl>(); }
};

template <typename T, class Alloc>
int vector<T, Alloc>::compare (size_t pos, size_t len, const T * v, size_t n) const
{
	const T * p0 = constData() + pos;
	const T * p1 = v;
	const T * pend0 = p0 + len;
	const T * pend1 = v + n;

	for (; p0 < pend0 && p1 < pend1; ++p0, ++p1) {
		if (*p0 > *p1 )
			return 1;

		if (*p0 < *p1 )
			return -1;
	}

	return (p0 == pend0)
			? (p1 == pend1) ? 0 : -1
			: 1;
}

template <typename T, class Alloc>
inline int vector<T, Alloc>::compare (size_t pos, size_t len, const vector<T, Alloc> & v, size_t subpos, size_t sublen) const
{
	return compare(pos, len, v.constData() + subpos, sublen);
}

template<typename T, class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::find (vector<T, Alloc>::const_iterator pos, const vector<T, Alloc> & v) const
{
	return const_iterator(std::search(cbegin() + pos, cend(), v.cbegin(), v.cend()));
}

template<typename T, class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::find (vector<T, Alloc>::const_iterator pos, size_t len, const vector<T, Alloc> & v) const
{
	return const_iterator(std::search(cbegin() + pos, cbegin() + pos + len, v.cbegin(), v.cend()));
}

template<typename T, class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::find (size_t pos, const vector<T, Alloc>::value_type * v, size_t n) const
{
	return const_iterator(std::search(cbegin() + pos, cend(), v, v + n));
}

template<typename T, class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::find (size_t pos, size_t len, const vector<T, Alloc>::value_type * v, size_t n) const
{
	return const_iterator(std::search(cbegin() + pos, cbegin() + pos + len, v, v + n));
}

template<typename T, class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::find (size_t pos, const T & val) const
{
	return vector<T, Alloc>::const_iterator(std::find(cbegin() + pos, cend(), val));
}

} // cwt

#endif /* __PFS_VECTOR_HPP__ */
