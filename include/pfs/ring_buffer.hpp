/*
 * ring_buffer.hpp
 *
 *  Created on: Sep 23, 2015
 *      Author: wladt
 */

#ifndef __PFS_RING_BUFFER_HPP__
#define __PFS_RING_BUFFER_HPP__

#include <pfs.hpp>
#include <pfs/noncopyable.hpp>

namespace pfs {

template <typename T>
class ring_buffer : noncopyable
{
public:
	typedef size_t size_type;
	typedef T      item_type;

private:
	size_type   _head;
	size_type   _count;
	size_type   _maxSize;
	item_type * _data;

private:
	void destroy_and_remove (size_t count);

public:
	ring_buffer (size_type maxSize)
		: _head(0)
		, _count(0)
		, _maxSize(maxSize)
	{
		_data = reinterpret_cast<T *>(new char[maxSize * sizeof(T)]);
	}

	~ring_buffer ()
	{
		destroy_and_remove(_count);
		delete [] reinterpret_cast<char *>(_data);
	}

	bool isEmpty () const
	{
		return _count == 0;
	}

	bool empty () const
	{
		return _count == 0;
	}

	size_type size () const
	{
		return _count;
	}

	size_type count () const
	{
		return _count;
	}

	bool canPush (size_type count = 1) const
	{
		return _maxSize - _count >= count;
	}

	/**
	 * @brief Adds data to the end of the rung buffer.
	 * @param value Data to be added.
	 *
	 * @note @a T must be copyable.
	 */
	template <typename U>
	void push (const U & value);

	void pop ()
	{
		destroy_and_remove(1);
	}

	template <typename U>
	void pop (U & x)
	{
		PFS_ASSERT(_count > 0);
		x = reinterpret_cast<U &>(_data[_head]);
		destroy_and_remove(1);
	}
};

template <typename T>
template <typename U>
void ring_buffer<T>::push (const U & value)
{
	PFS_ASSERT(sizeof(U) <= sizeof(T));
	PFS_ASSERT(canPush(1));

	size_type index = _count;

	// Is there a space at the right of the head or not ?
	//
	if (_head + _count > _maxSize ) {
		// No space at the right of the head,
		// calculate index to push new data
		//
		index = _head - (_maxSize - _count);
	}

	new (& _data[index]) U(value); // apply placement new
	++_count;
}

template <typename T>
void ring_buffer<T>::destroy_and_remove (size_t count)
{
	if (count > _count)
		count = _count;

	while (count > 0) {
		_data[_head].~T();

		if (_head < _maxSize - 1)
			++_head;
		else
			_head = 0;

		--_count;
		--count;
	}
}

} // pfs

#endif /* __PFS_RING_BUFFER_HPP__ */
