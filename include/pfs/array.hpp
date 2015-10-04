/**
 * @file   array.hpp
 * @author wladt
 * @date   Feb 18, 2013 4:25:32 PM
 *
 * @brief
 */

#ifndef __PFS_ARRAY_HPP__
#define __PFS_ARRAY_HPP__

#include <pfs.hpp>
#include <pfs/utility.hpp>
#include <cstring>

namespace pfs {

template <typename T>
class array
{
	size_t   _capacity;
	uint16_t _raw : 1; /* is raw data */
	T *      _head;

public:
	array () : _capacity(0), _raw(0), _head(nullptr) {}
	array (T a[], size_t n);
	array (size_t sz, bool zero = false);
	~array ();

	array (const array & other);
	array &	operator = (const array & other);

	void        alloc (size_t capacity);
	T &         at (size_t index) { PFS_ASSERT(index < _capacity); return _head[index]; }
	const T &   at (size_t index) const { PFS_ASSERT(index < _capacity); return _head[index]; }
	int         compare (size_t pos, const array & other, size_t subpos, size_t len) const;
	int         compare (const array & other, size_t len) const { return compare(0, other, 0, len); }
	int         compare (const array & other) const { return compare(0, other, 0, other._capacity); }
	void        bzero ()      { if(_head) ::memset(_head, 0, _capacity * sizeof(T)); }
	void        set   (int c) { if(_head) ::memset(_head, c, _capacity * sizeof(T)); }

	void        set   (int c, size_t off)
	{
		PFS_ASSERT(off < _capacity);
		if(_head)
			memset(_head + off, c, (_capacity - off) * sizeof(T));
	}

	array *     clone () const;
	T *         data  ()       { return _head; }
	const T *   data  () const { return _head; }
	const T *   constData () const { return _head; }
	bool        eq    (const array & a);
	bool        isRaw() const { return _raw; }
	void        move  (size_t off_to, size_t off_from, size_t n);
	void        realloc (size_t new_capacity);
	void        resize (size_t new_capacity) { realloc(new_capacity); }
	size_t      size  () const { return _capacity; }
	size_t      capacity() const { return _capacity; }
	void        swap(array<T> & other);

	T &         operator [] (size_t index) { return at(index); }
	const T &   operator [] (size_t index) const { return at(index); }

	void copyFrom (const array & from, size_t off_from, size_t n)
	{
		copy(*this, from, 0, off_from, n);
	}

	void copyFrom (const array & from, size_t n)
	{
		copy(*this, from, 0, 0, n);
	}

	void copyFrom (const array & from)
	{
		copy(*this, from, 0, 0, from._capacity);
	}

	void copyFrom (const T * from, size_t n)
	{
		copy (*this, from, 0, 0, n);
	}

	void copyTo (array & to, size_t off_to, size_t n) const
	{
		copy(to, *this, off_to, 0, n);
	}

	void copyTo (const array & to, size_t n)
	{
		copy(to, *this, 0, 0, n);
	}

	void copyTo (const array & to)
	{
		copy(to, *this, 0, 0, _capacity);
	}

	void copyTo (T * to, size_t n)
	{
		copy(to, *this, 0, n);
	}

	static void copy (array & to, const array & from, size_t off_to, size_t off_from, size_t n);
	static void copy (array & to, const T * from, size_t off_to, size_t n);
	static void copy (T * to, const array & from, size_t off_to, size_t n);

	static void deep_copy (T * to, const T * from, size_t n);
	static void deep_copy (array & to, const array & from, size_t off_to, size_t off_from, size_t n);
	static void deep_copy (array & to, const T * from, size_t off_to, size_t n);
};

template <typename T>
inline array<T>::array (T a[], size_t n)
	: _capacity(0), _raw(1), _head(nullptr)
{
	if (a) {
		_capacity = n;
		_head = & a[0];
	}
}

template <typename T>
inline array<T>::array (const array & other)
	: _capacity(other._capacity), _raw(0), _head(nullptr)
{
	if (other._capacity > 0) {
		alloc(other._capacity);
		deep_copy(*this, other, 0, 0, other._capacity);
	}
}

template <typename T>
inline array<T> & array<T>::operator = (const array & other)
{
	if (other._capacity > 0) {
		alloc(other._capacity);
		deep_copy(*this, other, 0, 0, other._capacity);
	}
	return *this;
}

template <typename T>
inline array<T>::array (size_t sz, bool zero)
	: _capacity(0), _raw(0), _head(nullptr)
{
	if (sz) {
		alloc(sz);

		if (zero) {
			bzero();
		}
	}
}

template <typename T>
inline array<T>::~array ()
{
	if(!_raw)
		delete[] _head;
}

template <typename T>
inline void array<T>::alloc (size_t size)
{
	if (!_raw && _head) {
		delete[] _head;
	}
	_head = new T[size];
	_raw = 0;
	_capacity = size;
}

template <typename T>
inline int array<T>::compare (size_t pos, const array & other, size_t subpos, size_t len) const
{
	PFS_ASSERT(pos < _capacity && subpos < other._capacity);
	return memcmp(_head + pos, other._head + subpos, len * sizeof(T));
}

template <typename T>
array<T>* array<T>::clone () const
{
	array<T> * clone = new array<T>(_capacity);
	if (clone)
		copy(*clone, *this, 0, 0, _capacity);
	return clone;
}

/**
* @brief Copy data from array @c from to array @c to.
*
* @details Actually copied minimum number of items
* 	calculated as MIN(from->size, to->size, n).
* 	Items' sizes of arrays must be equal.
*
* @param to Destination array.
* @param from Source array.
* @param off_to Offset in destination array.
* @param off_from Offset in source array.
* @param n Number of items to copy.
* @return Actually items copied.
*/
template <typename T>
void array<T>::copy (array & to, const array & from, size_t off_to, size_t off_from, size_t n)
{
	n = pfs_min(from._capacity - off_from, n);
	n = pfs_min(to._capacity - off_to, n);
	::memcpy(to._head + off_to, from._head + off_from, n * sizeof(T));
}

template <typename T>
void array<T>::copy (array & to, const T * from, size_t off_to, size_t n)
{
	n = pfs_min(to._capacity - off_to, n);
	::memcpy(to._head + off_to, from, n * sizeof(T));
}

template <typename T>
void array<T>::copy (T * to, const array & from, size_t off_from, size_t n)
{
	n = pfs::min(from._capacity - off_from, n);
	::memcpy(to, from._head + off_from, n * sizeof(T));
}

template <typename T>
void array<T>::deep_copy (T * to, const T * from, size_t n)
{
	while (n--) {
		*to++ = *from++;
	}
}

template <typename T>
void array<T>::deep_copy (array & to, const array & from, size_t off_to, size_t off_from, size_t n)
{
	n = pfs::min(from._capacity - off_from, n);
	n = pfs::min(to._capacity - off_to, n);
	for (size_t i = off_from, j = off_to; n > 0; --n, ++i, ++j) {
		to._head[i] = from._head[j];
	}
}

template <typename T>
void array<T>::deep_copy (array & to, const T * from, size_t off_to, size_t n)
{
	n = pfs_min(to._capacity - off_to, n);
	for (size_t i = off_to, j = 0; j < n; ++i, ++j) {
		to._head[i] = from[j];
	}
}


template <typename T>
inline bool array<T>::eq (const array &a)
{
	return _capacity == a._capacity
			&& memcmp(_head, a._head, _capacity * sizeof(T)) == 0
			? true : false;
}

/**
* @brief Reallocate array with preserving the data.
*
* @param a Source array.
* @param new_capacity New capacity.
* @return Reallocated array.
*/
template <typename T>
void array<T>::realloc (size_t new_capacity)
{
	if (new_capacity != _capacity) {
		T *new_head = new T[new_capacity];
		if (_head) {
			deep_copy(new_head, _head, pfs::min(_capacity, new_capacity));
			delete[] _head;
		}
		_head = new_head;
		_capacity = new_capacity;
	}
}


/**
* @brief Move data inside an array.
*
* @param a array.
* @param off_to Offset to move data.
* @param off_from Offset from move data.
* @param n Number of items to move.
* @return Number of actually moved items.
*/
template <typename T>
void array<T>::move (size_t off_to, size_t off_from, size_t n)
{
	PFS_ASSERT(off_to + n <= _capacity && off_from + n <= _capacity);

	n = pfs::min(n, _capacity - off_from);
	n = pfs::min(n, _capacity - off_to);

	memmove(_head + off_to, _head + off_from, n * sizeof(T));
}

template <typename T>
void array<T>::swap(array<T> & other)
{
	size_t   capacity = _capacity;
	uint16_t raw      = _raw;
	T       *head     = _head;

	_capacity = other._capacity;
	_raw      = other._raw;
	_head     = other._head;

	other._capacity = capacity;
	other._raw      = raw;
	other._head     = head;
}

} // pfs

#endif /* __PFS_ARRAY_P_HPP__ */
