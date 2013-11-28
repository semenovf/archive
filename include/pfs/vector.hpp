/*
 * vector.hpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */

#ifndef __PFS_VECTOR_HPP__
#define __PFS_VECTOR_HPP__

#include <vector>
#include <pfs/shared_ptr.hpp>

namespace pfs {

template <typename T>
class vector
{
private:
	typedef std::vector<T> impl;
	shared_ptr<impl> _pimpl;

	void detach()
	{
		if (!_pimpl.unique()) {
			shared_ptr<impl> d(new impl(*_pimpl));
			_pimpl.swap(d);
		}
	}
	vector (const impl & other) : _pimpl(new impl(other)) {}

public:
	vector (const vector & other) : _pimpl(other._pimpl) { }
	vector & operator = (const vector & other)
	{
		_pimpl = other._pimpl;
		return *this;
	}

public:
	typedef typename impl::iterator iterator;
	typedef typename impl::const_iterator const_iterator;

public:
	vector () : _pimpl(new impl()) {}
	explicit vector (size_t n, const T & v = T()) : _pimpl(new impl(n, v)) {}
	explicit vector (const T * values, size_t size);

    iterator       begin () { detach(); return _pimpl->begin(); }
    iterator       end   () { detach(); return _pimpl->end(); }
    const_iterator begin () const { return _pimpl->begin(); }
    const_iterator end   () const { return _pimpl->end(); }
    const_iterator cbegin() const { return begin(); }
    const_iterator cend  () const { return end(); }

	const T &    at (size_t i) const { return _pimpl->operator [] (i); }
	T &          operator [] (size_t i) { detach(); return _pimpl->operator [] (i); }
	const T &    operator [] (size_t i) const { return at(i); }

	T *       data ()            { detach(); return _pimpl->data(); }
	const T * data () const      { return _pimpl->data(); }
	const T * constData () const { return _pimpl->data(); }
	void      clear ()           { detach(); _pimpl->clear(); }
	bool      isEmpty () const   { return _pimpl->empty(); }
	size_t    size () const      { return _pimpl->size(); }
	size_t    length () const    { return size(); }

	void append (const T & value) { detach(); _pimpl->push_back(value); }
	void append (const T * values, size_t count);
	void append (const vector<T> & other) { append(other.constData(), other.size()); }

    void resize(size_t n, T v = T()) { detach(); _pimpl->resize(n, v); }
    void swap (vector<T> & other) { pfs::swap(_pimpl, other._pimpl); }

};

template <typename T>
vector<T>::vector (const T * values, size_t count)
	: _pimpl(new impl)
{
	size_t i = size();
	_pimpl->resize(size() + count);
	T * d = data();
	for (size_t j = 0; j < count; ++i, ++j) {
		d[i] = values[j];
	}
}

template <typename T>
void vector<T>::append (const T * values, size_t count)
{
	detach();
	size_t i = size();
	_pimpl->resize(size() + count);
	T * d = data();
	for (size_t j = 0; j < count; ++i, ++j) {
		d[i] = values[j];
	}
}

} // cwt

#endif /* __PFS_VECTOR_HPP__ */
