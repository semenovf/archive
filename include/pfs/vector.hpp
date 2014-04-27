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

    inline operator value_type () const
    {
    	return _i < _a.size()
    			? _a.__valueAt(_i)
    			: value_type();
    }

    inline value_type * operator -> () const
    {
    	PFS_ASSERT(_i < _a.size());
    	return & _a.__refAt(_i);
    }


    item_ref & operator = (const value_type & v)
    {
    	PFS_ASSERT(_i < _a.size());
    	_a._d.detach();
    	_a.__refAt(_i) = v;
        return *this;
    }

    item_ref & operator = (const item_ref & v)
    {
    	PFS_ASSERT(_i < _a.size());
    	_a._d.detach();
        _a.__refAt(_i) = v._a.__valueAt(v._i);
        return *this;
    }

    bool operator == (const value_type & v) const { return _a.__valueAt(_i) == v; }
    bool operator != (const value_type & v) const { return _a.__valueAt(_i) != v; }
    bool operator >  (const value_type & v) const { return _a.__valueAt(_i) >  v; }
    bool operator >= (const value_type & v) const { return _a.__valueAt(_i) >= v; }
    bool operator <  (const value_type & v) const { return _a.__valueAt(_i) <  v; }
    bool operator <= (const value_type & v) const { return _a.__valueAt(_i) <= v; }
};

template <typename T, typename Alloc = std::allocator<T> >
class vector : public pimpl_lazy_init<std::vector<T, Alloc> >
{
	friend class item_ref<T,Alloc>;

protected:
	typedef std::vector<T, Alloc> impl;
	typedef pimpl_lazy_init<std::vector<T, Alloc> > base_class;

public:
	typedef T item_type;
	typedef T value_type;
	typedef typename impl::iterator iterator;
	typedef typename impl::const_iterator const_iterator;
	typedef typename impl::reference reference;
	typedef typename impl::const_reference const_reference;

	typedef item_ref<T,Alloc> item_ref_type;

	const T & __valueAt (size_t i) { PFS_ASSERT(i < size()); return base_class::cast()->operator [] (i); }
	T & __refAt (size_t i) { PFS_ASSERT(i < size()); return base_class::cast()->operator [] (i); }

public:
	vector () : base_class() {}
	vector (size_t n, const T & v = T()) : base_class(new impl(n, v)) {}
	vector (const T * values, size_t n) : base_class(new impl) { append(values, n); }
	virtual ~vector () {}

public:
	bool isEmpty () const { return base_class::isNull() || base_class::cast()->empty(); }

    iterator       begin ()       { base_class::detach(); return base_class::cast()->begin(); }
    iterator       end   ()       { base_class::detach(); return base_class::cast()->end(); }
    const_iterator begin () const { return base_class::cast()->begin(); }
    const_iterator end   () const { return base_class::cast()->end(); }
    const_iterator cbegin() const { return begin(); }
    const_iterator cend  () const { return end(); }

	const T &     at (size_t i) const { PFS_ASSERT(i < size()); return base_class::cast()->operator [] (i); }
	const T &     operator [] (size_t i) const { return at(i); }

	item_ref_type at (size_t i) { PFS_ASSERT(i < size()); return item_ref_type(*this, i); }
	item_ref_type operator [] (size_t i) { return at(i); }

	reference       front ()       { return base_class::cast()->front(); }
	const_reference front () const { return base_class::cast()->front(); }
	reference       first ()       { return base_class::cast()->front(); }
	const_reference first () const { return base_class::cast()->front(); }
	reference       back  ()       { return base_class::cast()->back(); }
	const_reference back  () const { return base_class::cast()->back(); }
	reference       last  ()       { return base_class::cast()->back(); }
	const_reference last  () const { return base_class::cast()->back(); }

	T *       data ()            { base_class::detach(); return base_class::cast()->data(); }
	const T * data () const      { return base_class::cast()->data(); }
	const T * constData () const { return base_class::cast()->data(); }

	size_t size    () const { return base_class::cast()->size(); }
	size_t length  () const { return size(); }
	size_t capacity() const { return base_class::cast()->capacity(); }

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

    void swap (vector & o) { base_class::swap(o); }
    void reserve (size_t n) { base_class::detach(); base_class::cast()->reserve(n); }

    virtual void resize (size_t n, T v = T()) { base_class::detach(); base_class::cast()->resize(n, v); }
	virtual void clear () { base_class::detach(); base_class::cast()->clear(); }
	virtual void insert (iterator position, const T * ptr, size_t n)
	{
		base_class::detach();
		base_class::cast()->insert(position, ptr, ptr + n);
	}
	virtual void insert (iterator position, size_t n, const T & val)
	{
		base_class::detach();
		base_class::cast()->insert(position, n, val);
	}
    virtual iterator erase (iterator first, iterator last) { base_class::detach(); return base_class::cast()->erase(first, last); }

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

    //iterator erase (iterator pos) { return erase(begin() + pos, begin() + pos + 1); }
    void     erase (size_t index) { erase(index, 1); }
    void     erase (size_t index, size_t n) { erase(begin() + index, begin() + index + n); }

    //iterator remove (iterator pos) { return erase(pos); }
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
vector<T, Alloc>::find (typename vector<T, Alloc>::const_iterator pos, const vector<T, Alloc> & v) const
{
	return const_iterator(std::search(pos, cend(), v.cbegin(), v.cend()));
}

template<typename T, class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::find (typename vector<T, Alloc>::const_iterator pos, size_t len, const vector<T, Alloc> & v) const
{
	return const_iterator(std::search(pos, pos + len, v.cbegin(), v.cend()));
}

template<typename T, class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::find (size_t pos, const typename vector<T, Alloc>::value_type * v, size_t n) const
{
	return const_iterator(std::search(cbegin() + pos, cend(), v, v + n));
}

template<typename T, class Alloc>
typename vector<T, Alloc>::const_iterator
vector<T, Alloc>::find (size_t pos, size_t len, const typename vector<T, Alloc>::value_type * v, size_t n) const
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
