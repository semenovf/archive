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
	size_t   capacity;
	uint32_t raw : 1; /* is raw data */
	T       *head;

	Array(T a[]);
	Array(size_t sz, bool zero = false);
	~Array() { if(!raw) delete head; }

	T&          at(size_t index) { CWT_ASSERT(index < capacity); return head[index]; }
	const T&    at(size_t index) const { CWT_ASSERT(index < capacity); return head[index]; }
	void        bzero ()       { if(head) memset(head, 0, capacity * sizeof(T)); }
	Array*      clone ();
	void        copy  (Array &to, size_t off_to, size_t off_from, size_t n);
	void*       data  ()       { return head; }
	const void* data  () const { return head; }
	bool        eq    (const Array &a);
	void        move  (size_t off_to, size_t off_from, size_t n);
	void        realloc (size_t new_capacity);
	size_t      size  () const { return capacity; }
};

template <typename T>
inline Array<T>::Array(T a[])
	: capacity(0), raw(1), head(NULL)
{
	if(a) {
		capacity = sizeof(a);
		head = &a[0];
	}
}

template <typename T>
Array<T>::Array(size_t sz, bool zero)
	: capacity(0), raw(0), head(NULL)
{
	if (sz) {
		head = new T[sz];
		capacity = sz;

		if (zero) {
			bzero();
		}
	}
}

template <typename T>
inline Array<T>::~Array()
{
	if(!raw)
		delete[] head;
}

template <typename T>
Array* Array<T>::clone ()
{
	Array *clone = new Array<T>(capacity);
	if (clone)
		copy(*clone, 0, 0, capacity);
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
void Array<T>::copy (Array &to, size_t off_to, size_t off_from, size_t n)
{
	n = CWT_MIN(capacity - off_from, n);
	n = CWT_MIN(to.capacity - off_to, n);
	memcpy(to.head + off_to, head + off_from, n * sizeof(T));
}

template <typename T>
inline bool Array<T>::eq (const Array &a)
{
	return capacity == a.capacity
			&& memcmp(head, a.head, capacity * sizeof(T)) == 0
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
	if (new_capacity != capacity) {
		T *new_head = new T[new_capacity];
		memcpy(new_head, head, CWT_MIN(capacity, new_capacity));
		delete[] head;
		head = new_head;
		capacity = new_capacity;
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
	CWT_ASSERT(off_to >= capacity || off_from >= capacity);

	n = CWT_MIN(n, capacity - off_from);
	n = CWT_MIN(n, capacity - off_to);

	memmove(head + off_to, head + off_from, n * sizeof(T));
}


CWT_NS_END

#endif /* __CWT_ARRAY_P_HPP__ */
