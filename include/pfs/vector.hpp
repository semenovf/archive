/*
 * vector.hpp
 *
 *  Created on: Nov 21, 2013
 *      Author: wladt
 */

#ifndef __PFS_VECTOR_HPP__
#define __PFS_VECTOR_HPP__

#include <vector>

namespace pfs {

template <typename T>
class vector : private std::vector<T>
{
	typedef std::vector<T> base_class;

public:
	typedef typename base_class::iterator iterator;
	typedef typename base_class::const_iterator const_iterator;

public:
	vector () : base_class() {}

	T &            at (size_t i) { return base_class::operator [] (i); }
	const T &      at (size_t i) const { return base_class::operator [] (i); }
	T &            operator[](size_t i)       { return base_class::operator [] (i); }
	const T &      operator[](size_t i) const { return base_class::operator [] (i); }

	T *       data()            { return base_class::data(); }
	const T * data() const      { return base_class::data(); }
	const T * constData() const { return base_class::data(); }
	void      clear()           { base_class::clear(); }
	bool      isEmpty() const   { return base_class::empty(); }
	size_t size() const         { return base_class::size(); }
	size_t length() const       { return size(); }

	void append(const T & value) { base_class::push_back(value); }
	void append(const T * value, size_t count);
	void append(const vector<T> & other) { append(other.constData(), other.size()); }
};

template <typename T>
void vector<T>::append(const T * value, size_t count)
{
	T * d = data();
	base_class::reserve(size() + count);

	size_t i = size();
	for (size_t j = 0; j < count; ++i, ++j) {
		d[i] = value[j];
	}
	base_class::resize(i);
}

} // cwt

#endif /* __PFS_VECTOR_HPP__ */
