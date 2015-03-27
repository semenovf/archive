/*
 * vector.hpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */

#ifndef __PFS_VECTOR_HPP__
#define __PFS_VECTOR_HPP__

#include <pfs/pimpl.hpp>
#include <pfs/bits/vector_impl.hpp>
#include <pfs/bits/algorithm.hpp>
#include <pfs/bits/allocator.hpp>

namespace pfs {

template <typename T, typename Alloc = allocator<T> >
class vector : public nullable<vector_impl<T,Alloc> >
{
	friend class vector_impl<T,Alloc>;

protected:
	typedef nullable<vector_impl<T,Alloc> > base_class;
	typedef vector<T, Alloc> self_class;
	typedef vector_impl<T, Alloc> impl_class;

public:
	typedef typename impl_class::value_type             value_type;
	typedef typename impl_class::pointer                pointer;
	typedef typename impl_class::const_pointer          const_pointer;
	typedef typename impl_class::size_type              size_type;
	typedef typename impl_class::difference_type        difference_type;

	typedef pfs::reference<self_class>                  reference;
	typedef pfs::reference<const self_class>            const_reference;
	typedef pfs::random_access_iterator<self_class>     iterator;
	typedef pfs::random_access_iterator<const self_class> const_iterator;
    typedef std::reverse_iterator<iterator>		        reverse_iterator;
    typedef std::reverse_iterator<const_iterator>       const_reverse_iterator;

    typedef value_type             char_type; // used in FSM e.g.

public:
	vector () : base_class() {}
	vector (size_type n, const T & v = T()) : base_class(new impl_class(n, v)) {}
	vector (size_type n, const T v[]);

	template <typename InputIterator>
	vector (InputIterator first, InputIterator last)
	{
		insert(cbegin(), first, last);
	}

	virtual ~vector () {}

public:
	bool isEmpty () const { return base_class::isNull() || base_class::cast()->isEmpty(); }
	bool empty () const { return isEmpty(); }

    iterator begin () { return iterator(this, data()); }
    iterator end   () { return iterator(this, data() + size()); }
    const_iterator begin () const { return cbegin(); }
    const_iterator end   () const { return cend(); }
    const_iterator cbegin() const { return const_iterator(this, data()); }
    const_iterator cend  () const { return const_iterator(this, data() + size()); }
    reverse_iterator rbegin  ()   { return reverse_iterator(end()); }
    reverse_iterator rend    ()   { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin  () const { return crbegin(); }
    const_reverse_iterator rend    () const { return crend(); }
    const_reverse_iterator crbegin () const { return const_reverse_iterator(cend()); }
    const_reverse_iterator crend   () const { return const_reverse_iterator(cbegin()); }

	reference at (size_type index) const
	{
		PFS_ASSERT(index >= 0 && index < size());
		return reference(*const_cast<self_class *>(this), const_cast<pointer>(data() + index));
	}

	reference operator [] (size_type i) const { return at(i); }

	T valueAt (size_type i) const { return at(i); }

	iterator find (const T & value) const;

	reference front () const { return at(0); }
	reference first () const { return at(0); }
	reference back  () const { return at(size()-1); }
	reference last  () const { PFS_ASSERT(size() > 0); return at(size()-1); }

	const T * constData () const { return base_class::isNull() ? nullptr : base_class::cast()->constData(); }
	const T * data () const      { return base_class::isNull() ? nullptr : base_class::cast()->constData(); }
	T * data ()                  { return base_class::isNull() ? nullptr : base_class::cast()->data(); }

	size_type size    () const { return base_class::isNull() ? 0 : base_class::cast()->size(); }
	size_type length  () const { return size(); }
	size_type capacity() const { return base_class::isNull() ? 0 : base_class::cast()->capacity(); }

	bool contains (const vector<T, Alloc> & v) const { return pfs::search(cbegin(), cend(), v.cbegin(), v.cend()) != cend(); }
	bool contains (const T & value) const   { return pfs::find(cbegin(), cend(), value) != cend(); }
	bool startsWith (const T & value) const { return size() > 0 ? at(0) == value : false; }
	bool startsWith (const vector<T, Alloc> & v) const { return pfs::search(cbegin(), cend(), v.cbegin(), v.cend()) == cbegin(); }
	bool endsWith (const T & value) const   { return size() > 0 ? at(size()-1) == value : false; }
	bool endsWith (const vector<T, Alloc> & v) const
	{
		// Ends with null (empty) vector
		if (!v.size())
			return true;
		return size() >= v.size() ?
			pfs::search(cbegin(), cend(), v.cbegin(), v.cend()) == (cend() - v.size())
			: false;
	}

	void clear ()
	{
		if (!isEmpty()) {
			base_class::detach();
			base_class::cast()->clear();
		}
	}
    void reserve (size_type n) { base_class::detach(); base_class::cast()->reserve(n); }
    void resize  (size_type n) { base_class::detach(); base_class::cast()->resize(n, value_type()); }
    void resize  (size_type n, const value_type & value) { base_class::detach(); base_class::cast()->resize(n, value); }
    void shrink_to_fit (); // introduced in C++11

	iterator insert (const_iterator pos, size_type count, const T & value);
	iterator insert (const_iterator pos, const T & value)
	{
		return insert(pos, 1, value);
	}

	template <typename InputIterator>
	iterator insert (const_iterator pos, InputIterator first, InputIterator last);

	iterator erase (const_iterator pos);
	iterator erase (const_iterator first, const_iterator last);
	iterator remove (const_iterator pos) { return erase(pos); }
	iterator remove (const_iterator first, const_iterator last) { return erase(first, last); }
	iterator remove (size_type index) { return erase(cbegin() + index); }
	iterator remove (size_type index, size_t n) { return erase(cbegin() + index, cbegin() + index + n); }
	void pop_back () { if(size() > 0) erase(cend() - 1); }
	void pop_front () { if(size() > 0) erase(cbegin()); }

	void push_back (const T & value) { insert(cend(), value); }
	void append    (const T & value) { insert(cend(), value); }
	void append    (size_type n, const T & value)  { insert(cend(), n, value); }
	void append    (const vector & other) { insert(cend(), other.cbegin(), other.cend()); }

	void push_front (const T & value) { insert(cbegin(), value); }
	void prepend    (const T & value) { insert(cbegin(), value); }
	void prepend    (size_type n, const T & value)  { insert(cbegin(), n, value); }
	void prepend    (const vector & other) { insert(cbegin(), other.cbegin(), other.cend()); }

	vector	 operator +  (const vector & o) const { vector r(*this); r.append(o); return r; }
	vector & operator += (const vector & o) { append(o); return *this; }
	vector & operator += (const T & value)  { append(value); return *this; }
	vector & operator << (const T & value)  { append(value); return *this; }
	vector & operator << (const vector & o) { append(o); return *this; }

	void detach_and_assign (T * & p, const T & value); // pfs::reference class requirement
};


template <typename T, typename Alloc>
vector<T,Alloc>::vector (size_type n, const T v[])
	: base_class(new impl_class)
{
	for (size_type i = 0; i < n; ++i) {
		append(v[i]);
	}
}


template <typename T, typename Alloc>
void vector<T, Alloc>::detach_and_assign (T * & p, const T & value)
{
	T * first = data();
	difference_type offset = p - first;
	base_class::detach();
	p = data() + offset;
	*p = value;
}

template <typename T, class Alloc>
inline bool operator == (const vector<T, Alloc> & lhs, const vector<T, Alloc> & rhs)
{
	return compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend()) == 0;
}

template <typename T, class Alloc>
inline bool operator != (const vector<T, Alloc> & lhs, const vector<T, Alloc> & rhs)
{
	return compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend()) != 0;
}

template <typename T, class Alloc>
inline bool operator < (const vector<T, Alloc> & lhs, const vector<T, Alloc> & rhs)
{
	return compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend()) < 0;
}

template <typename T, class Alloc>
inline bool operator <= (const vector<T, Alloc> & lhs, const vector<T, Alloc> & rhs)
{
	return compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend()) <= 0;
}

template <typename T, class Alloc>
inline bool operator > (const vector<T, Alloc> & lhs, const vector<T, Alloc> & rhs)
{
	return compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend()) > 0;
}

template <typename T, class Alloc>
inline bool operator >= (const vector<T, Alloc> & lhs, const vector<T, Alloc> & rhs)
{
	return compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend()) >= 0;
}

////template<typename T, class Alloc>
////typename vector<T, Alloc>::const_iterator
////vector<T, Alloc>::find (typename vector<T, Alloc>::const_iterator pos, const vector<T, Alloc> & v) const
////{
////	return const_iterator(std::search(pos, cend(), v.cbegin(), v.cend()));
////}
////
////template<typename T, class Alloc>
////typename vector<T, Alloc>::const_iterator
////vector<T, Alloc>::find (typename vector<T, Alloc>::const_iterator pos, size_t len, const vector<T, Alloc> & v) const
////{
////	return const_iterator(std::search(pos, pos + len, v.cbegin(), v.cend()));
////}
////
////template<typename T, class Alloc>
////typename vector<T, Alloc>::const_iterator
////vector<T, Alloc>::find (size_t pos, const typename vector<T, Alloc>::value_type * v, size_t n) const
////{
////	return const_iterator(std::search(cbegin() + pos, cend(), v, v + n));
////}
////
////template<typename T, class Alloc>
////typename vector<T, Alloc>::const_iterator
////vector<T, Alloc>::find (size_t pos, size_t len, const typename vector<T, Alloc>::value_type * v, size_t n) const
////{
////	return const_iterator(std::search(cbegin() + pos, cbegin() + pos + len, v, v + n));
////}
////
////template<typename T, class Alloc>
////typename vector<T, Alloc>::const_iterator
////vector<T, Alloc>::find (size_t pos, const T & val) const
////{
////	return vector<T, Alloc>::const_iterator(std::find(cbegin() + pos, cend(), val));
////}

} // pfs

#include "bits/vector_impl_inc.hpp"

#endif /* __PFS_VECTOR_HPP__ */
