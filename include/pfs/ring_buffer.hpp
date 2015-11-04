/*
 * ring_buffer.hpp
 *
 *  Created on: Sep 23, 2015
 *      Author: wladt
 */

#ifndef __PFS_RING_BUFFER_HPP__
#define __PFS_RING_BUFFER_HPP__

#include <pfs.hpp>
#include <pfs/iterator.hpp>

namespace pfs {

// InputIterator requirements:
// 		operator < (const InputIterator & other)
//
template <typename InputIterator>
class ring_buffer
{
public:
	typedef typename std::iterator_traits<InputIterator>::value_type value_type;

private:
	InputIterator _begin;
	InputIterator _end;
	InputIterator _head;
	InputIterator _tail;

public:
	ring_buffer (InputIterator begin, InputIterator end)
		: _begin(begin)
		, _end(end)
		, _head(begin)
		, _tail(begin)
	{}

	~ring_buffer () {}

	bool empty () const
	{
		return _head == _tail;
	}

	/**
	 * @brief Returns maximum elements can store buffer.
	 * @return Maximum value count.
	 */
	size_t capacity () const
	{
		return std::distance(_begin, _end);
	}

	size_t size () const
	{
		if (_head <= _tail)
			return std::distance(_head, _tail);

		return std::distance(_begin, _tail) + std::distance(_head, _end);
	}

	/**
	 * @brief Checks if buffer can store one element.
	 * @return
	 */
	bool can_push () const
	{
		return can_push(1);
	}

	bool can_push (size_t count) const
	{
		return capacity() - size() >= count;
	}

	/**
	 * @brief Adds data to the end of the rung buffer.
	 * @param value Data to be added.
	 */
	void push (const value_type & value);

	void pop ();

	void pull (value_type * x)
	{
		PFS_ASSERT(!empty());
		*x = *_head;
	}

	void pull_and_pop (value_type * x)
	{
		pull(x);
		pop();
	}
};

template <typename InputIterator>
void ring_buffer<InputIterator>::push (const value_type & value)
{
	PFS_ASSERT(can_push(1));

	if (_tail == _end)
		_tail = _begin;

	*_tail = value;
	++_tail;
}

template <typename InputIterator>
void ring_buffer<InputIterator>::pop ()
{
	if (empty())
		return;

	_head->~value_type();
	++_head;
}

} // pfs

#endif /* __PFS_RING_BUFFER_HPP__ */
