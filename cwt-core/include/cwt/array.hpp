/**
 * @file   array.hpp
 * @author wladt
 * @date   Feb 18, 2013 4:25:32 PM
 *
 * @brief
 */

#ifndef __CWT_ARRAY_HPP__
#define __CWT_ARRAY_HPP__

#include <cwt/cwt.h>
#include <cstring>

CWT_NS_BEGIN

template <typename T>
class Array {
	size_t   m_capacity;
	uint16_t m_raw : 1; /* is raw data */
	T *      m_head;

public:
	Array() : m_capacity(0), m_raw(0), m_head(nullptr) {}
	Array(T a[], size_t n);
	Array(size_t sz, bool zero = false);
	~Array();

	Array(const Array & other);
	Array &	   operator = (const Array & other);

	void        alloc (size_t capacity);
	T &         at(size_t index) { CWT_ASSERT(index < m_capacity); return m_head[index]; }
	const T &   at(size_t index) const { CWT_ASSERT(index < m_capacity); return m_head[index]; }
	int         compare (size_t pos, const Array & other, size_t subpos, size_t len) const;
	int         compare (const Array & other, size_t len) const { return compare(0, other, 0, len); }
	int         compare (const Array & other) const { return compare(0, other, 0, other.m_capacity); }
	void        bzero ()      { if(m_head) ::memset(m_head, 0, m_capacity * sizeof(T)); }
	void        set   (int c) { if(m_head) ::memset(m_head, c, m_capacity * sizeof(T)); }
	void        set   (int c, size_t off) { CWT_ASSERT(off < m_capacity); if(m_head) ::memset(m_head + off, c, (m_capacity - off) * sizeof(T)); }
	Array*      clone () const;
	T*          data  ()       { return m_head; }
	const T*    data  () const { return m_head; }
	const T*    constData () const { return m_head; }
	bool        eq    (const Array & a);
	bool        isRaw() const { return m_raw; }
	void        move  (size_t off_to, size_t off_from, size_t n);
	void        realloc (size_t new_capacity);
	size_t      size  () const { return m_capacity; }
	size_t      capacity() const { return m_capacity; }
	void        swap(Array<T> & other);


	T&          operator [] (size_t index) { return at(index); }
	const T&    operator [] (size_t index) const { return at(index); }

	static void copy (Array &to, const Array &from, size_t off_to, size_t off_from, size_t n);
	static void copy (Array &to, const T *from, size_t off_to, size_t off_from, size_t n);

	static void deep_copy (T * to, const T * from, size_t n);
	static void deep_copy (Array &to, const Array &from, size_t off_to, size_t off_from, size_t n);
	static void deep_copy (Array &to, const T *from, size_t off_to, size_t off_from, size_t n);
};

template <typename T>
inline Array<T>::Array(T a[], size_t n)
	: m_capacity(0), m_raw(1), m_head(nullptr)
{
	if (a) {
		m_capacity = n;
		m_head = & a[0];
	}
}

template <typename T>
inline Array<T>::Array(const Array & other)
	: m_capacity(other.m_capacity), m_raw(0), m_head(nullptr)
{
	if (other.m_capacity > 0) {
		alloc(other.m_capacity);
		deep_copy(*this, other, 0, 0, other.m_capacity);
	}
}

template <typename T>
inline Array<T> & Array<T>::operator = (const Array & other)
{
	if (other.m_capacity > 0) {
		alloc(other.m_capacity);
		deep_copy(*this, other, 0, 0, other.m_capacity);
	}
	return *this;
}

template <typename T>
inline Array<T>::Array(size_t sz, bool zero)
	: m_capacity(0), m_raw(0), m_head(nullptr)
{
	if (sz) {
		alloc(sz);

		if (zero) {
			bzero();
		}
	}
}

template <typename T>
inline Array<T>::~Array()
{
	if(!m_raw)
		delete[] m_head;
}

template <typename T>
inline void Array<T>::alloc (size_t size)
{
	if (!m_raw && m_head) {
		delete[] m_head;
	}
	m_head = new T[size];
	m_raw = 0;
	m_capacity = size;
}

template <typename T>
inline int Array<T>::compare (size_t pos, const Array & other, size_t subpos, size_t len) const
{
	CWT_ASSERT(pos < m_capacity && subpos < other.m_capacity);
	return memcmp(m_head + pos, other.m_head + subpos, len * sizeof(T));
}

template <typename T>
Array<T>* Array<T>::clone () const
{
	Array<T> * clone = new Array<T>(m_capacity);
	if (clone)
		copy(*clone, *this, 0, 0, m_capacity);
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
void Array<T>::copy (Array &to, const Array &from, size_t off_to, size_t off_from, size_t n)
{
	n = CWT_MIN(from.m_capacity - off_from, n);
	n = CWT_MIN(to.m_capacity - off_to, n);
	memcpy(to.m_head + off_to, from.m_head + off_from, n * sizeof(T));
}

template <typename T>
void Array<T>::copy (Array &to, const T *from, size_t off_to, size_t off_from, size_t n)
{
	n = CWT_MIN(to.m_capacity - off_to, n);
	memcpy(to.m_head + off_to, from + off_from, n * sizeof(T));
}

template <typename T>
void Array<T>::deep_copy (T * to, const T * from, size_t n)
{
	while (n--) {
		*to++ = *from++;
	}
}

template <typename T>
void Array<T>::deep_copy (Array &to, const Array &from, size_t off_to, size_t off_from, size_t n)
{
	n = CWT_MIN(from.m_capacity - off_from, n);
	n = CWT_MIN(to.m_capacity - off_to, n);
	for (size_t i = off_from, j = off_to; n > 0; --n, ++i, ++j) {
		to.m_head[i] = from.m_head[j];
	}
}

template <typename T>
void Array<T>::deep_copy (Array &to, const T *from, size_t off_to, size_t off_from, size_t n)
{
	n = CWT_MIN(to.m_capacity - off_to, n);
	for (size_t i = off_from, j = off_to; n > 0; --n, ++i, ++j) {
		to.m_head[i] = from[j];
	}
}


template <typename T>
inline bool Array<T>::eq (const Array &a)
{
	return m_capacity == a.m_capacity
			&& memcmp(m_head, a.m_head, m_capacity * sizeof(T)) == 0
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
void Array<T>::realloc (size_t new_capacity)
{
	if (new_capacity != m_capacity) {
		T *new_head = new T[new_capacity];
		if (m_head) {
			deep_copy(new_head, m_head, CWT_MIN(m_capacity, new_capacity));
			delete[] m_head;
		}
		m_head = new_head;
		m_capacity = new_capacity;
	}
}


/**
* @brief Move data inside an array.
*
* @param a Array.
* @param off_to Offset to move data.
* @param off_from Offset from move data.
* @param n Number of items to move.
* @return Number of actually moved items.
*/
template <typename T>
void Array<T>::move (size_t off_to, size_t off_from, size_t n)
{
	CWT_ASSERT(off_to + n <= m_capacity && off_from + n <= m_capacity);

	n = CWT_MIN(n, m_capacity - off_from);
	n = CWT_MIN(n, m_capacity - off_to);

	memmove(m_head + off_to, m_head + off_from, n * sizeof(T));
}

template <typename T>
void Array<T>::swap(Array<T> & other)
{
	size_t   capacity = m_capacity;
	uint16_t raw      = m_raw;
	T       *head     = m_head;

	m_capacity = other.m_capacity;
	m_raw      = other.m_raw;
	m_head     = other.m_head;

	other.m_capacity = capacity;
	other.m_raw      = raw;
	other.m_head     = head;
}


CWT_NS_END

#endif /* __CWT_ARRAY_P_HPP__ */
