/*
 * vector.hpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */

#ifndef __PFS_VECTOR_HPP__
#define __PFS_VECTOR_HPP__

#include <vector>
#include <pfs/pimpl.hpp>

namespace pfs {

template <typename T>
class vector
{
	typedef std::vector<T> impl;
	pimpl _d;

public:
	typedef T item_type;
	typedef typename impl::iterator iterator;
	typedef typename impl::const_iterator const_iterator;
	typedef typename impl::reference reference;
	typedef typename impl::const_reference const_reference;

public:
	vector () : _d(new impl()) {}
	explicit vector (size_t n, const T & v = T()) : _d(new impl(n, v)) {}
	explicit vector (const T * values, size_t size);

    iterator       begin () { _d.detach(); return _d.cast<impl>()->begin(); }
    iterator       end   () { _d.detach(); return _d.cast<impl>()->end(); }
    const_iterator begin () const { return _d.cast<impl>()->begin(); }
    const_iterator end   () const { return _d.cast<impl>()->end(); }
    const_iterator cbegin() const { return begin(); }
    const_iterator cend  () const { return end(); }

	const T &    at (size_t i) const { return _d.cast<impl>()->operator [] (i); }
	T &          at (size_t i) { _d.detach(); return _d.cast<impl>()->operator [] (i); }
	const T &    operator [] (size_t i) const { return at(i); }
	T &          operator [] (size_t i) { return at(i); }

	reference       front ()       { return _d.cast<impl>()->front(); }
	const_reference front () const { return _d.cast<impl>()->front(); }
	reference       first ()       { return _d.cast<impl>()->front(); }
	const_reference first () const { return _d.cast<impl>()->front(); }
	reference       back  ()       { return _d.cast<impl>()->back(); }
	const_reference back  () const { return _d.cast<impl>()->back(); }
	reference       last  ()       { return _d.cast<impl>()->back(); }
	const_reference last  () const { return _d.cast<impl>()->back(); }

	T *       data ()            { _d.detach(); return _d.cast<impl>()->data(); }
	const T * data () const      { return _d.cast<impl>()->data(); }
	const T * constData () const { return _d.cast<impl>()->data(); }
	void      clear ()           { _d.detach(); _d.cast<impl>()->clear(); }
	bool      isEmpty () const   { return _d.cast<impl>()->empty(); }
	size_t    size () const      { return _d.cast<impl>()->size(); }
	size_t    length () const    { return size(); }

	void append (const T & value) { _d.detach(); _d.cast<impl>()->push_back(value); }
	void append (const T * values, size_t count);
	void append (const vector<T> & other) { append(other.constData(), other.size()); }

    void resize(size_t n, T v = T()) { _d.detach(); _d.cast<impl>()->resize(n, v); }

    iterator erase (iterator pos) { _d.detach(); return _d.cast<impl>()->erase(pos); }
    iterator erase (iterator first, const_iterator last) { _d.detach(); return _d.cast<impl>()->erase(first, last); }
    void erase (size_t index) { _d.detach(); _d.cast<impl>()->erase(begin() + index); }
    void erase (size_t index, size_t n) { _d.detach(); _d.cast<impl>()->erase(begin() + index, begin() + index + n); }

    iterator remove (iterator pos) { return erase(pos); }
    iterator remove (iterator first, iterator last) { return erase(first, last); }
    void remove (size_t index) { erase(index); }
    void remove (size_t index, size_t n) { erase(index, n); }

    void swap (vector & o) { _d.swap<impl>(o._d); }
};

template <typename T>
vector<T>::vector (const T * values, size_t count)
	: _d(new impl)
{
	size_t i = size();
	_d.cast<impl>()->resize(size() + count);
	T * d = data();
	for (size_t j = 0; j < count; ++i, ++j) {
		d[i] = values[j];
	}
}

template <typename T>
void vector<T>::append (const T * values, size_t count)
{
	_d.detach();
	size_t i = size();
	_d.cast<impl>()->resize(size() + count);
	T * d = data();
	for (size_t j = 0; j < count; ++i, ++j) {
		d[i] = values[j];
	}
}

} // cwt

#endif /* __PFS_VECTOR_HPP__ */
